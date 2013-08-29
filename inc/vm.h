/******************************************************************************
*  DCPU-16 Virtual Machine Class                                              *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_VIRTUAL_MACHINE_HEADER
#define DCPU_VIRTUAL_MACHINE_HEADER
/******************************************************************************/
#include <map>
#include <vector>
#include "core.h"
#include "mem.h"
#include "Hardware.h"
#include "basic_operations.h"
#include "log.h"
/******************************************************************************/
namespace DCPU{
    
   class DCPUVirtualMachine{     
    public:
      DCPUVirtualMachine();
      void load(Word* mem, Word size);
      void connect(Hardware* h);
      Word tick();

      std::string dumpstate();
      std::vector<Word> dumpMemory(Word sz, Word from = 0);
      int totalCycles()const;
      int totalInstructions()const;
            
    private:
      enum{ REG_SIZE = 8, DEVICE_MAX = 65536};             
      Memory RAM;
      Stack  stack;
      Word   Reg[REG_SIZE];         // A B C X Y Z I J
      Word   PC, EX, IA;            // Additional registers

      IntQueue IQ;         
     
      Word connected_devices;
      Hardware *Devices[DEVICE_MAX];
     
      Word  Silent;                 // for silent fails   
      Dword total_cycles;           // total cycles from VM start
      Dword instr_count;            // total instructions from VM start
      
      Word& next_word();            // returns next word ([PC++])
      void  skip();                 // skips conditionals chains

      Word  getSource(Word code);   // gets Value of second(source) argumend
      Word& getDest  (Word code);   // gets destination (first argument)

         /* Basic operations, change registers I,J*/
      Byte Sti(Word& arg, Word val);
      Byte Std(Word& arg, Word val);

         /* Special operations*/;
      void Jsr(Word  val);
      void Int(Word  val);
      void Iag(Word& arg);
      void Ias(Word  val);
      void Rfi(Word );
      void Iaq(Word  val);
      void Hwn(Word& arg);
      void Hwq(Word  val);
      void Hwi(Word  val);
};


}

#endif // !DCPU_VIRTUAL_MACHINE_HEADER
