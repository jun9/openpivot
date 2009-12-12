#include "Accumulator.h"
#include <math.h>
#include <iostream>
namespace openpivot
{
  Accumulator::Accumulator ():
    mNbEntries(0),
    mSum(0),
    mSquarredSum(0),
    mMethod(&Accumulator::getNbEntries)
  {
    
  }
  
  Accumulator::~Accumulator ()
  {
    
  }
  
  void Accumulator::getNbEntries(double & val) const
  {
    val = mNbEntries;
  }
  
  void Accumulator::getSum(double & val) const
  {
    val = mSum;
  }
  
  void Accumulator::getAverage(double & val) const
  {
    val = mNbEntries ? mSum / (double)mNbEntries : 0;
  }
  
  void Accumulator::getVariance(double & val) const
  {
    double coef = (mNbEntries > 1) ? mNbEntries / (mNbEntries - 1) : 0;
    double term1 = (mNbEntries > 1) ? mSquarredSum / mNbEntries : 0;
    double term2 = (mNbEntries > 1) ? (mSum * mSum) / (mNbEntries * mNbEntries) : 0;
    val = (term1 - term2) * coef;
  }
  
  void Accumulator::getStdDev(double &val) const 
  {
    getVariance(val);
    val = sqrt(val);
  }
  
  void Accumulator::addEntry(double val)
  {
    mNbEntries += 1;
    mSum += val;
    mSquarredSum += val * val;
  }
  
  double Accumulator::getValue() const
  {
    double val = 0;
    (this->*mMethod)(val);
    return val;
  }
  
  void Accumulator::setAggType(aggregation_type type)
  {
    switch (type) {
      
      case average :
        mMethod = &Accumulator::getAverage;
        break;
      case sum:
        mMethod = &Accumulator::getSum;
        break;
      case number :
        mMethod = &Accumulator::getNbEntries;
        break;
      case variance :
        mMethod = &Accumulator::getVariance;
        break;
      case stddev :
        mMethod = &Accumulator::getStdDev;
        break;
      default:
        mMethod = &Accumulator::getNbEntries;
        break;
      }
  }
  
} /* op */