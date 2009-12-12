/*
 *  CsvReader.h
 *  openpivot
 *
 *  Created by Romain Pechayre on 9/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CSVREADER_H_LBXGKD1B
#define CSVREADER_H_LBXGKD1B
#include <vector>
//#include <string>

#include "openpivotlib/OpenPivotContext.h"

#ifdef NO_GOOGLE_HASH
#include <map>
#else
#include <google/dense_hash_map>
#endif

namespace openpivot
{
using std::vector;
  //using std::string;
using std::map;
/**
* @brief Main input file reader.
*/
class CsvReader
{
public:
  /**
  * @brief default constructor
  */
  CsvReader ();
  
  /**
  * @brief destructor
  */
  ~CsvReader ();
  
  /**
  *@brief To set the input
  */
  void setInput(const char* );
  
  /**
  *@brief To set the context
  */
  void setContext(OpenPivotContext *);
  /**
  * @brief main method : performs reading of the input 
  * file and building of pivot table
  */
  bool read();
  
  /**
   *@brief clean allocated memory
   */
  void clean();
  
private:
  
  /**
  * @brief Called on each line as preprocessing to split the line using 
  * custum separator
  */
  void tokenizeBuffer();
  
  /**
  * @brief Called on each line of the file to process information based on what
  * is required n Settings instance
  */
  bool processLine(); 
  
  /**
  * @brief called upon redaing first line (required) : 
  * prepare indices for pivot table from headers
  */
  bool processHeader(); //One header line required here
  
  /**
  * @brief called at each line (except headers), build keys
  * based on rows required in Settings instance
  */
  const char* buildKey();
  
  /**
  * @brief check if headers require in settings are here
  * 
  * Must be called while processing Headers only
  */
  void checkHeaders();
  
  /**
  * @brief to get particular field in current Buffer according to 
  * separator
  */
  const char* getFromTokens(int) const;
  
  /**
  * @brief convenience TypeDef
  */
  //typedef dense_hash_map<const char*,int,Utils::Hash,Utils::eqstr> ReverseHeadersMap;
#ifdef NO_GOOGLE_HASH
  typedef std::map<const char*,int,Utils::StrComp>  ReverseHeadersMap;
#else
  typedef google::dense_hash_map<const char*,int,Utils::Hash, Utils::eqstr > ReverseHeadersMap;
#endif

  
private:
  char mSeparator;
  int mNbTokens;
  char *mCurrentBuffer;
  const char* mFileName;
  OpenPivotContext *mContext;
  vector<int> mPositions;
  vector<int> mRowsIndices;
  ReverseHeadersMap mHeadersMap;

  
};
} /* openpivot */
#endif /* end of include guard: CSVREADER_H_LBXGKD1B */
