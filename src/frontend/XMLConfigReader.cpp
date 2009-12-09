#include "XMLConfigReader.h"
#include <libxml/parser.h>
#include <libxml/xpathInternals.h>
#include <iostream>

namespace op {
using namespace std;
XMLConfigReader::XMLConfigReader():
  mDoc(0),
  mInputFile(),
  mContext(0)
{
}

XMLConfigReader::~XMLConfigReader() 
{
}

void XMLConfigReader::setInputFile(const string & name)
{
  mInputFile = name;
}

bool XMLConfigReader::read() 
{
  LIBXML_TEST_VERSION
  mDoc = xmlReadFile(mInputFile.c_str(),0,0);
  if (!mDoc)
  {
    std::cerr << "Failed to parse " << mInputFile << endl;
  }
  mXpathCtx = xmlXPathNewContext(mDoc);
  configureDefaultAccumulation();
  configureColsSettings();
  configureRowsSettings();
/*
* this is to debug memory for regression tests
*/
  xmlXPathFreeObject(mXpathObj);
  xmlXPathFreeContext(mXpathCtx);
  xmlMemoryDump();

  return true;
}
  

void XMLConfigReader::configureColsSettings() 
{
  mXpathObj = xmlXPathEvalExpression(BAD_CAST "/problem/columnlist/col",mXpathCtx);
  //print_xpath_nodes(mXpathObj->nodesetval,stdout);
  int size = (mXpathObj->nodesetval) ? mXpathObj->nodesetval->nodeNr : 0;
  
  for (int i = 0; i < size ; ++i)
  {
    xmlNode* currentCol = mXpathObj->nodesetval->nodeTab[i];
    if (!currentCol || currentCol->type != XML_ELEMENT_NODE)
      continue;
    //get Id of new column to create
    const char* idValue = (const char*)xmlGetProp (currentCol,BAD_CAST "id");
    aggregation_type typ = def;
    if (xmlHasProp(currentCol, BAD_CAST "accumulation"))
    {
      const char* accType = (const char*)xmlGetProp(currentCol,BAD_CAST "accumulation");
      typ = Utils::toAggType(accType);
    }
    Settings & settings = mContext->getSettings();
    settings.addColumnWithAggregTypeForKey(idValue,typ);
  }
}


void XMLConfigReader::configureRowsSettings()
{
  mXpathObj = xmlXPathEvalExpression(BAD_CAST "/problem/rowsequence/row",mXpathCtx);
  int size = (mXpathObj->nodesetval) ? mXpathObj->nodesetval->nodeNr : 0;
  
  for (int i = 0; i < size ; ++i)
  {
    xmlNode* currentRow = mXpathObj->nodesetval->nodeTab[i];
    if (!currentRow || currentRow->type != XML_ELEMENT_NODE)
      continue;
    //get Id of new column to create
    const char* idValue = (const char*)xmlGetProp (currentRow,BAD_CAST "id");

    Settings & settings = mContext->getSettings();
    settings.addRowForKey(idValue);
  }
}

void XMLConfigReader::configureDefaultAccumulation()
{
  mXpathObj = xmlXPathEvalExpression(BAD_CAST "/problem/defaultaccumulation",mXpathCtx);
  int size = (mXpathObj->nodesetval) ? mXpathObj->nodesetval->nodeNr : 0;
  if (size)
  {
    xmlNode* currentRow = mXpathObj->nodesetval->nodeTab[0];
    if (currentRow && currentRow->type == XML_ELEMENT_NODE)
    {
      Settings & settings = mContext->getSettings();
      const char* val = (const char*)xmlGetProp (currentRow,BAD_CAST "value");
      aggregation_type typ = Utils::toAggType(val);
      settings.setDefaultAccumulation(typ);
      free((void*)val);
    }
  }
}


}