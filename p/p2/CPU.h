#ifndef CPU_H
#define CPU_H


class CPU
{
public:
  CPU()
  :AR_(0), PC_(0), DR_(0), AC_(0), R_(0), Flag_(0)
  {}

/*==========================================================================================
Functions
==========================================================================================*/
  void fetch();

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