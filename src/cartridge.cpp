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
size_t Cartridge::ramBytes(uint8_t data){
    switch(data){
        case 0x00:
            return 0; //none
            break;
        case 0x01:
            return -1; //unused
            break;
        case 0x02:
            return 8192; //8KiB
            break;
        case 0x03:
            return 32768; //32KiB
            break;
        case 0x04:
            return 	131072; //128KiB
            break;
        case 0x05:
            return 65536; //64KiB
            break;
        
        default:
            throw std::runtime_error("INVALID EXTERNAL RAM SIZE DATA");
            break;
    }
}
size_t Cartridge::romBytes(uint8_t data){
    switch(data){
        case 0x00:
            return 32768; //32 KiB
            break;
        case 0x01:
            return 65536; //64KiB
            break;
        case 0x02:
            return 131072; //128KiB
            break;
        case 0x03:
            return 262144; //256KiB
            break;
        case 0x04:
            return 524288; //512KiB
            break;
        case 0x05:
            return 1048576; //1MB
            break;
        case 0x06:
            return 2097152; //2MB
            break;
        case 0x07:
            return 4194304; //4MB
            break;
        case 0x08:
            return 	8388608; //8MB
            break;
        
        default:
            throw std::runtime_error("INVALID ROM SIZE DATA");
            break;
    }
}