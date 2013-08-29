/******************************************************************************
* Lexical analyzer for DCPU language                                          *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_LEXER_HEADER
#define DCPU_LEXER_HEADER
/******************************************************************************/
#include<cctype>
#include<string>
#include<map>
#include<iostream>
#include"core.h"
/******************************************************************************/
namespace Lexer{ 

   enum Token{
      COMMENT=0, LABEL_DEF, LABEL, ENDLINE, BASICOPCD, SPECOPCD, REG_OR_STACK,
      COMMA, LB, RB, PLUS, NUMBER, UNDEFIND_WORD, UNDEFIND_SYMBOL, NONE
   };

   extern std::map<std::string, DCPU::Codes::BasicOpcode> BasicI;
   extern std::map<std::string, DCPU::Codes::SpecialOpcode> SpecI;
   extern std::map<std::string, short> Values;
   extern std::map<std::string, short> Labels;

   struct Lexem{
      Token type;
      std::string str_val;
      DCPU::Word num_val;

      Lexem(Token t) : type(t){}
      Lexem(Token t, std::string s) : type(t), str_val(s){}
      Lexem(Token t, DCPU::Word  n) : type(t), num_val(n){}
   private:
      Lexem();
   };
   extern Lexem getLexem(std::istream *input);

}


#endif //!DCPU_LEXER_HEADER
