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
    std::cout << "Dumping" << endl;
    ofstream stream (mFileName.c_str(), ios_base::out);
    if (!stream)
    {
       cout << "While opening a file an error is encountered" << endl;
    }
    PivotTable & table = mContext->getPivotTable();
    string buffer;
    Settings &settings = mContext->getSettings();
    
    Settings::RowIteratorPair itPair = settings.iterRows();
    for (std::list<string>::const_iterator it = itPair.first; it != itPair.second; ++it  )
    {
      string field = *it;
      //int indice = mHeadersMap.find(field)->second;
      //string el = getFromTokens(indice);
      buffer += field + ";";
    }
    
    Settings::ColsMapIteratorPair iterPair = settings.iterColumns();
    for (Settings::ColsMapIterator iter = iterPair.first; iter != iterPair.second; ++iter)
    {
      string field = iter->first;
      buffer += field;
      buffer += ";";
    }
    stream << buffer << std::endl;
    
    
    PivotTable::IterEntriesPair iterEntriesPair = table.iterEntries();
    for (;iterEntriesPair.first != iterEntriesPair.second; ++iterEntriesPair.first)
    {
      buffer = string();
      buffer += iterEntriesPair.first->first;
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
    
    return true;
  }
  
} /* op */