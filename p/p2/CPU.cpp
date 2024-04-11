#include "CPU.h"

void CPU::fetch()
{
  AR_ = PC_;
  std::cout << "fetch 1: AR = " << (int)AR_ << " and PC = " << (int)PC_ << '\n'; 
  
  DR_ = M_[AR_];
  PC_++;
  std::cout << "fetch 2: DR = " << (int)DR_ << " and PC = " << (int)PC_ << '\n'; 
  
  IR_ = DR_;
  AR_ = PC_;
  std::cout << "fetch 3: IR = " << (int)IR_ << " and AR = " << (int)AR_ << '\n'; 
  
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
      DR_ = std::stoi(value, NULL, 16);
      PC_ += 1;
      AR_ += 1;
      std::cout << "LDAC1:\tDR = " << (int)DR_ << " PC = " << (int)PC_ << " AR = " << (int)AR_ << '\n';
      std::getline(fileRead, value);
      TR_ = DR_;
      DR_ = std::stoi(value, NULL, 16);
      PC_ += 1;
      std::cout << "LDAC2:\tTR = " << (int)TR_ << " DR = " << (int)DR_ << " AR = " << (int)AR_ << " PC = " << (int)PC_ << '\n';
      AR_ = (TR_ << 8) | DR_;
      std::cout << "LDAC3:\tAR = " << (int)AR_ << '\n';
      DR_ = M_[AR_];
      std::cout << "LDAC4:\tDR = " << (int)DR_ << '\n';
      AC_ = DR_;
      std::cout << "LDAC5:\tAC = " << (int)AC_ << '\n';
    }
    break;
    case 2: //STAC 02
    {
      std::cout << "STAC instruction\n";
      PC_ += 1;
      std::getline(fileRead, value);
      DR_ = std::stoi(value, NULL, 16);
      PC_ += 1;
      AR_ += 1;
      std::cout << "STAC1:\tDR = " << (int)DR_ << " PC = " << (int)PC_ << " AR = " << (int)AR_ << '\n';
      std::getline(fileRead, value);
      TR_ = DR_;
      DR_ = std::stoi(value, NULL, 16);
      PC_ += 1;
      std::cout << "STAC2:\tTR = " << (int)TR_ << " DR = " << (int)DR_ << " AR = " << (int)AR_ << " PC = " << (int)PC_ << '\n';
      AR_ = (TR_ << 8) | DR_;
      std::cout << "STAC3:\tAR = " << (int)AR_ << '\n';
      M_[AR_] = AC_;
    }
    break;
    case 3: //MVAC 03
    {
      std::cout << "MVAC instruction\n";
      R_ = AC_;
      std::cout << "MVAC1: R = " << (int)R_ << '\n';
    }
    break;
    case 4: //MOVR 04
    {
      std::cout << "MOVR instruction\n";
      AC_ = R_;
      std::cout << "MOVR1: AC = " << (int)AC_ << '\n';
    }
    break;
    case 8: //ADD 08
    {
      std::cout << "ADD instruction\n";
      bool V = (Flag_ >> 3);
      AC_ = AC_ + R_;
      // set/reset flag
      if(AC_ > 127)
        Flag_ = Flag_ | 8;
      if(V != (Flag_ >> 3))
        Flag_ = Flag_ | 4;
      
    }
    break;
    case 9: //SUB 09
    {
      std::cout << "SUB instruction\n";
      bool V = (Flag_ >> 3);
      AC_ = AC_ - R_;
      // set/reset flag
      if(AC_ > 127)
        Flag_ = Flag_ | 8;
      if(V != (Flag_ >> 3))
        Flag_ = Flag_ | 4;
      
    }
    break;
    case 10: //INAC 0A
    {
      std::cout << "INAC instruction\n";
      bool V = (Flag_ >> 3);
      AC_ = AC_ + 1;
      //set/reset flag
      if(AC_ > 127)
        Flag_ = Flag_ | 8;
      if(V != (Flag_ >> 3))
        Flag_ = Flag_ | 4;
      
      std::cout << "INAC1: AC = " << (int)AC_ << " FLAG = " << (int)Flag_ << '\n';
    }
    break;
    case 11: //CLAC 0B
    {
      std::cout << "CLAC instruction\n";
      AC_ = 0;
      // Z set and CNV reset
      Flag_ = 1;
      std::cout << "CLAC1: AC = " << (int)AC_ << " FLAG = " << (int)Flag_ << '\n';
    }
    break;
    case 12: //AND 0C
    {
      std::cout << "AND instruction\n";
      // ZN set/reset
      AC_ = AC_ & R_;
      if (AC_ == 0)
        Flag_ = 1;
      else if (AC_ > 127)
        Flag_ = 8;
      else
        Flag_ = 0;
      std::cout << "AND1: AC = " << (int)AC_ << " Flags = " << (int)Flag_ << '\n';
    }
    break;
    case 13: //OR 0D
    {
      std::cout << "OR instruction\n";
      AC_ = AC_ | R_;
      // ZN set/reset
      if (AC_ == 0)
        Flag_ = 1;
      else if (AC_ > 127)
        Flag_ = 8;
      else
        Flag_ = 0;
      std::cout << "OR1: AC = " << (int)AC_ << " Flags = " << (int)Flag_ << '\n';
    }
    break;
    case 14: //XOR 0E
    {
      std::cout << "XOR instruction\n";
      AC_ = AC_ | R_;
      // ZN set/reset
      if (AC_ == 0)
        Flag_ = 1;
      else if (AC_ > 127)
        Flag_ = 8;
      else
        Flag_ = 0;
      std::cout << "XOR1: AC = " << (int)AC_ << " Flags = " << (int)Flag_ << '\n';
    }
    break;
    case 15: //NOT 0F
    {
      std::cout << "NOT instruction\n";
      AC_ = !AC_;
      // ZN set/reset
      if (AC_ == 0)
        Flag_ = 1;
      else if (AC_ > 127)
        Flag_ = 8;
      else
        Flag_ = 0;
      std::cout << "not1: AC = " << (int)AC_ << " Flags = " << (int)Flag_ << '\n';
    }
    break;
    case 22: //MVI 16
    {
      std::cout << "MVI instruction\n";
      std::getline(fileRead, value);
      PC_ += 1;
      AR_ += 1;
      DR_ = std::stoi(value, nullptr, 16);
      std::cout << "MVI1: DR = " << (int)DR_ << " PC = " << (int)PC_ << " AR = " << (int)AR_ << '\n';
      AC_ = DR_;
      std::cout << "MVI2: AC = " << (int)AC_ << '\n';
    }
    break;
    case 255: //
    {
      Done_ = true;
    }
    break; 
    default:
    {
      std::cout << "\n\n\n Error That OP Code does not exist\n\n\nProgram Terminated\n";
      Done_ = true;
    }
    break;
  }
}

