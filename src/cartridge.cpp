#include "cartridge.h"
#include <fstream>
#include <iterator>
#include <stdexcept>

void Cartridge::load(const std::string& path){
    std::ifstream file(path, std::ios::binary); //open file
    if (!file) {
        throw std::runtime_error("Could not open ROM: " + path); //throw if didnt open
    }

    rom = std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
}
uint8_t Cartridge::read(uint16_t address){
    return rom.at(address);
}