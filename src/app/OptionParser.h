#ifndef OPTIONPARSER_H_WJQEQFD0
#define OPTIONPARSER_H_WJQEQFD0
#include <map>
#include <string>
#include <vector>

namespace openpivot
{
using std::vector;
using std::string;

class OptionParser
{
public:
  OptionParser ();
  ~OptionParser ();
  void addArgumentOption(char key, const string & longKey, const string &  help = std::string(), const string & defaultval = string());
  void addSimpleOption(char key, const string & longKey,  const string &  help = std::string());
  void setUsageString(const string &);
  
  bool getSimpleOptionVal(char) const;
  bool getSimpleOptionVal(const string &) const;
  
  const string & getArgumentOptionVal(char) const;
  const string & getArgumentOptionVal(const string &) const;
  const vector<string> & getArguments() const;
  
  void parse(int argc, char * const argv[]);
  void usage();
  
private:
  bool isOption(const char*) const;
  void activateSimpleOption(char); // sets flag associated to simple option ot true
  char toShortKey(const string &) const;
  bool isSimpleOption(char) const;
  
private:
  string mUsageString;
  std::map<string,char> mOptionsMapping;
  std::map<char,string> mArgumentsOptions;
  std::map<char,bool> mSimpleOptions;
  std::map<char,string> mHelpMessage;
  vector<string> mArguments; 
  
};
} /* openpivot */

#endif /* end of include guard: OPTIONPARSER_H_WJQEQFD0 */

