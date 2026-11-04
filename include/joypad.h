#pragma once
#include "address.h"

class Joypad{

    public:
      enum class Button{
        RIGHT, LEFT, UP, DOWN, A, B, SELECT, START
      };

      Joypad(Interrupts& inter);

      uint8_t read(uint16_t address);
      void write(uint16_t address, uint8_t data);
      void press(Button button);
      void release(Button button);

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