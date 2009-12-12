/*
 *  AggregationColumn.cpp
 *  openpivot
 *
 *  Created by Romain Pechayre on 9/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AggregationColumn.h"

namespace openpivot
{
  
  AggregationColumn::AggregationColumn ():
    mAggregType(average),
    mId()
  {
    
  }
  
  AggregationColumn::~AggregationColumn ()
  {
    
  }
  
  void AggregationColumn::setId(const string & id) 
  {
    mId = id;
  }
  
  void AggregationColumn::setAggregationType(aggregation_type type)
  {
    mAggregType = type;
  }
  
  aggregation_type AggregationColumn::getAggregationType() const
  {
    return mAggregType;
  }
} /* op */