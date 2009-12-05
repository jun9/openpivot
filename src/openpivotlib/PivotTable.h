#ifndef PIVOTTABLE_H_UCDSE4RV
#define PIVOTTABLE_H_UCDSE4RV

#ifdef NO_GOOGLE_HASH
#include <map>
#else
#include <google/dense_hash_map> // very fast but memory intensive
#endif

//#include <string>
#include "Accumulator.h"
#include "openpivotlib/Utils.h"

namespace op
{

  //using std::string;

class PivotTable
{
public:
  PivotTable ();
  ~PivotTable ();
#ifdef NO_GOOGLE_HASH
  typedef std::map<const char*,Accumulator*,Utils::StrComp> EntriesMap;
  typedef std::map<const char*,EntriesMap*,Utils::StrComp> Container;
#else
  typedef google::dense_hash_map<const char*,Accumulator*,Utils::Hash, Utils::eqstr > EntriesMap;
  typedef google::dense_hash_map<const char*,EntriesMap*,Utils::Hash , Utils::eqstr> Container;
#endif
  
  
  Accumulator* getAccumulatorForKeyAndEntry(const char*, const char* );
  typedef std::pair<Container::const_iterator,Container::const_iterator> IterEntriesPair;
  IterEntriesPair iterEntries() const;
  void cleanup();
  
private:
  Container mDocumentMap;
};

} /* op */
#endif /* end of include guard: PIVOTTABLE_H_UCDSE4RV */
