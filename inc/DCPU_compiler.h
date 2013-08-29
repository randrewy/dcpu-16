/******************************************************************************
*     Compiler Module                                                         *
*                                                                             *
* Lexer (getLexem in particular) assumes that every line of code ends with    *
* the '\n' symbol or at least consists of it alone                            *
*                                                                             *
* Compiler doesn't support small-literal labels, so they always need extra    *
* word                                                                        *
* need_remap field is added fo future improvements: checks whether next word  *
* can be changed for small literal or not                                     *
*******************************************************************************/
#ifndef DCPU_COMPILER_HEADER
#define DCPU_COMPILER_HEADER
/******************************************************************************/
#include<iostream>
#include<sstream> 
#include<cctype>
#include<string>
#include<map>
#include<list>
#include<vector>
#include"core.h"
#include"lexer.h"

/******************************************************************************/
namespace DCPU_C{
   typedef DCPU::Word Word;
   using namespace Lexer;

   extern std::vector<std::string> Lines;

   extern void compile();

   struct Label_call{
      struct Pair{
         Word instruction;
         Word word_number;
      };
      std::list<Pair> calls;  // addresses where label is used as one of arguments
   };


   struct Ltbl_Entry{
      int        instruction_number;  // instruction where defined
      int        address;             // address label points to
      Label_call calls;
   };
   
   extern std::map<std::string, Ltbl_Entry> Label_tbl;

}
#endif