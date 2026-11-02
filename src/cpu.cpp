#include "cpu.h"

Cpu::Cpu(Bus& bus) : memory(bus){}

//setters for 16 bit registers
void Cpu::setAF(uint16_t val){
    regA = (val & HI_MASK) >> 8;
    regF = 0; //clears low bits
}
void Cpu::setBC(uint16_t val){
    regB = (val & HI_MASK) >> 8;
    regC = val & LO_MASK;
}
void Cpu::setDE(uint16_t val){
    regD = (val & HI_MASK) >> 8;
    regE = val & LO_MASK;
}
void Cpu::setHL(uint16_t val){
    regH = (val & HI_MASK) >> 8;
    regL = val & LO_MASK;
}

//getters and setters for flags
bool Cpu::getZ(){
    return (regF & z_MASK) != 0;
}
bool Cpu::getH(){
    return (regF & h_MASK) != 0;
}
bool Cpu::getN(){
    return (regF & n_MASK) != 0;
}
bool Cpu::getC(){
    return (regF & c_MASK) != 0;
}
void Cpu::setZ(bool val){
    if(val){
        regF = regF | z_MASK;
    }
    else regF = regF & (~z_MASK);
}
void Cpu::setH(bool val){
    if(val){
        regF = regF | h_MASK;
    }
    else regF = regF & (~h_MASK);
}
void Cpu::setN(bool val){
    if(val){
        regF = regF | n_MASK;
    }
    else regF = regF & (~n_MASK);
}
void Cpu::setC(bool val){
    if(val){
        regF = regF | c_MASK;
    }
    else regF = regF & (~c_MASK);
}
