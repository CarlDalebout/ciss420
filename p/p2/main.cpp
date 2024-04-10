#include <iostream>
#include <fstream>
#include <string>
#include "CPU.h"


int main()
{
  std::ifstream fileRead;
  std::string user_input = "";
  CPU cpu;

  while(true)
  {
    std::cout << "Enter the name of the file containing the program: ";
    std::getline(std::cin, user_input);
    fileRead.open(user_input);

    if(fileRead.is_open())
      break;
    else
      std::cout << "\n\n!!!!ERROR The " << user_input << " ran into an error when opening\n\n\n";
  
  }

  std::cout << "The " << user_input << " opened properly.\n\n";
  
  int PC = 0;
  int AR = 0;

  while(!fileRead.eof())
  {
    std::getline(fileRead, user_input);

    cpu.fetch();
    uint8_t opCode = std::stoi(user_input, nullptr, 16);
    cpu.do_instruction(opCode, fileRead);
    cpu.execution();

    if(cpu.pc() == -1)
      break;
  }

  return 0;
}