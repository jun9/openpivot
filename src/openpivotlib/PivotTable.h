#ifndef PIVOTTABLE_H_UCDSE4RV
#define PIVOTTABLE_H_UCDSE4RV
//#include <google/sparse_hash_map> // 2 bits overhead per entry ! but quite slow
//#include <google/dense_hash_map> // very fast but memory intensive
#include <string>
#include <map>
#include "Accumulator.h"
#include "openpivotlib/Utils.h"

namespace op
{

using std::string;

class PivotTable
{
public:
  PivotTable ();
  ~PivotTable ();
  typedef std::map<string,Accumulator*> EntriesMap;
  typedef std::map<string,EntriesMap*> Container;
  Accumulator* getAccumulatorForKeyAndEntry(const string &, const string &);
  typedef std::pair<Container::const_iterator,Container::const_iterator> IterEntriesPair;
  IterEntriesPair iterEntries() const;
  void cleanup();
  
private:
  Container mDocumentMap;
};

} /* op */
#endif /* end of include guard: PIVOTTABLE_H_UCDSE4RV */
