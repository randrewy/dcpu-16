/******************************************************************************
* Hardware interface for DCPU-16 PC                                           *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_HARDWARE_LEM1802_HEADER
#define DCPU_HARDWARE_LEM1802_HEADER
/******************************************************************************/
#include "../hardware.h"
/******************************************************************************/
namespace DCPU{
   namespace LEM{
      enum{
         DISPLAY_WIDTH_PIX = 128,
         DISPLAY_HEIGHT_PIX = 96,
         DISPLAY_WIDTH_CELL = 32,
         DISPLAY_HEIGHT_CELL = 12,
         CHARACTER_WIDTH = 4,
         CHARACTER_HEIGHT = 8,
         BLINK_INTERVAL = 320,
         VRAM_ADSRESS_ASSUMPTION = 0x8000   // plenty of programs assume vram start from 0x8000. Change to 0 for standart behavior
      };
      struct Color{
         Byte  u        :4,
               r        :4,
               g        :4,
               b        :4;
         Color(Word w);
      };
      struct Cell{
         Color f,          // foreground
               b;          // background
         Word w1, w2;      // actual character
         bool blink;
         Cell(): f(0), b(0){}
         Color getPixelColor(int i, int j);
      };

      class LEM1802 : public Hardware{
      public:  
         void connect(Memory* m, Word* r, Word&, Word&, Word&);
         HWInfo info();
         Word interrupt();
      
         LEM1802();   
         Cell getCell(Word row, Word col)const;
         bool connected()const;        // is true by defaul. Vram is automaticaly set at 0x8000
         struct Screen_Mem_Error{};
      private:
         Word* reg;                    // VM registers
         Memory* mem;                  // VM Memory

         const Word* screen_ram;       // also indicates whether Monitor is connected
         const Word* font_ram;
         const Word* palette_ram;

         Color border_color;
      };

   }
}



#endif   // !DCPU_HARDWARE_LEM1802 _HEADER
