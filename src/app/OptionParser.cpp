/*
 *  OptionParser.cpp
 *  openpivot-Xcode
 *
 *  Created by Romain Pechayre on 12/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OptionParser.h"
#include <map>
#include <string>
#include "openpivotlib/Utils.h"

#define MAXMARGIN 50

namespace openpivot
{
  
OptionParser::OptionParser():
  mUsageString(),
  mOptionsMapping(),
  mArgumentsOptions(),
  mSimpleOptions(),
  mHelpMessage(),
  mArguments()
{
  
}

OptionParser::~OptionParser ()
{
  
}

void OptionParser::setUsageString(const string & progName)
{
  mUsageString = progName;
}

void OptionParser::addArgumentOption(char key, const string& longKey,
                                     const string & help , const string & defaultval)
{
  mOptionsMapping[longKey] = key;
  mArgumentsOptions[key] = defaultval;
  mHelpMessage[key] = help;
}

void OptionParser::addSimpleOption(char key, const string& longKey,  
                                   const string & help )
{
  mOptionsMapping[longKey] = key;
  mSimpleOptions[key] = false;
  mHelpMessage[key] = help;
}

bool OptionParser::getSimpleOptionVal(char key) const
{
  std::map<char,bool>::const_iterator theFinder = mSimpleOptions.find(key);
  if (theFinder == mSimpleOptions.end())
  {
    char theKey[1];
    theKey[0] = key;
    string message = "Option " + string(theKey) 
    + " not registered as a simple option";
  }
  return theFinder->second;
  
}

bool OptionParser::getSimpleOptionVal(const string & longKey) const
{
  return getSimpleOptionVal(toShortKey(longKey));
}

const string & OptionParser::getArgumentOptionVal(char key) const
{
  std::map<char,string>::const_iterator theFinder = mArgumentsOptions.find(key);
  if (theFinder == mArgumentsOptions.end())
  {
    char theKey[1];
    theKey[0] = key;
    string message = "Option " + string(theKey) 
    + " not registered as a simple option";
  }
  return theFinder->second;
}

const string & OptionParser::getArgumentOptionVal(const string & longKey) const
{
  return getArgumentOptionVal(toShortKey(longKey));
}

const vector<string> & OptionParser::getArguments() const
{
  return mArguments;
}

void OptionParser::parse(int argc, char * const argv[])
{
  int i = 1;
  while(i < argc)
  {
    const char* currentArg = argv[i];
    if (isOption(currentArg)) // is it of type -* ?
    {
      // if only one option in this token
      if (currentArg[1] == '-' || strlen(currentArg) == 2) 
      {
        bool isLongKey = currentArg[1] == '-';
        char key  = '\0';
        if (isLongKey) // 
        {
          key = toShortKey(&currentArg[2]);
        } else {
          key = currentArg[1];
        }
        // now check if option is simple/argument
        if (isSimpleOption(key))
        {
          activateSimpleOption(key);
        } else 
        {
          // if key is last token or next token is option
          if (i + 1 >= argc || isOption(argv[i + 1])) 
          {
            char theKey [1];
            theKey[0] = key;
            string message = "Argument of option " + string(theKey) 
            +" missing";
            throw Exception (message);
          }
          // now we are sure that option exists and that argument is present
          i++;
          string myValue = string(argv[i]);
          mArgumentsOptions[key] = myValue;
        }
        
        
      } else //handle list of simple options in this list
      {
        for (size_t j = 1; j < strlen(currentArg); j++)
        {
          activateSimpleOption(currentArg[j]);
        }
      }
      //if first char is not '-' then token is simple argument
    } else
    {
      mArguments.push_back(currentArg);
    }
    i++;
  }
}

bool OptionParser::isOption(const char* field) const
{
  bool flag = false;
  if (strlen(field) > 1)
  {
    if (field[0] == '-')
      flag = true;
    if (strlen(field) < 2)
    {
      string message = "malformed option :" + string(field);
      throw Exception (field);
    }
  }
  return flag;
}

void OptionParser::activateSimpleOption(char key)
{
  std::map<char,bool>::iterator finder = mSimpleOptions.find(key);
  if (finder == mSimpleOptions.end())
  {
    char theKey [1];
    theKey[0] = key;
    string message = "Option " + string(theKey) +" not known";
    throw Exception (message);
  }
  finder->second = true;
}

char OptionParser::toShortKey(const string & longKey) const
{
  std::map<string,char>::const_iterator finder = mOptionsMapping.find(longKey);
  if (finder == mOptionsMapping.end())
  {
    string message = "Options " + string(longKey) + " not known";
    throw Exception (message);
  }
  return finder->second;
}

bool OptionParser::isSimpleOption(char key) const
{
  std::map<char,bool>::const_iterator finder = mSimpleOptions.find(key);
  if (finder != mSimpleOptions.end())
    return true;
  
  std::map<char,string>::const_iterator theFinder = mArgumentsOptions.find(key);
  if (theFinder != mArgumentsOptions.end())
    return false;
  
  char theKey [1];
  theKey[0] = key;
  string message = "Option " + string(theKey) +" not known";
  throw Exception (message);
  return false;
}

void OptionParser::usage()
{
  std::map<char,string> reverseMap;
  std::map<string,char>::const_iterator iter = mOptionsMapping.begin();
  for (; iter != mOptionsMapping.end(); ++iter) 
  {
    reverseMap[iter->second] = iter->first;
  }
  std::cout << "Usage: " << mUsageString << std::endl;
  std::cout << "Options:" << std::endl;
  string mess = "    -h, --help";//       show this help message and exit";
  size_t myLength = mess.length();
  size_t myMax = (MAXMARGIN - myLength) > 0 ? MAXMARGIN - myLength: 0;
  for (size_t thIndex = 0 ; thIndex < myMax; thIndex++)
    mess.push_back(' ');
  mess += "show this help message and exit";
  std::cout <<  mess << std::endl;;
  for (std::map<char,bool>::const_iterator iter2 = mSimpleOptions.begin(); iter2!= mSimpleOptions.end(); ++ iter2)
  {
    char theKey[1];
    theKey[0] = iter2->first;
    string beginmessage  = "    -" + string(theKey) + ",  --" + reverseMap[iter2->first];
    size_t theLength = beginmessage.length();
    size_t max = (MAXMARGIN - (int)theLength) > 0 ? MAXMARGIN - theLength: 0;
    for (size_t i = 0; i < max;i++)
    {
      beginmessage.push_back(' ');
    }
    std::cout << beginmessage << mHelpMessage[iter2->first] << std::endl; 
  }
  
  for (std::map<char,string>::iterator iterHelps = mArgumentsOptions.begin(); iterHelps!= mArgumentsOptions.end() ;++ iterHelps)
  {
    char theKey[1];
    theKey[0] = iterHelps->first;
    string beginmessage  = "    -" + string(theKey) + " VALUE,  --" + reverseMap[iterHelps->first] + "=VALUE";
    size_t theLength = beginmessage.length();
    size_t max = (MAXMARGIN - (int)theLength) > 0 ? MAXMARGIN - theLength: 0;
    for (size_t i = 0; i < max;i++)
    {
      beginmessage.push_back(' ');
    }
    std::cout << beginmessage << mHelpMessage[iterHelps->first] << std::endl; 
  }
  exit(1);
}

  
} /* openpivot */