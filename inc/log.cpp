#include "log.h"
namespace Log{


   std::list<std::string> DCPU_Log;

   void elog(string str)
   {
      DCPU_Log.push_back(str);
   }
   void elog(string str, int val)
   {
      char buff[9];
      _itoa(val, buff, 10); 
      DCPU_Log.push_back(str + buff);
   }
   void print()
   {
      int c=1;
      for(std::list<std::string>::iterator i=DCPU_Log.begin(); i!=DCPU_Log.end(); i++)
         printf("%d : %s.\n",c++,i->c_str());
   }
   std::list<std::string>::const_iterator log_begin()
   {
      return DCPU_Log.begin();
   }

};
