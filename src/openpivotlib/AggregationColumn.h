/*
 *  AggregationColumn.h
 *  openpivot
 *
 *  Created by Romain Pechayre on 9/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef AGGREGATIONCOLUMN_H_UC9VVJXI
#define AGGREGATIONCOLUMN_H_UC9VVJXI
 
#include <string>
#include "Settings.h"

namespace op
{
using std::string;


class AggregationColumn
{
public:
  AggregationColumn ();
  ~AggregationColumn ();
  
  void setId(const string &);
  void setAggregationType(aggregation_type);
  
  aggregation_type getAggregationType() const;
  
private:
  aggregation_type mAggregType;
  string mId;
  
  
};

} /* op */

 #endif /* end of include guard: AGGREGATIONCOLUMN_H_UC9VVJXI */