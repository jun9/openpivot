/*
 *  Settings.cpp
 *  openpivot
 *
 *  Created by Romain Pechayre on 9/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Settings.h"
#include "AggregationColumn.h"
#include <iostream>
namespace openpivot
{
  
Settings::Settings():
  mDefautlAggregation(average),
  mColumns(),
  mRowsMap(),
  mRows()
{

}

Settings::~Settings()
{
  
}

bool Settings::hasColumn(const char*  field) const
{
  ColumnMap::const_iterator finder = mColumns.find(field);
  return finder != mColumns.end();
}

bool Settings::hasRow(const char* field) const
{
  //std::cerr << "dealing with in HasRow " << field << " currently " << mRowsMap.size() << std::endl;
  RowsContainer::const_iterator finder = mRowsMap.find(field);
  return finder != mRowsMap.end();
}
  
void Settings::addColumnWithAggregTypeForKey(const char* field, aggregation_type type)
{
  ColumnMap::iterator finder = mColumns.find(field);
  if (finder == mColumns.end())
  {
    AggregationColumn* col = new AggregationColumn();
    if (type == def)
      col->setAggregationType(mDefautlAggregation);
    else
      col->setAggregationType(type);
    col->setId(field);  
    mColumns.insert(std::make_pair(field, col));
  }
}

void Settings::setDefaultAccumulation(aggregation_type type)
{
  mDefautlAggregation = type;
}

AggregationColumn* Settings::getAggregationColumn(const char*  name)
{
  AggregationColumn* col = 0;
  ColumnMap::iterator finder = mColumns.find(name);
  if (finder != mColumns.end())
  {
    col = finder->second;
  } 
  return col;
}

void Settings::addRowForKey(const char* name)
{
  //std::cerr <<"trying adding  row " << name;
  if (  mRowsMap.insert(name).second)
  {
    //std::cerr <<" success" ;
    mRows.push_back(name);
  }
  //std::cerr << std::endl;

}

Settings::RowIteratorPair Settings::iterRows() const
{
  return std::make_pair(mRows.begin(),mRows.end());
}

Settings::ColsMapIteratorPair Settings::iterColumns() const
{
  return std::make_pair(mColumns.begin(), mColumns.end());
}
} /* openpivot */