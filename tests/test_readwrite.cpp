#include "cartridge.h"
#include "bus.h"
#include <iostream>
#include <format>

//Phase 1.3 test: write a byte, read it back
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom.gb>\n";
        return 1;
    }

    Cartridge cart;
    cart.load(argv[1]);
    Bus bus(cart);

    int failed = 0;

    //write then read the same spot
    uint16_t addresses[] = { 0x8000, 0x9FFF, 0xC000, 0xDFFF, 0xFE00, 0xFE9F,
                             0xFF00, 0xFF7F, 0xFF80, 0xFFFE, 0xFFFF };
    for (uint16_t addr : addresses) {
        bus.write(addr, 0x42);
        uint8_t got = bus.read(addr);
        bool ok = (got == 0x42);
        if (!ok) failed++;
        std::cout << std::format("{}  0x{:04X}  wrote 0x42, read 0x{:02X}\n",
                                 ok ? "PASS" : "FAIL", addr, got);
    }

    //echo: write one side, read the other
    bus.write(0xE005, 0x77);
    bool echo1 = bus.read(0xC005) == 0x77;
    bus.write(0xC010, 0x88);
    bool echo2 = bus.read(0xE010) == 0x88;
    if (!echo1) failed++;
    if (!echo2) failed++;
    std::cout << (echo1 ? "PASS" : "FAIL") << "  write 0xE005, read 0xC005\n";
    std::cout << (echo2 ? "PASS" : "FAIL") << "  write 0xC010, read 0xE010\n";

    //unusable stays 0xFF
    bus.write(0xFEA0, 0x42);
    bool unusable = bus.read(0xFEA0) == 0xFF;
    if (!unusable) failed++;
    std::cout << (unusable ? "PASS" : "FAIL") << "  0xFEA0 still reads 0xFF\n";

    //ROM can't be changed
    uint8_t before = bus.read(0x0100);
    bus.write(0x0100, before + 1);
    bool romSame = bus.read(0x0100) == before;
    if (!romSame) failed++;
    std::cout << (romSame ? "PASS" : "FAIL") << "  writing ROM doesn't change it\n";

    std::cout << "\n" << (failed == 0 ? "All passed" : std::format("{} failed", failed)) << "\n";
    return failed == 0 ? 0 : 1;
}