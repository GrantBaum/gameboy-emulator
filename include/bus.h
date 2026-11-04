#pragma once
#include "address.h"
#include "cartridge.h"
#include "ppu.h"
#include <array>
#include <cstdint>


class Bus {

    public:

        //constructor
        Bus(Cartridge& cart, Ppu& graphicsCard);
        
        //helpers
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value); 

    private:

        //reference to the cartridge
        Cartridge& cartridge;
        Ppu& graphics;

        //enum class for different locations
        enum class Reigon{
            VRAM, WRAM, OAM, IO, HIGH, IE,
            ROMO, ROMN,
            EXT,
            UNUSABLE, ECHO
        };

        //memory
        std::array<uint8_t, 8192> work_ram{}; //8KiB
        std::array<uint8_t, 128> io_ram{};
        std::array<uint8_t, 127> high_ram{};

        //private helpers
        Bus::Reigon getReigon(uint16_t addr);
};