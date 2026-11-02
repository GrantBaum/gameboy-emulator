#pragma once
#include "address.h"
#include "bus.h"

class Cpu{

    public:
        //constructor
        Cpu(Bus& bus);

    private:
        //reference to the bus
        Bus& memory;
        
        //special registers
        uint16_t PC; //program counter
        uint16_t SP; //stack pointer
        
        //normal registers
        uint8_t regA;
        uint8_t regF; //this one cannot be accessed on its own
        uint8_t regB;
        uint8_t regC;
        uint8_t regD;
        uint8_t regE;
        uint8_t regH;
        uint8_t regL;

        //getters for the 16 bit registers
        uint16_t getAF() const{return (regA << 8) | regF};
        uint16_t getBC() const{return (regB << 8) | regC};
        uint16_t getDE() const{return (regD << 8) | regE};
        uint16_t getHL() const{return (regH << 8) | regL};

        //setters for the 16 bit registers
        void setAF(uint16_t val);
        void setBC(uint16_t val);
        void setDE(uint16_t val);
        void setHL(uint16_t val);

        //getters and setters for flags
        bool getZ();
        bool getH();
        bool getN();
        bool getC();
        void setZ(bool val);
        void setH(bool val);
        void setN(bool val);
        void setC(bool val);

};