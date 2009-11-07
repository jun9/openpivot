#ifndef ACCUMULATOR_H_KG5GC13U
#define ACCUMULATOR_H_KG5GC13U

#include "openpivotlib/Utils.h"

namespace op
{
  
class Accumulator
{
public:
  Accumulator ();
  virtual ~Accumulator ();
  
  void addEntry(double);
  void setAggType(aggregation_type);
  double getValue() const;

private:
  typedef void (Accumulator::*Method)(double &) const;

private:
  void getNbEntries(double &) const;
  void getSum(double &) const;
  void getAverage(double &) const;
  void getVariance(double &) const;
  void getStdDev(double &) const;
  
  
  
private:
  int mNbEntries;
  double mSum;
  double mSquarredSum;
  Method mMethod;
};
} /* op */
#endif /* end of include guard: ACCUMULATOR_H_KG5GC13U */
