#include "CPU.h"
#include <iostream>

void CPU::fetch()
{
  AR_ = PC_;
  std::cout << "fetch1: AR = " << AR_ << " and PC = " << PC_ << '\n'; 
  
}