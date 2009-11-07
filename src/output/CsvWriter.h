#ifndef CSVWRITER_H_UUXGVCCB
#define CSVWRITER_H_UUXGVCCB

#include "openpivotlib/OpenPivotContext.h"
namespace op
{
  
class CsvWriter
{
public:
  CsvWriter ();
  ~CsvWriter ();
  
  /**
  *@brief To set the output
  */
  void setOutput(const string &);
  
  /**
  *@brief To set the context
  */
  void setContext(OpenPivotContext *);
  /**
  * @brief main method : performs writing of the output 
  * file with built pivot table
  */
  bool dump();
  
private:
  OpenPivotContext* mContext;
  string mFileName;
};

} /* op */
#endif /* end of include guard: CSVWRITER_H_UUXGVCCB */
