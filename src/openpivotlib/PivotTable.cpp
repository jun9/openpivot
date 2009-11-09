#include "PivotTable.h" 
namespace op
{
  PivotTable::PivotTable ()
  {
    mDocumentMap.set_empty_key(string());
  }

  PivotTable::~PivotTable ()
  {

  }

  Accumulator* PivotTable::getAccumulatorForKeyAndEntry(const string & key, const string & entryKey)
  {
    //std::cout << "Key " << key << std::endl;
    Container::const_iterator finder = mDocumentMap.find(key);
    EntriesMap* entries = 0;
    if (finder == mDocumentMap.end())
    {
      entries = new EntriesMap();
      entries->set_empty_key(string());
      mDocumentMap[key] = entries;
    } else
      entries = finder->second;
    EntriesMap::const_iterator finderEntry = entries->find(entryKey.c_str());
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
      for (EntriesMap::iterator iterEntries = entries->begin(); iterEntries != entries->end(); ++iterEntries)
      {
        if (iterEntries->second)
        {
          delete iterEntries->second;
          iterEntries->second = 0;
        }
      }
      delete entries;
      iter->second = 0;
    }
  }
  
  PivotTable::IterEntriesPair PivotTable::iterEntries() const
  {
    return std::make_pair(mDocumentMap.begin(),mDocumentMap.end());
  }
  
  } /* op */