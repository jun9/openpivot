#include "PivotTable.h" 
namespace openpivot 
{
  PivotTable::PivotTable ()
  {
#ifndef NO_GOOGLE_HASH
    mDocumentMap.set_empty_key(0);
#endif
  }

  PivotTable::~PivotTable ()
  {

  }

  Accumulator* PivotTable::getAccumulatorForKeyAndEntry(const char* key, const char* entryKey)
  {
    //std::cout << "Key " << key << std::endl;
    Container::const_iterator finder = mDocumentMap.find(key);
    EntriesMap* entries = 0;
    if (finder == mDocumentMap.end())
    {
      entries = new EntriesMap();
#ifndef NO_GOOGLE_HASH
      entries->set_empty_key(0);
#endif
      size_t theSize = strlen(key);
      char* newKey = new char[theSize];
      newKey[0] = '\0';
      strcpy(newKey, key);
      mDocumentMap[newKey] = entries;
    } else {
      entries = finder->second;
    }
      
    EntriesMap::const_iterator finderEntry = entries->find(entryKey);
    Accumulator* result = 0;
    if (finderEntry == entries->end())
    {
      result = new Accumulator();
      entries->insert(std::make_pair(entryKey,result));
    } else  
        result = finderEntry->second;
    return result;
  }

  void PivotTable::cleanup()
  {
    EntriesMap* entries = 0;
    for (Container::iterator iter = mDocumentMap.begin(); iter != mDocumentMap.end(); ++iter)
    {
      entries = iter->second;
      for (EntriesMap::iterator theIterEntries = entries->begin(); theIterEntries != entries->end(); ++theIterEntries)
      {
        if (theIterEntries->second)
        {
          delete theIterEntries->second;
          theIterEntries->second = 0;
        }
      }
      delete entries;
      delete[] iter->first;
      iter->second = 0;
    }
  }
  
  PivotTable::IterEntriesPair PivotTable::iterEntries() const
  {
    return std::make_pair(mDocumentMap.begin(),mDocumentMap.end());
  }
  
  } /* openpivot */