/******************************************************************************
*     Disassembler module                                                     *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_DISASM_HEADER
#define DCPU_DISASM_HEADER
/******************************************************************************/
#include<iostream>
#include<sstream> 
#include<cctype>
#include<string>
#include<map>
#include<list>
#include<vector>
#include"core.h"
#include"mem.h"

/******************************************************************************/
namespace DCPU_disasm{
   extern std::vector<std::string> Lines;    // source code
   const int use_base = 16;
   void load_code(DCPU::Word* mem, int size);
   void disassemble(DCPU::Memory);
   void step();
   void prepare();
}
#endif