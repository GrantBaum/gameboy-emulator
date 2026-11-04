#pragma once
#include "address.h"

class Interrupts{

    public:
    //enum class for the different bits
    enum class Type{
      Vblank, //bit 0
      Stat, //bit 1
      Timer, //bit 2
      Serial, //bit 3
      Joypad //bit 4
    };

    void request(Type type); //turn that bit on in IF
    void clear(Type type); //turn that bit off in IF
    uint8_t pending(); //return IE & IF, only the 5 real bits

    uint8_t read(uint16_t address); //FF0F is IF, FFFF is IE
    void write(uint16_t address, uint8_t value);

    private:
      uint8_t IE_byte;
      uint8_t IF_byte;
};