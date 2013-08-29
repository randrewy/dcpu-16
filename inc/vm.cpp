#include "vm.h"

namespace DCPU
{
   struct DCPU_instruction{
      Byte cost;
      Basic_Inst instr;
   };
   //typedef void (DCPUVirtualMachine::*SpecOpcdPtr)(Word);
   extern const DCPU_instruction InstructionSet[];    

   bool need_next_word(Word val)       // checks if val (sourse or dest) needs to read next word form RAM
   {
      switch(val)
      {
         case Codes::aAnw:   case Codes::aBnw: case Codes::aCnw: case Codes::aXnw:    
         case Codes::aYnw:   case Codes::aZnw: case Codes::aInw: case Codes::aJnw:  
         case Codes::aNEXTW: case Codes::NEXTW:
            return true;
         default:
            return false;
      }
   }
   bool is_conditional(Word opcd)
   {
      switch(opcd)
      {
         case Codes::IFB: case Codes::IFC: case Codes::IFE: case Codes::IFN:   
         case Codes::IFG: case Codes::IFA: case Codes::IFL: case Codes::IFU: 
            return true;
         default:
            return false;
      }
      
   }
        
   DCPUVirtualMachine::DCPUVirtualMachine() : RAM(), stack(&RAM), IQ()
   {
      PC = 0;
      connected_devices = 0;
      total_cycles = 0;
      instr_count = 0;
      for(int i=0; i<REG_SIZE; i++)
         Reg[i]=0;
   }
   void DCPUVirtualMachine::load(Word* mem, Word size)
   {
      for(Word i=0; i<size;i++)
         RAM.at(i) = mem[i];
   }
   void DCPUVirtualMachine::connect(Hardware* h)
   {
      if(connected_devices == DEVICE_MAX) throw;
      h->connect(&RAM, Reg, PC, EX, IA);
      Devices[connected_devices++] = h;
   }
   Word DCPUVirtualMachine::tick()
   {
      instr_count++;
      Dword  last_cycle_count = total_cycles;
      
      if(IA && IQ.check_queue(instr_count)) // if IA == 0 --> interrupts are disabled
      {
         IntQueue::QueuePair p = IQ.pop();
         IA = p.ia;                          // restore interrupt parameters
         Int(p.a);                           // and call it with em
         return Word(total_cycles-last_cycle_count);  // importunate warning ;)
      }

      Word  current = next_word(),           // current word from RAM
            op      = getOCode(current),     // Codes of operation and arguments
            scode   = getSCode(current),     //
            dcode   = getDCode(current),     //
            src_arg = 0;                     

      switch(op)
      {
         case Codes::SPH:        // Special opcodes handeled here
            switch(dcode)        // op == 0; destination part of word determine type of instruction 
            {
               case Codes::_S_JSR:
                  Jsr(getSource(scode));
                  break;
               
               case Codes::_S_INT:
                  Int(getSource(scode));
                  break;

               case Codes::_S_IAG:
                  Iag(getDest(scode)); // NOTE! IAG writes it's argument!
                  break;

               case Codes::_S_IAS:
                  Ias(getSource(scode));    
                  break;

               case Codes::_S_RFI:
                  Rfi(getSource(scode));
                  IQ.set_last(instr_count);
                  break;

               case Codes::_S_IAQ:
                  Iaq(getSource(scode));
                  break;

               case Codes::_S_HWN:
                  Hwn(getDest(scode));    // NOTE! HWN writes it's argument!
                  break;

               case Codes::_S_HWQ:
                  Hwq(getSource(scode));
                  break;

               case Codes::_S_HWI:
                  Hwi(getSource(scode));
                  break;
               
               default:
                  Log::elog("Invalid special opcode:", op);
            }
            break;

         case Codes::STD:
            src_arg = getSource(scode); // source is always treated first                        
            Std(getDest(dcode), src_arg);
            break;

         case Codes::STI:
            src_arg = getSource(scode); 
            Sti(getDest(dcode), src_arg);
            break;

         case Codes::IFB: case Codes::IFC: case Codes::IFE: case Codes::IFN:
         case Codes::IFG: case Codes::IFA: case Codes::IFL: case Codes::IFU:
            total_cycles += InstructionSet[op].cost;
            src_arg = getSource(scode); 
            if(!InstructionSet[op].instr(getDest(dcode), src_arg, EX)) skip(); //Skips next instruction(s) if test fails
            break;

         default:
            if(InstructionSet[op].instr)    // all other functions 
            {    
               total_cycles += InstructionSet[op].cost;

               src_arg = getSource(scode);
               InstructionSet[op].instr(getDest(dcode), src_arg, EX);
            }
            else Log::elog("Invalid opcode:", op);
            break;
      }
      return Word(total_cycles-last_cycle_count);
   }
      
   std::string DCPUVirtualMachine::dumpstate()
   {
      std::string dump;
      char buff[] = {'_','_','_','_','_','_'};
      for(int i=0; i<8; i++)
      {
          _itoa(Reg[i],buff,16);
          dump.append(buff).append(" | ");
      }     
      return dump;
   }
   std::vector<Word> DCPUVirtualMachine::dumpMemory(Word sz, Word from)
   {
      std::vector<Word> v;
      for(Word i = 0; i<sz; i++)
         v.push_back(RAM.at(from + i));
      return v;
   }
   int DCPUVirtualMachine::totalCycles()const
   {
      return total_cycles;
   }
   int DCPUVirtualMachine::totalInstructions()const
   {
      return instr_count;
   }

