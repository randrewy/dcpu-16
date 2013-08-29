/******************************************************************************
* Memory:                                                                     *
*  16 bit words                                                               *
*  0x10000 words of ram                                                       *
*                                                                             *
*                                                                             *
* Stack:                                                                      *
*  In the DCPU-16, the stack is located at the end of the memory space,       *
*  and grows backward. The first item to go on the stack is saved at          *
*  location 0xffff. The Stack Pointer (SP) special purpose register points    *
*  to the top of the stack.                                                   *
*  Main Operations:                                                           *
*    PUSH(value)  - Adds a value to the top of the stack                      *
*    POP()        - Reads, then removes the value from the top of the stack   *
*    PEEK()       - Reads the value on the top of the stack                   *
*    PICK(offset) - Reads a value from inside the stack                       *
*                                            (http://0x10cwiki.com/wiki/Stack)*
*  Over/Underflow behavior is undefind.                                       *
*  The realisation ensures no crashes in any case. The program data can be    *
*  overriden though.                                                          *
*  popping from empty stack always returns value at the end of the memory     *
*                                                                             *
*                                                                             *
* IntQueue:                                                                   *
*  A simple FIFO collection for interrupts. Max length is 256 interrupts.     *
*  Attempting to queue more than 256 interrupts will cause the DCPU to catch  *
*  fire. In this case an exeption is generated.                               *
*  Implemented as a cyclic array where next index is given by next(curr)      *
*                                                                             *
*  check_queue(ic)  checks whether:                                           *
*     1) queueing is disabled,                                                *
*     2) interrupt queue is not empty,                                        *
*     3) Last task was not an interrupt. (1 instruction gap)                  *
*  => Inerrupt from queue must be triggered                                   *
*                                                                             *
*******************************************************************************/
#ifndef DCPU_MEMORY_STACK_HEADER
#define DCPU_MEMORY_STACK_HEADER
/******************************************************************************/
#include "core.h"
#include "log.h"
/******************************************************************************/

namespace DCPU{
   
   class Memory{
    public:
      Word& at(Word addr);                      // returns word at address
      static const SDword MEM_SIZE = 0x10000;
      static const SDword MEM_END  = MEM_SIZE-1;

    private:
      Word words[MEM_SIZE];
   };

   class Stack{
    public:
      Stack(Memory* assoc_mem);
      Word& sp();

      void  push(Word w);                       // simple push
      Word& delayed_push();                     // push 0, then returns reference to the "top" of the stack <--- trick used in vm.cpp
      Word  pop ();                             // Reads the value from the top of the stack, SP is incremented.
      Word& peek();                             // Reads the value on the top of the stack. Doesnt change SP
      Word& pick(Word offset);                  // Reads a value from inside the stack. Doesnt change SP

   private:                                 
      Word SP;                                  // pointer to the top of the stack ('address' actualy)
      Memory* am;                               // the memory stack is located in
      Stack(){}

   };




   class IntQueue{
    public:
      struct QueuePair{ Word a, ia;};
      IntQueue();

      Word size();  
      void push(QueuePair p);                   // push into queue
      void push(Word a, Word ia);
      QueuePair pop();                          // pop                            
      void dropp();                             // dropps all interrupts 
      bool check_queue(Dword ic);               // checks for pending interrupts
      void set_last(Dword ic);                  // last instruction counter for interrupt 

      void enable();
      void disable();
      bool is_enabled();

    private:
      Byte next(Byte pos);                       // get index of element next to elem with index pos
      Byte lo,                                  // the firs element (first to enter queue)
           hi;                                  // the last one
      Word sz;                                  // actual size;
      Dword lsi;                                // last interrupt instruction;
      bool enabled;
      static const int MAX_SIZE = 256;
      QueuePair ints[MAX_SIZE];
      class CaughtFire{};                       // OverFlow
      class QueueUnderFlow{};                   // UnderFlow
   };

      


}
#endif // !DCPU_MEMORY_STACK_HEADER