#include "core.h"

namespace DCPU{
   union uniWord{
   private:
      struct Fields{
         Word opcode : 5,
              dest   : 5,
              source : 6;
      };
   public:
      Fields fields;
      Word full;
   };

   union DoubleWord{
      Dword b32;
      Word b16[2];
   };

   Byte getOCode (Word w)
   {
      return w & 0x1F;        //  0000000000011111
   }
   Byte getSCode (Word w)
   {
      return (w>>10) & 0x3F;  //  1111110000000000
   }
   Byte getDCode (Word w)
   {
      return (w>>5)  & 0x1F;  //  0000001111100000
   }

   void setOCode (Word& w, Byte c)
   { 
      uniWord uw;
      uw.full = w;
      uw.fields.opcode = c;
      w = uw.full;
   }
   void setSCode (Word& w, Byte c)
   {
      uniWord uw;
      uw.full = w;
      uw.fields.source = c;
      w = uw.full;
   }
   void setDCode (Word& w, Byte c)
   {      
      uniWord uw;
      uw.full = w;
      uw.fields.dest = c;
      w = uw.full;
   }

   Word makeWord(Byte o, Byte d, Byte s)
   {
      uniWord w;

      w.fields.opcode = o;
      w.fields.dest = d;
      w.fields.source = s;
      return w.full;
   }

   Byte getHigh8(Word w)
   {
      return (w>>8) & 0xFF;
   }
   Byte getLow8(Word w)
   {
      return w & 0xFF;
   }

   Word getHigh16(Dword d)
   {
      DoubleWord t;
      t.b32=d;
      return t.b16[0];
   }
   Word getLow16(Dword d)
   {
      DoubleWord t;
      t.b32=d;
      return t.b16[1];
   }
   void setHigh16(Dword& d, Word w)
   {
      DoubleWord t;
      t.b32=d;
      t.b16[0] = w;
      d = t.b32;
   }
   void  setLow16(Dword& d, Word w)
   {
      DoubleWord t;
      t.b32=d;
      t.b16[1] = w;
      d = t.b32;
   }

}