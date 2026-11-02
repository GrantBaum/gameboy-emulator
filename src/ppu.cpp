#include "ppu.h"
#include <stdexcept>

uint8_t Ppu::read(uint16_t address){
    if(address >= VRAM_START && address <= VRAM_END){
        address -= VRAM_START;
        return vram.at(address);
    }
    else if(address >= OAM_START && address <= OAM_END){
        address -= OAM_START;
        return oam.at(address);
    }
    else throw std::runtime_error("readError: MEMORY TRIED TO ACCESS PPU AND THEN PROVIDED INVALID PPU ADDRESS");
}
void Ppu::write(uint16_t address, uint8_t value){
    if(address >= VRAM_START && address <= VRAM_END){
        address -= VRAM_START;
        vram.at(address) = value;
    }
    else if(address >= OAM_START && address <= OAM_END){
        address -= OAM_START;
        oam.at(address) = value;
    }
    else throw std::runtime_error("writeError: MEMORY TRIED TO ACCESS PPU AND THEN PROVIDED INVALID PPU ADDRESS");
}