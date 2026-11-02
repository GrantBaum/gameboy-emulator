#pragma once
#include "address.h"

class Joypad{

    public:

    private:
};

/*

Joypad
  public:
    enum class Button: Right, Left, Up, Down, A, B, Select, Start

    constructor(Interrupts&)

    read(address)         FF00, built from the group pick and button states
    write(address, value) FF00, keeps only bits 4 and 5
    press(button)         called by main, requests the Joypad interrupt on a new press
    release(button)       called by main

  private:
    select       bits 4 and 5 from the last write
    dpad         4 button states, your own way (true = held)
    actions      4 button states, your own way
    interrupts   reference to Interrupts
    
    */