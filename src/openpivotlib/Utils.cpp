#include "Utils.h"
#include <sstream>



namespace op
{
  bool Utils::eqstr::operator()(const char* s1, const char* s2) const
  {
    std::cout << "Comparing strings :" << std::endl << std::flush;
    std::cout<< s1 << std::endl<< std::flush;
    std::cout<< s2 << std::endl << std::flush;
    bool is = (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
    std::cout <<"result : " << is << std::endl << std::flush;
    return is;
  }

  size_t Utils::SpecialHash::operator()(const char* s) const
  {
    return hash<const char *>() (s);
  }

  aggregation_type Utils::toAggType(const string & str)
  {
    std::cerr << "agg type " << str <<std::endl;
    if (str == "average") return average;
    if (str == "sum") return sum;
    if (str == "number") return number;
    if (str == "def") return def;
    if (str == "variance") return variance;
    if (str == "stddev") return stddev;
    throw Exception("Aggregation Type not recognized");
    return def; // dummy return 
  } 
  
  double Utils::toDouble(const string & str)
  {
    std::istringstream i(str);
    double res ;
    if (!(i >> res))
    {
      string message = "Error, string " + str + " is not convertibe to a double";
      throw message;
    }
    return res;

  } 
  
  Exception::Exception (const string & mess):
    mMessage(mess)
  {
    
  }
  
  Exception::~Exception ()
  {
    
  }
  
  const string & Exception::getMessage() const
  {
    return mMessage;
  }
} /* op */


