#pragma once
#include "address.h"
#include <array>

class Ppu{

    public:
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value); 

    private:
        //memory
        std::array<uint8_t, 160> oam{}; 
        std::array<uint8_t, 8192> vram{}; //8KiB

};