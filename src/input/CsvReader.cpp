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

#define SPACES " \t\r\n"
using std::string;

inline string trim_right (const string & s, const string & t = SPACES)
{ 
  string d (s); 
  string::size_type i (d.find_last_not_of (t));
  if (i == string::npos)
    return "";
  else
   return d.erase (d.find_last_not_of (t) + 1) ; 
}  // end of trim_right
  
  
namespace op
{
  #define __DEFAULT_BUFFER_SIZE__ 255
  
  using namespace std;
  
  CsvReader::CsvReader():
    mSeparator(';'),
    mNbTokens(0),
    mCurrentBuffer(0),
    mContext(0),
    mFileName() ,
    mPositions(),
    mHeadersMap()
  {
#ifndef NO_GOOGLE_HASH
    mHeadersMap.set_empty_key(string());
#endif
  }
  
  CsvReader::~CsvReader()
  {
    
  }
  
  void CsvReader::setInput(const string & name)
  {
    mFileName = name;
  }
  
  bool CsvReader::read()
  {
    ifstream stream (mFileName.c_str(), ios_base::in);
    if (!stream)
    {
       cout << "While opening a file an error is encountered" << endl;
    }
    mCurrentBuffer = new char[__DEFAULT_BUFFER_SIZE__];
    //handle headers :
    if (!stream.eof()) 
    {
      stream.getline(mCurrentBuffer,__DEFAULT_BUFFER_SIZE__);
      tokenizeBuffer();
      processHeader();
    }

    //process lines :
    while (!stream.eof()) {
      stream.getline(mCurrentBuffer,__DEFAULT_BUFFER_SIZE__);
      tokenizeBuffer();
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
   
    for (int i = 0; i < __DEFAULT_BUFFER_SIZE__; ++i) {
      if (mCurrentBuffer[i] == '\n') // FIXME: will probably not work on windows ...
        break;
      if (mCurrentBuffer[i] == mSeparator) {
        mCurrentBuffer[i] = 0;
        mNbTokens++;
        mPositions.push_back(i+1);
      }
      
    }
  }
  
  bool CsvReader::processLine()
  {
    string key = buildKey();
    Settings & settings = mContext->getSettings();
    PivotTable & table = mContext->getPivotTable();
    Settings::ColsMapIteratorPair iterPair = settings.iterColumns();
    
    //iterate over all columns to enrich all corresponding Accumulators
    for (Settings::ColsMapIterator iter = iterPair.first; iter != iterPair.second; ++iter)
    {
      string field = iter->first;
      Accumulator* acc = table.getAccumulatorForKeyAndEntry(key,field);
      int pos =  mHeadersMap.find(field)->second;
      string val = getFromTokens(pos);
      try 
      {
        double doubleVal = Utils::toDouble(val);
        acc->addEntry(doubleVal);
      }catch ( string & str)
      {
        std::cerr <<"   ignored : " <<  str << endl;
      }
    }
    return true;
  }
  
  bool CsvReader::processHeader()
  {
    checkHeaders();
    for (int j = 0; j < mNbTokens; ++j)
    {
      string field = getFromTokens(j);
      //if field is a column :
      Settings & settings = mContext->getSettings();
      if (settings.hasColumn(field) || settings.hasRow(field)) 
      {
        //cerr << "Adding : " << field.c_str() << endl;
        bool hein = mHeadersMap.insert(std::make_pair(field,j)).second;
        if (!hein) 
        {
          cerr << "problem here " << endl;
        }
      }
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
      string col = iter->first;
      bool is = false;
      for (int i = 0; i < mNbTokens; ++i)
      {
        string field = getFromTokens(i);
        if (field == col)
        {
          is = true;
          break;
        }
      }
      if (!is)
      {
        string message = "Error - Column " + col + " missing from headers";
        throw Exception (message);
      }
    }
    //now ckeck for rows
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<string>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      string row = *it;
      bool is = false;
      for (int i = 0; i < mNbTokens; ++i)
      {
        string field = getFromTokens(i);
        if (field == row)
        {
          is = true;
          break;
        }
      }
      if (!is)
      {
        string message = "Error - Row " + row + " missing from headers";
        throw Exception (message);
      }
    }
    
    
  }
  
  string CsvReader::getFromTokens(int pos) const
  {
    int position = mPositions[pos];
    string field = &mCurrentBuffer[position];
    return field;
  }
  
  string CsvReader::buildKey()
  {
    Settings & settings = mContext->getSettings();
    string key = "";
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<string>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      string field = *it;
      int indice = mHeadersMap.find(field)->second;
      string el = getFromTokens(indice);
      key += el + ";";
    }
    return key;
  }
  
  void CsvReader::setContext(OpenPivotContext * contex)
  {
    mContext = contex;
  }
  
  
} /* op */
