#include "bus.h"
#include <stdexcept>

Bus::Bus(Cartridge& cart, Ppu& graphicsCard) : cartridge(cart), graphics(graphicsCard){}
uint8_t Bus::read(uint16_t address){

    Reigon reig = getReigon(address);

    switch(reig){
        case Reigon::ROMO:
            return cartridge.read(address);
            break;
        case Reigon::ROMN:
            return cartridge.read(address);
            break;
        case Reigon::VRAM:
            return graphics.read(address);
            break;
        case Reigon::EXT:
            return cartridge.read(address);
            break;
        case Reigon::WRAM:
            address -= WORK_START;
            return work_ram.at(address);
            break;
        case Reigon::ECHO:
            address -= ECHO_START;
            return work_ram.at(address); //same as work ram
            break;
        case Reigon::OAM:
            return graphics.read(address);
            break;
        case Reigon::UNUSABLE:
            return 0xFF;
            break;
        case Reigon::IO:
            address -= IO_START;
            return io_ram.at(address);
            break;
        case Reigon::HIGH:
            address -= HIGH_START;
            return high_ram.at(address);
            break;
        case Reigon::IE:
            return IE_byte;
            break;
        
        default:
            throw std::runtime_error("ATTEMPTING TO ACCESS INVALID MEMORY ADDRESS");
    }
}
Bus::Reigon Bus::getReigon(uint16_t addr){
    if(addr >= ROM_O_START && addr <= ROM_O_END){
        return Reigon::ROMO;
    }
    else if(addr >= ROM_N_START && addr <= ROM_N_END){
        return Reigon::ROMN;
    }
    else if(addr >= VRAM_START && addr <= VRAM_END){
        return Reigon::VRAM;
    }
    else if(addr >= EXT_START && addr <= EXT_END){
        return Reigon::EXT;
    }
    else if(addr >= WORK_START && addr <= WORK_END){
        return Reigon::WRAM;
    }
    else if(addr >= ECHO_START && addr <= ECHO_END){
        return Reigon::ECHO;
    }
    else if(addr >= OAM_START && addr <= OAM_END){
        return Reigon::OAM;
    }
    else if(addr >= UNUSABLE_START && addr <= UNUSABLE_END){
        return Reigon::UNUSABLE;
    }
    else if(addr >= IO_START && addr <= IO_END){
        return Reigon::IO;
    }
    else if(addr >= HIGH_START && addr <= HIGH_END){
        return Reigon::HIGH;
    }
    else if(addr == IE){
            return Reigon::IE;
    }
    else throw std::runtime_error("INVALID RAM ADDRESS LOCATION");
}
void Bus::write(uint16_t address, uint8_t value){
    Reigon reig = getReigon(address);

    if(reig == Reigon::ROMO || reig == Reigon::ROMN){
        //do nothing for now, in the future pass to cartridge
    }
    if(reig == Reigon::VRAM){
        graphics.write(address, value);
    }
    if(reig == Reigon::EXT){
        cartridge.write(address, value);
    }
    if(reig == Reigon::WRAM){
        address -= WORK_START;
        work_ram.at(address) = value;
    }
    if(reig == Reigon::ECHO){
        address -= ECHO_START;
        work_ram.at(address) = value;
    }
    if(reig == Reigon::OAM){
        graphics.write(address, value);
    }
    if(reig == Reigon::UNUSABLE){
        //do nothing on purpose
    }
    if(reig == Reigon::IO){
        address -= IO_START;
        io_ram.at(address) = value;
    }
    if(reig == Reigon::HIGH){
        address -= HIGH_START;
        high_ram.at(address) = value;
    }
    if(reig == Reigon::IE){
        IE_byte = value;
    }
}