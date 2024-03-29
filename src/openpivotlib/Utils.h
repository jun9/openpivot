#ifndef UTILS_H_2K54SQXP
#define UTILS_H_2K54SQXP
#include <string>
#include <google/dense_hash_map>
#include HASH_FUN_H    // defined in config.h
#include <cstring>

namespace openpivot
{

  using SPARSEHASH_HASH;
  using std::string;
  /**
  * @brief enum representing aggregation type
  */
  enum aggregation_type {
    def = 0,
    sum,
    average,
    number,
    variance,
    stddev
  };
  
  class Utils
  {
  public:
    /**
    * @brief Method to convert string to aggregation typ
    */
    static aggregation_type toAggType(const string&) ;
    static double toDouble(const char* ) ;
    /**
    * @brief Utility struct for hash maps
    */
    struct eqstr 
    {
      bool operator()(const char* s1, const char * s2) const
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
    };
    
    /**
    * @brief Utility struct for hash maps
    */
    struct HashString
    {
      size_t operator()(const char*  ) const;
    };
    
    
    struct HashFun
    {
      size_t operator()(const char* str) const 
      {
        int val = 0;
        const char* current = &str[0];
        while (*current != '\0')
        {
          val += *(++current);
        }
        return SPARSEHASH_HASH<int>()(val);
      }
    };
    typedef HashFun Hash;
    
    /**
    *@brief comparator for maps
    */
    struct StrComp
    {
      bool operator()(const char* s1, const char* s2) const
      {
        //std::cout << "Comparing string " << s1 << " and string " << s2 << std::flush;
        bool is = strcmp(s1,s2) < 0;
        //std::cout << " --> " << is << std::endl << std::flush;
        return is;
      }
    };
  private:
    Utils ();
    ~Utils ();
    
  };
  
  class Exception
  {
  public:
    Exception (const string &);
    ~Exception ();
    const string & getMessage() const;
  private:
    string mMessage;
  };
  

  
  
  
} /* openpivot */


#endif /* end of include guard: UTILS_H_2K54SQXP */
