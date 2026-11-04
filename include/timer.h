#pragma once
#include "address.h"

class Timer{

    public:
      //constructor
      Timer(Interrupts& inter);


      //readers and writers
      uint8_t read(uint16_t address);
      void write(uint16_t address, uint8_t value);
      void step(size_t cycles);

    private:
      uint16_t counter; //goes up every cycle
      uint8_t TIMA;
      uint8_t TMA;
      uint8_t TAC;

      Interrupts& interrupt;
};
