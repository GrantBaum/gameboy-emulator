#pragma once
#include "address.h"
#include "cartridge.h"
#include <array>
#include <cstdint>


class Bus {

    public:

        //constructor
        Bus(Cartridge& cart);
        
        //helpers
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value); //I wanna give back what was there and what is written now

    private:

        //reference to the cartridge
        Cartridge& cartridge;

        //enum class for different locations
        enum class Reigon{
            VRAM, WRAM, OAM, IO, HIGH, IE,
            ROMO, ROMN,
            EXT,
            UNUSABLE, ECHO
        };

        //memory
        std::array<uint8_t, 8192> vram{}; //8KiB
        std::array<uint8_t, 8192> work_ram{}; //8KiB
        std::array<uint8_t, 160> oam{}; 
        std::array<uint8_t, 128> io_ram{};
        std::array<uint8_t, 127> high_ram{};
        uint8_t ie_byte;

        //private helpers
        Bus::Reigon getReigon(uint16_t addr);
};