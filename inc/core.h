/******************************************************************************
* Types used                                                                  *
* Constants                                                                   *
* Opcode and argument enums                                                   *
* Basic low-level functions                                                   *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_CORE_TYPES_HEADER
#define DCPU_CORE_TYPES_HEADER
/******************************************************************************/

namespace DCPU{

   typedef unsigned char            Byte;   // 8  bits
   typedef unsigned short           Word;   // 16 bits
   typedef   signed short         Signed;   // 16 bits 
   typedef unsigned int            Dword;   // 32 bits 
   typedef   signed int           SDword;   // 32 bits 
   typedef          float          Float;   

   typedef Byte (*Basic_Inst)(Word&, Word, Word&);
/******************************************************************************/
   const          int     BYTE_MAX = 0xFF;
   const          int     BYTE_MIN = 0x0;
   const          int     WORD_MAX = 0xFFFF;
   const          int     WORD_MIN = 0x0;
   const          int   SIGNED_MAX = 0x7FFF;
   const          int   SIGNED_MIN = -0x8000;
   const unsigned int    DWORD_MAX = 0xFFFFFFFF;
   const          int    DWORD_MIN = 0;
   const          int   SDWORD_MAX = 0x7FFFFFFF;
   const   signed int   SDWORD_MIN = -static_cast<int>(0x80000000);  // just rly annoying 

   const          int SMALL_LITERAL_OFFSET = 0x21;                         // zero offset for literal (small) values
   const          int SMALL_LITERAL_SHIFT  = 0x20;                         // small literals begins from this value
   const          int SMALL_LITERAL_MAX    = 0x3f - SMALL_LITERAL_OFFSET;  // Max for small_literal
   const          int SMALL_LITERAL_MIN    = SMALL_LITERAL_SHIFT - SMALL_LITERAL_OFFSET;
    
/******************************************************************************/

   namespace Codes{
      enum BasicOpcode{
            SPH = 0x00,        SET = 0x01,        ADD = 0x02,        SUB = 0x03,  
            MUL = 0x04,        MLI = 0x05,        DIV = 0x06,        DVI = 0x07,   
            MOD = 0x08,        MDI = 0x09,        AND = 0x0A,        BOR = 0x0B,   
            XOR = 0x0C,        SHR = 0x0D,        ASR = 0x0E,        SHL = 0x0F,   
            IFB = 0x10,        IFC = 0x11,        IFE = 0x12,        IFN = 0x13,   
            IFG = 0x14,        IFA = 0x15,        IFL = 0x16,        IFU = 0x17,   
          O__24 = 0x18,      O__25 = 0x19,        ADX = 0x1A,        SBX = 0x1B,
          O__28 = 0x1C,      O__29 = 0x1D,        STI = 0x1E,        STD = 0x1F   
      };                                                      
                                                           
      enum SpecialOpcode{                                           
         _S__00 = 0x00,     _S_JSR = 0x01,     _S__02 = 0x02,     _S__03 = 0x03,  
         _S__04 = 0x04,     _S__05 = 0x05,     _S__06 = 0x06,     _S__07 = 0x07,   
         _S_INT = 0x08,     _S_IAG = 0x09,     _S_IAS = 0x0A,     _S_RFI = 0x0B,   
         _S_IAQ = 0x0C,     _S__13 = 0x0D,     _S__14 = 0x0E,     _S__15 = 0x0F,   
         _S_HWN = 0x10,     _S_HWQ = 0x11,     _S_HWI = 0x12,     _S__19 = 0x13,   
         _S__20 = 0x14,     _S__21 = 0x15,     _S__22 = 0x16,     _S__23 = 0x17,   
         _S__24 = 0x18,     _S__25 = 0x19,     _S__26 = 0x1A,     _S__27 = 0x1B,
         _S__28 = 0x1C,     _S__29 = 0x1D,     _S__30 = 0x1E,     _S__31 = 0x1F  
      };                                                      
                                                           
      enum Argument{                                               
              A = 0x00,          B = 0x01,          C = 0x02,          X = 0x03,  
              Y = 0x04,          Z = 0x05,          I = 0x06,          J = 0x07,   
             aA = 0x08,         aB = 0x09,         aC = 0x0A,         aX = 0x0B,   
             aY = 0x0C,         aZ = 0x0D,         aI = 0x0E,         aJ = 0x0F,   
           aAnw = 0x10,       aBnw = 0x11,       aCnw = 0x12,       aXnw = 0x13,   
           aYnw = 0x14,       aZnw = 0x15,       aInw = 0x16,       aJnw = 0x17,   
        PUSHPOP = 0x18,       PEEK = 0x19,       PICK = 0x1A,         SP = 0x1B,
             PC = 0x1C,         EX = 0x1D,     aNEXTW = 0x1E,      NEXTW = 0x1F
      };
   }
    
   
   Byte getOCode (Word);  // get operation code
   Byte getSCode (Word);  // get source code
   Byte getDCode (Word);  // get destination code

   void setOCode (Word&, Byte); // set operation code
   void setSCode (Word&, Byte); // set source code
   void setDCode (Word&, Byte); // set destination code
                     
   Word makeWord(Byte o, Byte d, Byte s);   

   Byte getHigh8(Word);
   Byte getLow8(Word);
   
   Word getHigh16(Dword);
   Word getLow16(Dword);

   void setHigh16(Dword&, Word);
   void setLow16(Dword&, Word);


}


#endif // !DCPU_CORE_TYPES_HEADER