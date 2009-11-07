#ifndef XMLCONFIGREADER_H_H4DKFCED
#define XMLCONFIGREADER_H_H4DKFCED


#include <string>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "openpivotlib/OpenPivotContext.h"
namespace op {

using std::string;

/**
 * @brief Configuration File reader
 *
 * will setup a pivot table config object
 */
class XMLConfigReader
{
public:
	XMLConfigReader();
	~XMLConfigReader();
	
	void setInputFile(const string &);
  void setContext(OpenPivotContext *);
	bool read();
	
private:
  void configureColsSettings();
  void configureRowsSettings();
  void configureDefaultAccumulation();
  

private:
	xmlDocPtr mDoc;
	xmlXPathContextPtr mXpathCtx; 
  xmlXPathObjectPtr mXpathObj;
	string mInputFile;
  OpenPivotContext *mContext;
	
};

inline void XMLConfigReader::setContext(OpenPivotContext * context)
 {
   mContext = context;
 }
}
#endif /* end of include guard: XMLCONFIGREADER_H_H4DKFCED */