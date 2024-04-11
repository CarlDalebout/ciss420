#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class CPU
{
public:
  CPU()
  : AR_(0), PC_(0), DR_(0), IR_(0), TR_(0), 
    AC_(0), R_(0), Flag_(0), Done_(false), M_(std::vector<uint8_t> (65536))
  {}

/*==========================================================================================
Functions
==========================================================================================*/
  void fetch();
  void do_instruction(uint8_t, std::ifstream &);

/*==========================================================================================
Inline Functions
==========================================================================================*/
  inline void execution()
  {
    std::cout << "Instruction execution complete: AC = " << (int)  AC_ << 
                                                 " R = " << (int)   R_ << 
                                              " flag = " << (int)Flag_ << 
                                                " AR = " << (int)  AR_ <<
                                                " PC = " << (int)  PC_ <<
                                                " DR = " << (int)  DR_ << 
                                                    "\n" << std::endl;
  }

/*==========================================================================================
  Seters and Geters
==========================================================================================*/
  int pc()    { return PC_; }
  bool done() { return Done_; }
private:
  uint16_t            AR_;
  uint16_t            PC_;
  uint8_t             DR_;
  uint8_t             IR_;
  uint8_t             TR_;
  uint8_t             AC_;
  uint8_t              R_;
  uint8_t           Flag_; /* NVCZ */
  bool              Done_;  
  std::vector<uint8_t> M_;
};

#endif