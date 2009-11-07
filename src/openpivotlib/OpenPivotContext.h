#ifndef OPENPIVOTCONTEXT_H_KDF33K5S
#define OPENPIVOTCONTEXT_H_KDF33K5S

#include "Settings.h"
#include "PivotTable.h"

namespace op
{
  /**
  *@brief Object containning all data needed for an OpenPivot instance
  */
  class OpenPivotContext
  {
  public:
    /**
    *@brief Default constructor
    */
    OpenPivotContext ();
    
    /**
    *@brief Destructor
    */
    ~OpenPivotContext ();
    
    Settings & getSettings();
    PivotTable & getPivotTable();
     
  private:
    Settings mSettings;
    PivotTable mTable;
  };
  
  inline Settings & OpenPivotContext::getSettings() { return mSettings;}
  inline PivotTable & OpenPivotContext::getPivotTable() { return mTable;}
} /* op */

#endif /* end of include guard: OPENPIVOTCONTEXT_H_KDF33K5S */
