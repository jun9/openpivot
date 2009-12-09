/*
 *  CsvReader.cpp
 *  openpivot
 *
 *  Created by Romain Pechayre on 9/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "CsvReader.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

//#define SPACES " \t\r\n"
//using std::string;

  
namespace op
{
  #define __DEFAULT_BUFFER_SIZE__ 4096
  
  using namespace std;
  
  CsvReader::CsvReader():
    mSeparator(';'),
    mNbTokens(0),
    mCurrentBuffer(0),
    mFileName(0) ,  
    mContext(0),
    mPositions(),
    mRowsIndices(),
    mHeadersMap()
  {
#ifndef NO_GOOGLE_HASH
    mHeadersMap.set_empty_key(0);
#endif
  }
  
  CsvReader::~CsvReader()
  {
    
  }
  
  void CsvReader::setInput(const char*  name)
  {
    mFileName = name;
  }
  
  bool CsvReader::read()
  {
    ifstream stream (mFileName, ios_base::in);
    if (!stream)
    {
       cout << "While opening a file an error is encountered" << endl;
    }
    mCurrentBuffer = new char[__DEFAULT_BUFFER_SIZE__];
    mCurrentBuffer[0] = '\0';
    
    //handle headers :
    if (!stream.eof()) 
    {
      stream.getline(mCurrentBuffer,__DEFAULT_BUFFER_SIZE__);
      try {
        tokenizeBuffer();
      }
      catch (Exception & exc) {
        std::cerr << "Headers cannot be tokeized" << std::endl;
      }
      
      processHeader();
    }

    //process lines :
    int cpt = 0;
    while (!stream.eof()) {
      cpt++;
      mCurrentBuffer[0] = '\0';
      stream.getline(mCurrentBuffer,__DEFAULT_BUFFER_SIZE__);
      try {
        tokenizeBuffer();
      }
      catch (Exception & exc) {
        //do nothing, just skip line
        continue;
      }
      processLine();
    }
    delete [] mCurrentBuffer; 
    return true;
  }
  
  void CsvReader::tokenizeBuffer()
  {
    mPositions.clear();
    mNbTokens = 1;
    mPositions.push_back(0);
    bool hasTokens = false;
    char* bufferBuffer = &mCurrentBuffer[0];
    for (int i = 0; i < __DEFAULT_BUFFER_SIZE__; ++i) {
      char & currentChar = *bufferBuffer;
      if (currentChar == '\0')
        break;
      if (currentChar == mSeparator) {
        hasTokens = true;
        currentChar = '\0';
        mNbTokens++;
        mPositions.push_back(i+1);
      }
      bufferBuffer++;
    }
    if (!hasTokens)
      throw Exception ("This line does not have tokens");
  }
  
  bool CsvReader::processLine()
  {
    const char* key = buildKey(); //delete if not used
    static Settings & settings = mContext->getSettings();
    static PivotTable & table = mContext->getPivotTable();
    Settings::ColsMapIteratorPair iterPair = settings.iterColumns();
    
    //iterate over all columns to enrich all corresponding Accumulators
    for (Settings::ColsMapIterator iter = iterPair.first; iter != iterPair.second; ++iter)
    {
      const char* field = iter->first;
      Accumulator* acc = table.getAccumulatorForKeyAndEntry(key,field);
      ReverseHeadersMap::const_iterator finder = mHeadersMap.find(field);
      /*for (ReverseHeadersMap::const_iterator itreMap  = mHeadersMap.begin(); itreMap != mHeadersMap.end(); ++itreMap) {
        cout << itreMap->first << "("<< (void*)itreMap->first<<")"<< " --> " << itreMap->second << std::endl;;
      }*/
      if (finder == mHeadersMap.end()) 
      {
        std::string mess = "Key "+ std::string(field)+ " not found in map"  ;
        throw Exception (mess);
      }
        
      int pos =  finder->second;
      const char* val = getFromTokens(pos);
      try 
      {
        double doubleVal = Utils::toDouble(val);
        acc->addEntry(doubleVal);
      }catch ( const char * str)
      {
        std::cerr <<"   ignored : " <<  str << endl;
      }
    }
    delete[] key;
    return true;
  }
  
  bool CsvReader::processHeader()
  {
    checkHeaders();
    for (int j = 0; j < mNbTokens; ++j)
    {
      const char* field = getFromTokens(j);
      //if field is a column :
      Settings & settings = mContext->getSettings();
      if (settings.hasColumn(field) || settings.hasRow(field)) 
      {
        //cerr << "Adding : " << field.c_str() << endl;
        //TODO : check here
        size_t length = strlen(field);
        char *newField  = new char[length] ;
        if(length)
          newField[0] = '\0';
        strcpy(newField, field);
        std::pair<const char*,int> insertPair = std::make_pair(newField,j);
        std::pair<ReverseHeadersMap::iterator,bool> res = mHeadersMap.insert(insertPair);
        /*assert(mHeadersMap.find(newField)->second == j && "Not found after copy an insertion");
        assert((Utils::Hash()(field) == Utils::Hash()(newField)) && "strings copied not equal");
        assert(Utils::eqstr()(field,newField) && "strings copied not equal");

        
        assert(mHeadersMap.find(field)->second == j && "Not found after insertion");
        assert(mHeadersMap[field] == j && "Not found after insertion");
         */
        bool hein = res.second;
        
        /*cout << "headers map size : " << mHeadersMap.size() << std::endl;
        for (ReverseHeadersMap::const_iterator itreMap  = mHeadersMap.begin(); itreMap != mHeadersMap.end(); ++itreMap) {
          cout << itreMap->first << " --> " << itreMap->second << std::endl;;
        }
        cout << std::endl;
         */
        if (!hein) 
        {
          cerr << "problem here " << endl;
        }
      }
    }
    
    Settings & settings = mContext->getSettings();
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<const char*>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      const char* field = *it;
      int indice = mHeadersMap.find(field)->second;
      mRowsIndices.push_back(indice);
    }
    return true;
  }
  
  void CsvReader::checkHeaders()
  {
    Settings & settings = mContext->getSettings();
    Settings::ColsMapIteratorPair iterPair = settings.iterColumns();
    //iterate over all columns to enrich all corresponding Accumulators
    for (Settings::ColsMapIterator iter = iterPair.first; iter != iterPair.second; ++iter)
    {
      const char* col = iter->first;
      bool is = false;
      for (int i = 0; i < mNbTokens; ++i)
      {
        const char* field = getFromTokens(i);
        if (strcmp(field, col) == 0)
        {
          is = true;
          break;
        }
      }
      if (!is)
      {
        string message = "Error - Column " + string(col) + " missing from headers";
        throw Exception (message);
      }
    }
    //now ckeck for rows
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<const char*>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      const char* row = *it;
      bool is = false;
      for (int i = 0; i < mNbTokens; ++i)
      {
        const char* field = getFromTokens(i);
        
        if (strcmp(field,row) == 0)
        {
          is = true;
          break;
        }
      }
      if (!is)
      {
        string message = "Error - Row " + string(row) + " missing from headers";
        throw Exception (message);
      }
    }
    
    
  }
  
  const char* CsvReader::getFromTokens(int pos) const
  {
    int position = mPositions[pos];
    return &mCurrentBuffer[position];
  }
  
  const char* CsvReader::buildKey()
  {
    //string key = string();
    size_t size = 32;
    char* buffer = new char[size];
    buffer[0] = '\0';
    
    size_t currentSize = 0;
    for (vector<int>::const_iterator iterRows = mRowsIndices.begin(); iterRows != mRowsIndices.end(); ++iterRows)
    {
      int pos = *iterRows;
      const char* current = getFromTokens(pos);
      currentSize += strlen(current) + 1;
      if (currentSize >= size)
      {
        char* newStr = new char[2*size];
        newStr[0] = '\0';
        strcpy(newStr, buffer);
        delete [] buffer;
        buffer = newStr;
      }
      strcat(buffer, current);
      strcat(buffer, ";");  
    }
    return buffer;
  }
  
  void CsvReader::setContext(OpenPivotContext * contex)
  {
    mContext = contex;
  }
  
  void CsvReader::clean()
  {
    for (ReverseHeadersMap::iterator iterHeaders = mHeadersMap.begin(); iterHeaders != mHeadersMap.end(); ++ iterHeaders)
    {
      if (iterHeaders->first) {
        delete [] iterHeaders->first;
        
      }
    }
  }
  
} /* op */
