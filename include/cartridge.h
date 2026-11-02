#pragma once
#include "address.h"
#include <array>
#include <vector>
#include <string>
#include <cstdint>

class Cartridge{

    friend struct CartridgeTest;

    public:
        void load(const std::string& path);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
        void cart_info(); //cause it should cout the stuff instead of returning. Could make it return string too.

    private:
   
        std::vector<uint8_t> rom;
        std::vector<uint8_t> ext_ram;

        //private helpers
        size_t ramBytes(uint8_t data);
        size_t romBytes(uint8_t data);
};