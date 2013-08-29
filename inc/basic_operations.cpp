#include "basic_operations.h"

namespace DCPU{

   Byte Set(Word& arg, Word val,Word&   )
   {
      arg = val;
      return 0;
   }
   Byte Add(Word& arg, Word val,Word& EX)
   {
      SDword result = arg + val;
      arg = result & 0xFFFF; 
      EX = 0;
      if(result > WORD_MAX) EX = 1;
      return 1;
   }
   Byte Sub(Word& arg, Word val,Word& EX)
   {
      EX = 0;
      arg= arg - val;
      if(val > arg) EX = WORD_MAX;     // Underflow
      return 1;
   }
   Byte Mul(Word& arg, Word val,Word& EX)
   {
      Dword result = arg * val;       // Extra space to store result
      EX = result >> 16;              // EX = 16 upper bits 
      arg= result & 0xFFFF; 
      return 1;
   }
   Byte Mli(Word& arg, Word val,Word& EX)
   {
      SDword result = Signed(val) * Signed(arg);
      EX = result >> 16;
      arg= result & 0xFFFF;
      return 1;
   }
   Byte Div(Word& arg, Word val,Word& EX)
   {
      if(val == 0){                    // zero divide: 'silent fail'
         arg = 0;       
         EX = 0;
      }
      else{
         EX =((arg<<16)/val)&0xffff;   // 
         arg = arg/val  ;              // rounds towards zero. 
      }
      return 1;
   }
   Byte Dvi(Word& arg, Word val,Word& EX)
   {
      if(val == 0){
         arg = 0;
         EX = 0;
      }
      else{
         EX =((Signed(arg)<<16)/val)&0xffff;   // 
         arg = Signed(arg)/val  ;              // rounds towards zero. 
      }
      return 1;
   }
   Byte Mod(Word& arg, Word val,Word&   )
   {
      if(val == 0)
         arg = 0;
      else
         arg = arg % val;
      return 0;
   }
   Byte Mdi(Word& arg, Word val,Word&   )
   {
      if(val == 0)
         arg = 0;
      else
         arg = Signed(arg) % Signed(val);
      return 0;
   }
   Byte And(Word& arg, Word val,Word&   )
   {
      arg = arg & val;
      return 0;
   }
   Byte Bor(Word& arg, Word val,Word&   )
   {
      arg = arg | val;
      return 0;
   }
   Byte Xor(Word& arg, Word val,Word&   )
   {
      arg = arg ^ val;
      return 0;
   }
   Byte Shr(Word& arg, Word val,Word& EX)
   {
      Dword res = arg << 16;
      EX = (res >> val) & 0xFFFF;    //Lower 16 bit (the bits shifted out)
      arg  = arg >> val;
      return 1;
   }
   Byte Asr(Word& arg, Word val,Word& EX)
   {
      SDword res = arg << 16;
      EX = (res >> val) & 0xFFFF;     //Lower 16 bit
      arg  = Signed(arg) >> val;      // Arithmetic shift!
      return 1;
   }
   Byte Shl(Word& arg, Word val,Word& EX)
   {
      SDword res = arg;
      res <<= val;
      EX = (res & 0xFFFF0000) >> 16;   //Upper 16 bit
      arg  =  res & 0x0000FFFF;
      return 1;
   }
   Byte Ifb(Word& arg1, Word arg2, Word&)
   {
      return (arg1 & arg2) != 0;
   }
   Byte Ifc(Word& arg1, Word arg2, Word&)
   {
      return !(arg1 & arg2);
   }
   Byte Ife(Word& arg1, Word arg2, Word&)
   {
      return arg1 == arg2;
   }
   Byte Ifn(Word& arg1, Word arg2, Word&)
   {
      return arg1 != arg2;
   }
   Byte Ifg(Word& arg1, Word arg2, Word&)
   {
      return arg1 > arg2;
   }
   Byte Ifa(Word& arg1, Word arg2, Word&)
   {
      return Signed(arg1) > Signed(arg2);
   }
   Byte Ifl(Word& arg1, Word arg2, Word&)
   {
      return arg1 < arg2;
   }
   Byte Ifu(Word& arg1, Word arg2, Word&)
   {
      return Signed(arg1) < Signed(arg2);
   }
   Byte Adx(Word& arg, Word val, Word& EX)
   {
      SDword result = arg + val + EX;
      arg  = result & 0xFFFF; 
      EX = 0;
      if(result > WORD_MAX) EX = 1;
      return 1;
   }
   Byte Sbx(Word& arg, Word val, Word& EX)
   {
      EX = 0;
      if(val > arg + EX) EX = WORD_MAX;         //underflow
      if(arg + EX - val > WORD_MAX) EX = 0x1;   //overflow
      arg = arg + EX - val;
      return 1;
   } 

}


