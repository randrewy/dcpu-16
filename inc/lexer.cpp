#include "lexer.h"

std::map<std::string, DCPU::Codes::BasicOpcode> Lexer::BasicI;
std::map<std::string, DCPU::Codes::SpecialOpcode> Lexer::SpecI;
std::map<std::string, short> Lexer::Values;
std::map<std::string, short> Lexer::Labels;


void initialize();
class Lets_dont_use_initializer_lists{
public:
   Lets_dont_use_initializer_lists(){initialize();}
}initializer;


template<class T> int contains(T mp, std::string val)
{
   return mp.count(val);
}


std::string read_alnum(std::istream *input)
{
   char ch;
   std::string str;
   while(input->get(ch) && std::isalnum(ch)) 
         str.push_back(char(toupper(ch))); 
   input->putback(ch);
   return str;
}


Lexer::Lexem Lexer::getLexem(std::istream *input)
{
   char ch=0;
   //int input_start_pos = input->tellg();
   std::string str;
   DCPU::Word  ival;

   do{input->get(ch);
      }while(ch!='\n'&&std::isspace(ch)); //skips all white-space characters except EOL

   switch(ch)
   {
      case ':':
         return Lexem(LABEL_DEF, read_alnum(input));

      case ';':
         return COMMENT;
            
      case '\n':
         return ENDLINE;

      case ',':
         return COMMA;

      case '[':
         return LB;

      case ']':
         return RB;

      case '+':
         return PLUS;

      case '\0':
         return NONE; // no endl? okay =\

      default:
      {
         if(isalpha(ch))
         {
            str = char(toupper(ch));
            str += read_alnum(input);  //alnum is needed to 'prevent correct parsing' of cases like "INT0"
               
            if(contains(BasicI, str))
               return Lexem(BASICOPCD, str);

            else if(contains(SpecI, str))
               return Lexem(SPECOPCD, str);

            else if(contains(Values, str))
               return Lexem(REG_OR_STACK, str);
               
            else if(contains(Labels, str))			// lowest priority
               return Lexem(LABEL, str);
               
            else 
               return Lexem(UNDEFIND_WORD, str);
         }
         else if(isdigit(ch))
         {
            if(ch == '0'){
               input->get(ch);
               if(ch == 'x' || ch =='X')                    // 0x_(hex value)
                  *(input)>>std::hex>>ival;
               else if(isdigit(ch))             // 0_(oct value)
               {
                  input->putback(ch);
                  *(input)>>std::oct>>ival;
               }
               else                             // 0 (zero)
               {
                  input->putback(ch);           
                  ival = 0;
               }
            }
            else{                               // other number
               input->putback(ch);              
               *(input)>>ival;
            }
            return Lexem(NUMBER, ival);
         }
         else                                   // none of above - smth is wrong
         {
            str = ch;
            return Lexem(UNDEFIND_SYMBOL, str);
         }
      }
   }
}


void initialize()
{
   Lexer::BasicI["SET"] = DCPU::Codes::SET;    
   Lexer::BasicI["ADD"] = DCPU::Codes::ADD;
   Lexer::BasicI["SUB"] = DCPU::Codes::SUB;
   Lexer::BasicI["MUL"] = DCPU::Codes::MUL;  
   Lexer::BasicI["MLI"] = DCPU::Codes::MLI;
   Lexer::BasicI["DIV"] = DCPU::Codes::DIV;
   Lexer::BasicI["DVI"] = DCPU::Codes::DVI;
   Lexer::BasicI["MOD"] = DCPU::Codes::MOD;
   Lexer::BasicI["MDI"] = DCPU::Codes::MDI;    
   Lexer::BasicI["AND"] = DCPU::Codes::AND;
   Lexer::BasicI["BOR"] = DCPU::Codes::BOR;
   Lexer::BasicI["XOR"] = DCPU::Codes::XOR;   
   Lexer::BasicI["SHR"] = DCPU::Codes::SHR;
   Lexer::BasicI["ASR"] = DCPU::Codes::ASR;
   Lexer::BasicI["SHL"] = DCPU::Codes::SHL;
   Lexer::BasicI["IFB"] = DCPU::Codes::IFB;
   Lexer::BasicI["IFC"] = DCPU::Codes::IFC;   
   Lexer::BasicI["IFE"] = DCPU::Codes::IFE;
   Lexer::BasicI["IFN"] = DCPU::Codes::IFN;
   Lexer::BasicI["IFG"] = DCPU::Codes::IFG;  
   Lexer::BasicI["IFA"] = DCPU::Codes::IFA;
   Lexer::BasicI["IFL"] = DCPU::Codes::IFL;
   Lexer::BasicI["IFU"] = DCPU::Codes::IFU;
   Lexer::BasicI["ADX"] = DCPU::Codes::ADX;
   Lexer::BasicI["SBX"] = DCPU::Codes::SBX;
   Lexer::BasicI["STI"] = DCPU::Codes::STI;
   Lexer::BasicI["STD"] = DCPU::Codes::STD;

   Lexer::SpecI["JSR"] = DCPU::Codes::_S_JSR;
   Lexer::SpecI["INT"] = DCPU::Codes::_S_INT;
   Lexer::SpecI["IAG"] = DCPU::Codes::_S_IAG;
   Lexer::SpecI["IAS"] = DCPU::Codes::_S_IAS;
   Lexer::SpecI["RFI"] = DCPU::Codes::_S_RFI;
   Lexer::SpecI["IAQ"] = DCPU::Codes::_S_IAQ;
   Lexer::SpecI["HWN"] = DCPU::Codes::_S_HWN;
   Lexer::SpecI["HWQ"] = DCPU::Codes::_S_HWQ;
   Lexer::SpecI["HWI"] = DCPU::Codes::_S_HWI;

   Lexer::Values["A"]    = DCPU::Codes::A;
   Lexer::Values["B"]    = DCPU::Codes::B;
   Lexer::Values["C"]    = DCPU::Codes::C;
   Lexer::Values["X"]    = DCPU::Codes::X;
   Lexer::Values["Y"]    = DCPU::Codes::Y;
   Lexer::Values["Z"]    = DCPU::Codes::Z;
   Lexer::Values["I"]    = DCPU::Codes::I;
   Lexer::Values["J"]    = DCPU::Codes::J;
   Lexer::Values["PUSH"] = DCPU::Codes::PUSHPOP;
   Lexer::Values["POP"]  = DCPU::Codes::PUSHPOP;
   Lexer::Values["PEEK"] = DCPU::Codes::PEEK;
   Lexer::Values["PICK"] = DCPU::Codes::PICK;
   Lexer::Values["SP"]   = DCPU::Codes::SP;
   Lexer::Values["PC"]   = DCPU::Codes::PC;
   Lexer::Values["EX"]   = DCPU::Codes::EX;

}


