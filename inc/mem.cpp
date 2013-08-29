#include "mem.h"

namespace DCPU{
   Word& Memory::at(Word addr)
   {
      if(0<=addr && addr<MEM_SIZE)      // safe playing
         return words[addr];
      throw;
   }

   Stack::Stack(Memory* m)
   {
      am = m;       
      SP = 0;                          // stack grows backwards and starts at 0
   }
   Word& Stack::sp()
   {
      return SP;
   }

   void Stack::push(Word w)
   {
      am->at(--SP) = w;
   }
   Word& Stack::delayed_push()
   {
      push(0);
      return am->at(SP);
   }
   Word Stack::pop()
   {
      if(SP)
         return am->at(SP++);
      Log::elog("popped from empty stack");      
      return 0;
   }
   Word& Stack::peek()
   {
      if(SP)
         return am->at(SP);
      Log::elog("peeking from empty stack;");
      return am->at(Memory::MEM_END);                        // TODO: bad enough. better to throw exeption
   }
   Word& Stack::pick(Word offset)
   {
      if( SP+offset > SP)                 // otherwise offset is bigger than the stack size (the sum overflowed)
         return am->at(SP+offset);
      Log::elog("peeking from empty stack;");
      return am->at(am->MEM_END);         // TODO: bad enough. better to throw exeption
   }


   
   
   void IntQueue::enable()
   {
      enabled = true;
   }
   void IntQueue::disable()
   {
      enabled = false;
   }
   bool IntQueue::is_enabled()
   {
      return enabled;
   }


   bool IntQueue::check_queue(Dword ic)
   {
      //  Queueing is disabled. Interrupt Queue is not empty.
      // Last task was not an interrupt.
      return !enabled && size()>0 && (ic-lsi > 1);
   } 

   void IntQueue::set_last(Dword ic)
   {
      lsi = ic;
   }



   void IntQueue::push(IntQueue::QueuePair i)
   {
      if(sz < MAX_SIZE)  
      {
         hi = next(hi);
         ints[hi] = i;
         sz++;
      }
      else throw CaughtFire(); 
   }
   void IntQueue::push(Word a, Word ia)
   {
      IntQueue::QueuePair t;
      t.a = a;
      t.ia = ia;
      this->push(t);
   }
   IntQueue::QueuePair IntQueue::pop()
   {
      if(sz == 0)
         throw QueueUnderFlow();

      QueuePair tmp = ints[lo];
      lo = next(lo);
      return tmp;
   }
   void IntQueue::dropp()
   {
      sz=lo=hi=0;
   }

   IntQueue::IntQueue()
   {
      lsi=sz=lo=hi=0;
      enabled = false;
   }

   Byte IntQueue::next(Byte pos)
   {
      if(pos == MAX_SIZE-1)
         return 0;
      return ++pos;
   }
   Word IntQueue::size()
   {
      return sz;
   }
}