   Word& DCPUVirtualMachine::getDest(Word code)
   {
      using namespace Codes;
      switch (code)
      {
         case A: case B: case C: case X:
         case Y: case Z: case I: case J:
            total_cycles+=0;
            return Reg[code];

         case aA: case aB: case aC: case aX:
         case aY: case aZ: case aI: case aJ:
            total_cycles+=0;
            return RAM.at(Reg[code-REG_SIZE]);

         case aAnw: case aBnw: case aCnw: case aXnw:
         case aYnw: case aZnw: case aInw: case aJnw:
            total_cycles+=1;
            return RAM.at(Reg[code - 2*REG_SIZE] + next_word());

         case PUSHPOP:
            total_cycles+=0;
            return stack.delayed_push();

         case PEEK:
            total_cycles+=0;
            return stack.peek();

         case PICK:
            total_cycles+=1;
            return stack.pick(next_word());

         case SP:   
            total_cycles+=0;
            return stack.sp();

         case Codes::PC:                        // there is a class member 'PC' -_- 
            total_cycles+=0;
            return PC;
            
         case Codes::EX:
            total_cycles+=0;
            return EX;
         
         case aNEXTW:
            total_cycles+=1;
            return RAM.at(next_word());

         case NEXTW:
            total_cycles+=1;
            return next_word();

         default:
            total_cycles+=0;
            return Silent;    // Attempting to write to a literal value fails silently;
      }
   }
   Word& DCPUVirtualMachine::next_word()
   {
      return RAM.at(PC++);            
   }    

   Word DCPUVirtualMachine::getSource(Word code)
   {
      if(code == Codes::PUSHPOP)
      {
         total_cycles+=0;
         return stack.pop();
      }
      else if(code >= SMALL_LITERAL_SHIFT)
      {
         total_cycles+=0;
         return code - SMALL_LITERAL_OFFSET;
      }
      else
         return getDest(code);
   }


   void DCPUVirtualMachine::skip()  // skips next instruction, including all extra words (Next_word) 
   {
      total_cycles++;                                  // extra cost for skipping
      Word curr = next_word();                         // goto next inst
      if(need_next_word(getSCode(curr)))               // next_word for source if needed
         next_word(); 
      if(need_next_word(getDCode(curr)))               // next_word for dest   if needed
         next_word(); 

      if(is_conditional(getOCode(curr)))
         skip();  // if current word was conditional skips an other instruction (conditional chain)
   }


   Byte DCPUVirtualMachine::Sti(Word& arg, Word val)
   {
      total_cycles+=2;
      arg = val;
      Reg[6]++;         //I
      Reg[7]++;         //J
      return 0;
   }
   Byte DCPUVirtualMachine::Std(Word& arg, Word val)
   {
      total_cycles+=2;
      arg = val;
      Reg[6]--;         //I
      Reg[7]--;         //J
      return 0;
   }

   void DCPUVirtualMachine::Jsr(Word val)
   {
      total_cycles+=3;
      stack.push(PC);
      PC = val;
   }
   void DCPUVirtualMachine::Int(Word val)
   {      
      total_cycles+=4;
      if(IA){
         if(!IQ.is_enabled()){
            IQ.enable();
            stack.push(PC);
            stack.push(Reg[0]);
            PC = IA;
            Reg[0] = val;
         }
         else IQ.push(val, IA);         
      }
   }
   void DCPUVirtualMachine::Iag(Word& arg)
   {
      total_cycles+=1;
      arg = IA;
   }
   void DCPUVirtualMachine::Ias(Word val)
   {
      total_cycles+=1;
      IA = val;
      if(!IA) IQ.dropp();     // clear interrupt queue if IA is set to zero
   }
   void DCPUVirtualMachine::Rfi(Word)
   {
      total_cycles+=3;
      Reg[0] = stack.pop();   
      PC = stack.pop();
      IQ.disable();
   }
   void DCPUVirtualMachine::Iaq(Word val)
   {
      total_cycles+=2;
      if(val) IQ.enable();
   }
   void DCPUVirtualMachine::Hwn(Word& arg)
   {
      total_cycles+=2;
      arg = connected_devices;
   }
   void DCPUVirtualMachine::Hwq(Word val)
   {
      total_cycles+=4;
      if(val<connected_devices)
      {
         HWInfo inf(Devices[val]->info());
         Reg[0]= getLow16 (inf.HardwareID);        // A
         Reg[1]= getHigh16(inf.HardwareID);        // B
         Reg[2]= inf.HardwareVersion;              // C
         Reg[3]= getLow16 (inf.ManufacturerID);    // X
         Reg[4]= getHigh16(inf.ManufacturerID);    // Y
      }
      else Log::elog("Error getting info of unexisting device", val);
   }
   void DCPUVirtualMachine::Hwi(Word val)
   {
      total_cycles+=4;
      if(val<connected_devices)
         total_cycles += Devices[val]->interrupt();
      else Log::elog("Error sending an interrupt to unexisting device", val);
   }



   const DCPU_instruction InstructionSet[] = { //STI and STD are handeled separately
      {0, 0},        {1, Set},      {2, Add},      {2, Sub},      
      {2, Mul},      {2, Mli},      {3, Div},      {3, Dvi},      
      {3, Mod},      {3, Mdi},      {1, And},      {1, Bor},      
      {1, Xor},      {1, Shr},      {1, Asr},      {1, Shl},      
      {2, Ifb},      {2, Ifc},      {2, Ife},      {2, Ifn},
      {2, Ifg},      {2, Ifa},      {2, Ifl},      {2, Ifu},      
      {0, 0},        {0, 0},        {3, Adx},      {3, Sbx},      
      {0, 0},        {0, 0},        {0, 0},        {0, 0}     
   };

}

