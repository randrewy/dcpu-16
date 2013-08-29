/******************************************************************************
*     Basic DCPU operations                                                   *
* Return value is either true/false for boolean operations or indicates       *
* whether EX was changed (1) or not (0).                                      *
*                                                                             *
* STI and STD operations are not defind here                                  *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_INSTRUCTION_SET_HEADER
#define DCPU_INSTRUCTION_SET_HEADER
/******************************************************************************/
#include "core.h"
/******************************************************************************/
namespace DCPU{
   Byte Set(Word& arg, Word val, Word&   );     //SET - Sets a value    
   Byte Add(Word& arg, Word val, Word& EX);     //ADD - Adds values
   Byte Sub(Word& arg, Word val, Word& EX);     //SUB - Subtracts values
   Byte Mul(Word& arg, Word val, Word& EX);     //MUL - Multiplies unsigned values
   Byte Mli(Word& arg, Word val, Word& EX);     //MLI - Multiplies signed values
   Byte Div(Word& arg, Word val, Word& EX);     //DIV - Divides unsigned values
   Byte Dvi(Word& arg, Word val, Word& EX);     //DVI - Divides signed values
   Byte Mod(Word& arg, Word val, Word&   );     //MOD - Finds modulo (remainder) of unsigned values
   Byte Mdi(Word& arg, Word val, Word&   );     //MDI - Finds modulo (remainder) of signed values
   Byte And(Word& arg, Word val, Word&   );     //AND - Performs a bitwise AND operation on values
   Byte Bor(Word& arg, Word val, Word&   );     //BOR - Performs a bitwise OR operation on values
   Byte Xor(Word& arg, Word val, Word&   );     //XOR - Performs a bitwise XOR operation on values
   Byte Shr(Word& arg, Word val, Word& EX);     //SHR - Performs a right bit shift on unsigned values
   Byte Asr(Word& arg, Word val, Word& EX);     //ASR - Performs a right bit shift on signed values
   Byte Shl(Word& arg, Word val, Word& EX);     //SHL - Performs a left bit shift on values
                                               
   Byte Ifb(Word& arg1, Word arg2, Word& );     //IFB - Performs a bitwise AND operation and checks if nonzero. If check fails, ignores the next instruction
   Byte Ifc(Word& arg1, Word arg2, Word& );     //IFC - Performs a bitwise AND operation and checks if zero. If check fails, ignores the next instruction
   Byte Ife(Word& arg1, Word arg2, Word& );     //IFE - Checks if two values are equal to each other. If check fails, ignores the next instruction
   Byte Ifn(Word& arg1, Word arg2, Word& );     //IFN - Checks if two values are not equal to each other. If check fails, ignores the next instruction
   Byte Ifg(Word& arg1, Word arg2, Word& );     //IFG - Checks if an unsigned value is greater than another unsigned value. If check fails, ignores the next instruction
   Byte Ifa(Word& arg1, Word arg2, Word& );     //IFA - Checks if a signed value is greater than another signed value. If check fails, ignores the next instruction
   Byte Ifl(Word& arg1, Word arg2, Word& );     //IFL - Checks if an unsigned value is less than another unsigned value. If check fails, ignores the next instruction
   Byte Ifu(Word& arg1, Word arg2, Word& );     //IFU - Checks if a signed value is less than another signed value. If check fails, ignores the next instruction
                                                
   Byte Adx(Word& arg, Word val, Word& EX);     //ADX - Takes the sum of arg, val and EX 
   Byte Sbx(Word& arg, Word val, Word& EX);     //SBX - Subtracts the arguments, then adds EX

}
#endif // !DCPU_INSTRUCTION_SET_HEADER