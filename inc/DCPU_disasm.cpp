#include "DCPU_disasm.h"
namespace DCPU_disasm{
   using namespace DCPU;

   std::map<short, std::string> BasicI;
   std::map<short, std::string> SpecI;
   std::map<short, std::string> Values;
   std::map<short, std::string> Labels;

   std::vector<std::string> Lines; 
   std::vector<std::string>::iterator curr_line;

   std::string getSource(Word code);
   std::string getDest(Word code);
   void initialize();

   std::string Itoa(int i, int radix = use_base)
   {
      char buff[9];
      switch(use_base)
      {
         case 10:
            return _itoa(i, buff, radix);
         case 8:
            return std::string("0")+_itoa(i, buff, radix);
         case 16:
            return std::string("0x")+_itoa(i, buff, radix);
         default:
            return std::string("(") + _itoa(radix, buff, 10)+
               std::string(")x")+_itoa(i, buff, radix);
      }
   }

   Memory ram;
   Word PC;
   int instr_count;
   const Byte REG_SIZE = 8; 
   const std::string space =" ";
   const std::string comma =", ";

   Word& next_word()   
   {
      return ram.at(PC++);            
   } 
   void prepare()
   {
      PC = 0;
      instr_count = 0;
      initialize();
      Lines.clear();
      Lines.push_back("");
      curr_line = Lines.begin();
   }
   void load_code(Word* mem, int size)
   {
      for(Word i=0; i<size;i++)
         ram.at(i) = mem[i];
   }



   
   void step()
   {
      instr_count++;
    
      Word  current = next_word();  // get next word
      Word  op    = getOCode(current),
            scode = getSCode(current),
            dcode = getDCode(current);

      std::string src_arg;

      switch(op)
      {
         case Codes::SPH:        // Special opcodes handeled here
            switch(dcode)        // op == 0; destination part of word determine type of instruction 
            {
               case Codes::_S_IAG:
               case Codes::_S_HWN:
                  curr_line->append(SpecI[dcode]);
                  curr_line->append(space);
                  curr_line->append(getDest(scode));  // NOTE getDEST
                  break;
               default:
                  curr_line->append(SpecI[dcode]);
                  curr_line->append(space);
                  curr_line->append(getSource(scode));
                  break;
            }
            break;
        
         default:
            src_arg = getSource(scode);
            curr_line->append(BasicI[op]);
            curr_line->append(space);
            curr_line->append(getDest(dcode));
            curr_line->append(comma);
            curr_line->append(src_arg);
            break;
      }
      curr_line->append("\n");
      Lines.push_back("");
      curr_line = --Lines.end();
   }


   std::string getDest(Word code)
   {
      using namespace Codes;
      switch (code)
      {
         case A: case B: case C: case X:
         case Y: case Z: case I: case J:
         case Codes::PC: case Codes::EX:
         case PEEK: case PICK: case SP:
            return Values[code];

         case aA: case aB: case aC: case aX:
         case aY: case aZ: case aI: case aJ:
            return '[' + Values[code-REG_SIZE] + ']';

         case aAnw: case aBnw: case aCnw: case aXnw:
         case aYnw: case aZnw: case aInw: case aJnw:
            return '[' + Values[code - 2*REG_SIZE] + " + " + Itoa(next_word()) + ']';

         case PUSHPOP:
            return "PUSH";
         
         case aNEXTW:
            return "["+ Itoa(next_word()) + ']';

         case NEXTW:
            return Itoa(next_word()); 

         default:
            return Itoa(code);      // small literal
      }
   }

   std::string getSource(Word code)
   {
      if(code == Codes::PUSHPOP)
         return "POP";
      else if(code >= SMALL_LITERAL_SHIFT)
         return Itoa(code - SMALL_LITERAL_OFFSET); 
      else
         return getDest(code);
   }


   void initialize()
   {
      BasicI[ DCPU::Codes::SET ] = "SET";    
      BasicI[ DCPU::Codes::ADD ] = "ADD";
      BasicI[ DCPU::Codes::SUB ] = "SUB";
      BasicI[ DCPU::Codes::MUL ] = "MUL";  
      BasicI[ DCPU::Codes::MLI ] = "MLI";
      BasicI[ DCPU::Codes::DIV ] = "DIV";
      BasicI[ DCPU::Codes::DVI ] = "DVI";
      BasicI[ DCPU::Codes::MOD ] = "MOD";
      BasicI[ DCPU::Codes::MDI ] = "MDI";    
      BasicI[ DCPU::Codes::AND ] = "AND";
      BasicI[ DCPU::Codes::BOR ] = "BOR";
      BasicI[ DCPU::Codes::XOR ] = "XOR";   
      BasicI[ DCPU::Codes::SHR ] = "SHR";
      BasicI[ DCPU::Codes::ASR ] = "ASR";
      BasicI[ DCPU::Codes::SHL ] = "SHL";
      BasicI[ DCPU::Codes::IFB ] = "IFB";
      BasicI[ DCPU::Codes::IFC ] = "IFC";   
      BasicI[ DCPU::Codes::IFE ] = "IFE";
      BasicI[ DCPU::Codes::IFN ] = "IFN";
      BasicI[ DCPU::Codes::IFG ] = "IFG";  
      BasicI[ DCPU::Codes::IFA ] = "IFA";
      BasicI[ DCPU::Codes::IFL ] = "IFL";
      BasicI[ DCPU::Codes::IFU ] = "IFU";
      BasicI[ DCPU::Codes::ADX ] = "ADX";
      BasicI[ DCPU::Codes::SBX ] = "SBX";
      BasicI[ DCPU::Codes::STI ] = "STI";
      BasicI[ DCPU::Codes::STD ] = "STD";

      SpecI[ DCPU::Codes::_S_JSR ] = "JSR";
      SpecI[ DCPU::Codes::_S_INT ] = "INT";
      SpecI[ DCPU::Codes::_S_IAG ] = "IAG";
      SpecI[ DCPU::Codes::_S_IAS ] = "IAS";
      SpecI[ DCPU::Codes::_S_RFI ] = "RFI";
      SpecI[ DCPU::Codes::_S_IAQ ] = "IAQ";
      SpecI[ DCPU::Codes::_S_HWN ] = "HWN";
      SpecI[ DCPU::Codes::_S_HWQ ] = "HWQ";
      SpecI[ DCPU::Codes::_S_HWI ] = "HWI";

      Values[ DCPU::Codes::A ]    = "A";
      Values[ DCPU::Codes::B ]    = "B";
      Values[ DCPU::Codes::C ]    = "C";
      Values[ DCPU::Codes::X ]    = "X";
      Values[ DCPU::Codes::Y ]    = "Y";
      Values[ DCPU::Codes::Z ]    = "Z";
      Values[ DCPU::Codes::I ]    = "I";
      Values[ DCPU::Codes::J ]    = "J";
      Values[ DCPU::Codes::PUSHPOP ] = "STACK";
      Values[ DCPU::Codes::PEEK ] = "PEEK";
      Values[ DCPU::Codes::PICK ] = "PICK";
      Values[ DCPU::Codes::SP ]   = "SP";
      Values[ DCPU::Codes::PC ]   = "PC";
      Values[ DCPU::Codes::EX ]   = "EX";
     
   }



}