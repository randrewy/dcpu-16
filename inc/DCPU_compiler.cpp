#include "DCPU_compiler.h"
/*
  -Primary expression means one of:
   COMMENT, LABEL, ENDLINE, BASICOPCD, SPECOPCD, NONE

*/
namespace DCPU_C{
   using namespace DCPU::Codes;
   int instr_count;

   std::map<std::string, Ltbl_Entry> Label_tbl;
   
   struct Raw_argument{
      DCPU::Word code, next_w;         // arg code
      std::string label_call;          // label name that is called here
      bool extra;                      // is  extra word used?
      Raw_argument(DCPU::Word c, DCPU::Word n): code(c), next_w(n), extra(1){}
      Raw_argument(DCPU::Word c): code(c), extra(0){}
      Raw_argument(DCPU::Word c, std::string lbl_call): code(c), label_call(lbl_call), extra(1){}  // no small - leteral labels
    private:
      Raw_argument(){}	
   };
   
   struct Raw_Instruction{
      std::vector<DCPU::Word> w;   
      bool need_remap;                 // 
   };
   
   std::vector<Raw_Instruction> rasm;
   int instructionAdress(int i);

   struct Code_pair{
      int inst,
          line;
      Code_pair(int inst_, int line_) : inst(inst_), line(line_){}
   };
   std::vector<Code_pair> line_match;

   std::vector<std::string> Lines;    // source code



   int primary (std::istream *input, Raw_Instruction& output)     // returns number of arguments
   {
      Lexem L = getLexem(input);
      switch(L.type)
      {
		 case COMMENT: 
         case ENDLINE:
         case NONE:
            return 0;               // no arguments are 

         case LABEL_DEF:
            Label_tbl[L.str_val].instruction_number = instr_count;// label points to the next instruction
            return primary(input, output);                        // another attempt to get primary expression

         case BASICOPCD:
            output.w.push_back(0);
            DCPU::setOCode(output.w[0], BasicI[L.str_val]);
            
            return 2;               // 2 arguments needed

         case SPECOPCD:
            output.w.push_back(0);
            DCPU::setOCode(output.w[0], 0);
            DCPU::setDCode(output.w[0], SpecI[L.str_val]);
            return 1;               // 1 argument needed

       //case EXTRA:
       //
       //   return 3;               // various number of arguments
         default:
            throw;
      }
   }
   /*
   Lexem Last_Lexem, temp1, temp2;
   Raw_argument secondary(std::istream *input, bool get)
   {
      if(get) Last_Lexem = getLexem(input);
      switch(Last_Lexem.type)
      {
         case REG_OR_STACK:
         	return Raw_argument(Values[Last_Lexem.str_val]);

         case NUMBER:
            temp1 = getLexem(input);
            switch(temp1.type)
            {
               case PLUS:
                  temp2 = getLexem(input);

               case COMMA:
               case COMMENT: 
               case ENDLINE:
               case NONE:
                  Last_Lexem = temp1;
                  if(Last_Lexem.num_val < DCPU::SMALL_LITERAL_MAX && DCPU::Signed(Last_Lexem.num_val) >= -1)            // small literal 
                     return Raw_argument(DCPU::SMALL_LITERAL_OFFSET + Last_Lexem.num_val);
			         else
			            return Raw_argument(DCPU::Codes::NEXTW, Last_Lexem.num_val);
            }

         case LABEL:
            return Raw_argument(DCPU::Codes::NEXTW, Last_Lexem.str_val);




      }

      return 1;
   }

   void check_endline(std::istream *input)
   {
      Lexem L = getLexem(input);
      switch(L.type)
      {
         case COMMENT: 
         case ENDLINE:
         case NONE:
            return; 
         default:
            throw;
      }
   }



   bool parse (std::istream *input, Raw_Instruction& output){return 1;}






   
   

               
   void compile()
   {
      instr_count = 0;
      rasm.clear();
      line_match.clear();
      Ltbl.clear();

      
      
      for (unsigned int i = 0; i < Lines.size(); i++)
      {
         try{
            Raw_Instruction current;
            std::stringstream ss(Lines[i]);
            switch(primary(&ss, current))             
            {
               case 0:            // no opcode parsed
                  continue;       // goto next line
               case 1:
                  secondary(&ss, current, arg_type::SRC);         // get source argument 
                  check_endline(&ss);
                  break;

               case 2:           // Basic opcode 
                  secondary(&ss, current, arg_type::SRC);         // get source argument 
                  if(getLexem(&ss).type != COMMA)
                     throw;
                  secondary(&ss, current, arg_type::DST);         // get dest arg

                  check_endline(&ss);
                  break;
               //case 3:      
            }
            line_match.push_back(Code_pair(instr_count,i));    // some debug info
            rasm.push_back(current);                           // adds instruction 
            instr_count++;
         }
         catch(...)
         {}
      }
   }
   */

}
   
