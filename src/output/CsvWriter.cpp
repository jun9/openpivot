#include <fstream>
#include "CsvWriter.h"
#include "openpivotlib/PivotTable.h"
#include "openpivotlib/AggregationColumn.h"
namespace op
{
  using namespace std;
  
  CsvWriter::CsvWriter ():
    mContext(0),
    mFileName()
  {
    
  }
  
  CsvWriter::~CsvWriter ()
  {
    
  }
  
  void CsvWriter::setContext(OpenPivotContext * context)
  {
    mContext = context;
  }
  
  void CsvWriter::setOutput(const string & name)
  {
    mFileName = name;
  }
  
  bool CsvWriter::dump()
  {
    ofstream stream (mFileName.c_str(), ios_base::out);
    if (!stream)
    {
       cout << "While opening a file an error is encountered" << endl;
    }
    PivotTable & table = mContext->getPivotTable();
    size_t size = 256;
    size_t currentSize = 0;
    char* buffer = new char[size];
    buffer[0] = '\0';
    Settings &settings = mContext->getSettings();
    
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<const char*>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      const char* field = *it;
      currentSize += strlen(field) + 1;
      if (currentSize >= size)
      {
        size *= 2; 
        char* newBuff = new char [size];
        newBuff[0] = '\0';
        strcpy(newBuff, buffer);
        delete[] buffer;
        buffer = newBuff;
      }
      strcat(buffer, field);
      strcat(buffer, ";");
    }
    
    Settings::ColsMapIteratorPair iterPair = settings.iterColumns();
    for (Settings::ColsMapIterator iter = iterPair.first; iter != iterPair.second; ++iter)
    {
      const char* field = iter->first;
      currentSize += strlen(field) + 1;
      if (currentSize >= size)
      {
        size *= 2;
        char* newBuff = new char [size];
        newBuff[0] = '\0';
        strcpy(newBuff, buffer);
        delete[] buffer;
        buffer = newBuff;
      }
      strcat(buffer, field);
      strcat(buffer, ";");
    }
    stream << buffer << std::endl;
    
    
    PivotTable::IterEntriesPair iterEntriesPair = table.iterEntries();
    for (;iterEntriesPair.first != iterEntriesPair.second; ++iterEntriesPair.first)
    {
      if (buffer)
        delete [] buffer;
      size = 256;
      currentSize = 0;
      buffer = new char[size];
      buffer[0] = '\0';
      //buffer = string();
      const char* field = iterEntriesPair.first->first;
      currentSize += strlen(field);
      if (currentSize >= size)
      {
        size *= 2;
        char* newBuff = new char [size];
        newBuff[0] = '\0';
        strcpy(newBuff, buffer);
        delete[] buffer;
        buffer = newBuff;
      }
      strcat(buffer, field);
      //buffer += iterEntriesPair.first->first;
      stream << buffer ;
      PivotTable::EntriesMap theMap = *((iterEntriesPair.first)->second);
      for (PivotTable::EntriesMap::const_iterator iterAccu = theMap.begin(); iterAccu != theMap.end(); ++iterAccu)
      {
        Accumulator* acc = iterAccu->second;
        aggregation_type type = settings.getAggregationColumn(iterAccu->first)->getAggregationType();
        acc->setAggType(type);
        double val = acc->getValue();
        //buffer += iterAccu->second + ";";
        stream << val << ";";
      }
      stream << std::endl;
    }
    delete [] buffer;
    return true;
  }
  
} /* op */