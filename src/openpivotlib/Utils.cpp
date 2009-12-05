#include "Utils.h"
#include <sstream>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>


namespace op
{
  bool Utils::eqstr::operator()(const char* s1, const char* s2) const
  {
    //std::cout << "Comparing strings :" << std::endl << std::flush;
//    std::cout<< s1 << std::endl<< std::flush;
//    std::cout<< s2 << std::endl << std::flush;
    //return strcmp(s1, s2) == 0;
    //
    bool is = (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
    //std::cout <<"result : " << is << std::endl << std::flush;
    return is;
  }

  size_t Utils::HashString::operator()(const char* s) const
  {
    size_t res = hash<const char *>() (s);
    return res;
  }

  aggregation_type Utils::toAggType(const string &  str)
  {
    if (str == "average") return average;
    if (str == "sum") return sum;
    if (str == "number") return number;
    if (str == "def") return def;
    if (str == "variance") return variance;
    if (str == "stddev") return stddev;
    throw Exception("Aggregation Type not recognized");
    return def; // dummy return 
  } 
  double MyAtof(const char *, bool & );
  double MyStrtod(const char *, char **, bool & );
  
  double Utils::toDouble(const char*  str)
  {
    //std::istringstream i(str);
    bool status = true;
    
    //double res = MyAtof(str.c_str(),status);
    double res = MyAtof(str, status);
    if (!status)
    {
      string message = "Error, string " + string(str) + " is not convertible to a double";
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

  double MyStrtod(const char *str, char **endptr, bool & status)
  {
    double number;
    int exponent;
    int negative;
    char *p = (char *) str;
    double p10;
    int n;
    int num_digits;
    int num_decimals;

    // Skip leading whitespace
    while (isspace(*p)) p++;

    // Handle optional sign
    negative = 0;
    switch (*p) 
    {             
      case '-': negative = 1; // Fall through to increment position
      case '+': p++;
    }

    number = 0.;
    exponent = 0;
    num_digits = 0;
    num_decimals = 0;

    // Process string of digits
    while (isdigit(*p))
    {
      number = number * 10. + (*p - '0');
      p++;
      num_digits++;
    }

    // Process decimal part
    if (*p == '.') 
    {
      p++;

      while (isdigit(*p))
      {
        number = number * 10. + (*p - '0');
        p++;
        num_digits++;
        num_decimals++;
      }

      exponent -= num_decimals;
    }

    if (num_digits == 0)
    {
      errno = ERANGE;
      status = false;
      return 0.0;
    }

    // Correct for sign
    if (negative) number = -number;

    // Process an exponent string
    if (*p == 'e' || *p == 'E') 
    {
      // Handle optional sign
      negative = 0;
      switch(*++p) 
      {   
        case '-': negative = 1;   // Fall through to increment pos
        case '+': p++;
      }

      // Process string of digits
      n = 0;
      while (isdigit(*p)) 
      {   
        n = n * 10 + (*p - '0');
        p++;
      }

      if (negative) 
        exponent -= n;
      else
        exponent += n;
    }

    if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP)
    {
      errno = ERANGE;
      status = false;
      return HUGE_VAL;
    }

    // Scale the result
    p10 = 10.;
    n = exponent;
    if (n < 0) n = -n;
    while (n) 
    {
      if (n & 1) 
      {
        if (exponent < 0)
          number /= p10;
        else
          number *= p10;
      }
      n >>= 1;
      p10 *= p10;
    }

    if (number == HUGE_VAL) 
    {
      errno = ERANGE;
      status = false;
    }
    if (endptr) *endptr = p;

    return number;
  }

  double MyAtof(const char *str, bool & status)
  {
    return MyStrtod(str, NULL, status);
  }
}

