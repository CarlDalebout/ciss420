#include "CPU.h"

void CPU::fetch()
{
  AR_ = PC_;
  std::cout << "fetch 1: AR = " << AR_ << " and PC = " << PC_ << '\n'; 
  
  //DR_ = 
  PC_++;
  std::cout << "fetch 2: AR = " << DR_ << " and PC = " << PC_ << '\n'; 
  
  IR_ = DR_;
  AR_ = PC_;
  std::cout << "fetch 3: IR = " << AR_ << " and PC = " << AR_ << '\n'; 
  
}

void CPU::do_instruction(uint8_t opCode, std::ifstream & fileRead)
{
  std::string value;
  switch (opCode)
  {
    case 0: //NOP 00
    {
      std::cout << "nop instruction\n";

    }
    break;
    case 1: //LDAC 01
    {
      std::cout << "LDAC instruction\n";
      std::getline(fileRead, value);
      //AC_ = M[std::stoi(value, NULL, 16)];
    }
    break;
    case 2: //STAC 02
    {
      std::cout << "STAC instruction\n";
      std::getline(fileRead, value);
      //M[std::stoi(value, NULL, 16)] = AC;
    }
    break;
    case 3: //MVAC 03
    {
      std::cout << "MVAC instruction\n";
      R_ = AC_;
    }
    break;
    case 4: //MOVR 04
    {
      std::cout << "MOVR instruction\n";
      AC_ = R_;
    }
    break;
    case 8: //ADD 08
    {
      std::cout << "ADD instruction\n";
      // if (AC_ + R_ > 255)
      // {

      // }
      AC_ = AC_ + R_;
    }
    break;
    case 9: //SUB 09
    {
      std::cout << "SUB instruction\n";
      AC_ = AC_ - R_;
    }
    break;
    case 10: //INAC 0A
    {
      std::cout << "INAC instruction\n";
      AC_ = AC_ + 1;
    }
    break;
    case 11: //CLAC 0B
    {
      std::cout << "CLAC instruction\n";
      AC_ = 0;
    }
    break;
    case 12: //AND 0C
    {
      std::cout << "AND instruction\n";
      AC_ = AC_ & R_;
    }
    break;
    case 13: //OR 0D
    {
      std::cout << "OR instruction\n";
      AC_ = AC_ | R_;
    }
    break;
    case 14: //XOR 0E
    {
      std::cout << "XOR instruction\n";
      AC_ = AC_ | R_;
    }
    break;
    case 15: //NOT 0F
    {
      std::cout << "NOT instruction\n";
    }
    break;
    case 22: //MVI 16
    {
      std::cout << "MVI instruction\n";
      std::getline(fileRead, value);
      AC_ = std::stoi(value, nullptr, 16);
    }
    break;
    case 255: //
    {
      PC_ = -1;
    }
    break; 
    default:
    break;
  }
}

