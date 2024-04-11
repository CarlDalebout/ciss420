#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <fstream>
#include <string>

class CPU
{
public:
  CPU()
  : AR_(0), PC_(0), DR_(0), IR_(0), TR_(0), 
    AC_(0), R_(0), Flag_(0)
  {}

/*==========================================================================================
Functions
==========================================================================================*/
  void fetch();
  void do_instruction(uint8_t, std::ifstream &);

/*==========================================================================================
Inline Functions
==========================================================================================*/
  inline void CPU::execution()
  {
    std::cout << "Instruction execution complete: AC = " <<   AC_ << 
                                                " R = " <<    R_ << 
                                              " flag = " << Flag_ << 
                                                " AR = " <<   AR_ <<
                                                " PC = " <<   PC_ <<
                                                " DR = " <<   DR_ << 
                                                std::endl;
  }

/*==========================================================================================
  Seters and Geters
==========================================================================================*/
  int pc() { return PC_; }
private:
  uint16_t  AR_;
  uint16_t  PC_;
  uint8_t   DR_;
  uint8_t   IR_;
  uint8_t   TR_;
  uint8_t   AC_;
  uint8_t    R_;
  uint8_t Flag_; /* NVCZ */
  
};

#endif