/******************************************************************************
* DCPU simple error log (forawhile)                                           *
*                                                                             *
*******************************************************************************/
#include <string>
#include <list>
#include <stdlib.h>
#include <cstdio>
using namespace std;
/******************************************************************************/
namespace Log{
   void elog(string str);
   void elog(string str, int val);
   void print();
   std::list<std::string>::const_iterator log_begin();
};
