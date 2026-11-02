#pragma once

class Interrupts{

    public:

    private:
};

/*

Interrupts
  public:
    enum class Type: VBlank = bit 0, Stat = bit 1, Timer = bit 2,
                     Serial = bit 3, Joypad = bit 4

    request(type)         turn that bit on in IF
    clear(type)           turn that bit off in IF (CPU calls this once it handles one)
    pending()             return IE & IF, only the 5 real bits

    read(address)         FF0F gives IF, FFFF gives IE
    write(address, value) FF0F sets IF, FFFF sets IE

  private:
    IF    one byte
    IE    one byte
    
    */