
#ifndef SETTINGS_H_I5PA6WWL
#define SETTINGS_H_I5PA6WWL

//#include <string>

#include "Utils.h"
#include <vector>
#include <list>
#include <set>
#include <map>
namespace op {

//using std::string;
using std::list;

class AggregationColumn;

class Settings 
{
public:
  typedef list<const char*>::const_iterator RowIterator;
  typedef std::pair<RowIterator,RowIterator> RowIteratorPair; 
  typedef std::map<const char*,AggregationColumn*,Utils::StrComp>::const_iterator ColsMapIterator;
  typedef std::pair<ColsMapIterator,ColsMapIterator> ColsMapIteratorPair;
  
  Settings();
  ~Settings();
  
  void addColumnWithAggregTypeForKey(const char* , aggregation_type);
  void addRowForKey(const char* );
  void setDefaultAccumulation(aggregation_type);
  
  bool hasColumn(const char* ) const;
  bool hasRow(const char* ) const;
  AggregationColumn* getAggregationColumn(const char* );
  
  RowIteratorPair iterRows() const;
  ColsMapIteratorPair iterColumns() const;
  
private:
  typedef std::map<const char*,AggregationColumn*,Utils::StrComp> ColumnMap;
  typedef std::set<const char*,Utils::StrComp> RowsContainer;

private:
  aggregation_type mDefautlAggregation;
  ColumnMap mColumns;
  RowsContainer mRowsMap;
  list<const char*> mRows;
};
}

 #endif /* end of include guard: AGGREGATIONCOLUMN_H_UC9VVJXI */