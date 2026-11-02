#pragma once
#include "address.h"

class Timer{

    public:

    private:
};

/*

Timer
  public:
    constructor(Interrupts&)

    read(address)         FF04 to FF07
    write(address, value) FF04 to FF07
    step(cycles)          move time forward, maybe request the Timer interrupt

  private:
    counter      16 bits, goes up every cycle. DIV is its top byte.
    TIMA         one byte
    TMA          one byte
    TAC          one byte
    interrupts   reference to Interrupts
    
    */