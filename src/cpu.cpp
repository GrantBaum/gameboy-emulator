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
bool getZ(){
    if((regF & z_MASK) != 0){
        return true;
    }
    else return false;
}
bool getH(){
    if((regF & h_MASK) != 0){
        return true;
    }
    else return false;
}
bool getN(){
    if((regF & n_MASK) != 0){
        return true;
    }
    else return false;
}
bool getC(){
    if((regF & c_MASK) != 0){
        return true;
    }
    else return false;
}
void setZ(bool val){
    if(val){
        regF = regF | z_MASK;
    }
    else regF = regF & (~z_MASK);
}
void setH(bool val){
    if(val){
        regF = regF | h_MASK;
    }
    else regF = regF & (~h_MASK);
}
void setN(bool val){
    if(val){
        regF = regF | n_MASK;
    }
    else regF = regF & (~n_MASK);
}
void setC(bool val){
    if(val){
        regF = regF | c_MASK;
    }
    else regF = regF & (~c_MASK);
}
