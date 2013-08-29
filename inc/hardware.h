/******************************************************************************
* Hardware interface for DCPU-16 PC                                           *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_HARDWARE_INTERFACE_HEADER
#define DCPU_HARDWARE_INTERFACE_HEADER
/******************************************************************************/
#include "core.h"
#include "mem.h"
/******************************************************************************/
namespace DCPU{


   struct HWInfo{
      Dword HardwareID;
      Word  HardwareVersion;
      Dword ManufacturerID;
      HWInfo(Dword HID, Word Ver, Dword MID) : HardwareID(HID), HardwareVersion(Ver), ManufacturerID(MID){}
   };

   class Hardware{
   public:
      virtual HWInfo info() =0; 

      virtual Word interrupt() =0;
      virtual void connect(Memory* m, Word* r, Word& PC, Word& EX, Word& IA) =0;
   };



}



#endif   // !DCPU_HARDWARE_INTERFACE_HEADER