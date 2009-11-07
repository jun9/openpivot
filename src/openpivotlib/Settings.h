
#ifndef SETTINGS_H_I5PA6WWL
#define SETTINGS_H_I5PA6WWL

#include <string>
//#include <google/dense_hash_map>
//#include <google/dense_hash_set>

#include "Utils.h"
#include <vector>
#include <list>
#include <set>
#include <map>
namespace op {

using std::string;
using std::list;

class AggregationColumn;

class Settings 
{
public:
  typedef list<string>::const_iterator RowIterator;
  typedef std::pair<RowIterator,RowIterator> RowIteratorPair; 
  typedef std::map<const char*,AggregationColumn*,Utils::StrComp>::const_iterator ColsMapIterator;
  typedef std::pair<ColsMapIterator,ColsMapIterator> ColsMapIteratorPair;
  
  Settings();
  ~Settings();
  
  void addColumnWithAggregTypeForKey(const string &, aggregation_type);
  void addRowForKey(const string &);
  void setDefaultAccumulation(aggregation_type);
  
  bool hasColumn(const string &) const;
  bool hasRow(const string &) const;
  AggregationColumn* getAggregationColumn(const string &);
  
  RowIteratorPair iterRows() const;
  ColsMapIteratorPair iterColumns() const;
  
private:
  typedef std::map<const char*,AggregationColumn*,Utils::StrComp> ColumnMap;
  typedef std::set<const char*,Utils::StrComp> RowsContainer;

private:
  aggregation_type mDefautlAggregation;
  ColumnMap mColumns;
  RowsContainer mRowsMap;
  list<string> mRows;
};
}

 #endif /* end of include guard: AGGREGATIONCOLUMN_H_UC9VVJXI */