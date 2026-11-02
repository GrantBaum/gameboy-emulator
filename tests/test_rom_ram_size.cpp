#include "cartridge.h"
#include <iostream>
#include <format>
#include <string>
#include <exception>

// Phase 1.2 test: only checks that the size codes turn into the right byte counts.
//
// SETUP: add this line inside the Cartridge class in cartridge.h so the test can see private stuff:
//
//     friend struct CartridgeTest;
//
// Expects these private helpers:
//     size_t romBytes(uint8_t code);
//     size_t ramBytes(uint8_t code);
// and a private std::vector<uint8_t> named ext_ram.
//
// RUN:
//     ./gameboy                       (just the table checks)
//     ./gameboy roms/01-special.gb    (table checks plus a real ROM)

static int passed = 0;
static int failed = 0;

static void check(const std::string& name, size_t got, size_t expected) {
    if (got == expected) {
        std::cout << "  PASS  " << name << "\n";
        passed++;
    } else {
        std::cout << "  FAIL  " << name << "  (expected " << expected << ", got " << got << ")\n";
        failed++;
    }
}

struct CartridgeTest {
    static void romTable() {
        std::cout << "\nROM size codes\n";
        Cartridge c;
        check("0x00 is 32 KB",  c.romBytes(0x00), 32768);
        check("0x01 is 64 KB",  c.romBytes(0x01), 65536);
        check("0x02 is 128 KB", c.romBytes(0x02), 131072);
        check("0x03 is 256 KB", c.romBytes(0x03), 262144);
        check("0x04 is 512 KB", c.romBytes(0x04), 524288);
        check("0x05 is 1 MB",   c.romBytes(0x05), 1048576);
        check("0x06 is 2 MB",   c.romBytes(0x06), 2097152);
        check("0x07 is 4 MB",   c.romBytes(0x07), 4194304);
        check("0x08 is 8 MB",   c.romBytes(0x08), 8388608);
    }

    static void ramTable() {
        std::cout << "\nRAM size codes\n";
        Cartridge c;
        check("0x00 is no RAM", c.ramBytes(0x00), 0);
        check("0x02 is 8 KB",   c.ramBytes(0x02), 8192);
        check("0x03 is 32 KB",  c.ramBytes(0x03), 32768);
        check("0x04 is 128 KB", c.ramBytes(0x04), 131072);
        check("0x05 is 64 KB",  c.ramBytes(0x05), 65536);
    }

    static void realRom(const std::string& path) {
        std::cout << "\nReal ROM: " << path << "\n";
        Cartridge c;
        c.load(path);

        uint8_t romCode = c.read(0x0148);
        uint8_t ramCode = c.read(0x0149);
        std::cout << std::format("  INFO  ROM code 0x{:02X}, RAM code 0x{:02X}\n", romCode, ramCode);

        check("header ROM size matches the file", c.romBytes(romCode), c.rom.size());
        check("ext_ram was resized to match the header", c.ext_ram.size(), c.ramBytes(ramCode));
    }
};

int main(int argc, char* argv[]) {
    try {
        CartridgeTest::romTable();
        CartridgeTest::ramTable();
        if (argc >= 2) {
            CartridgeTest::realRom(argv[1]);
        }
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 1;
    }

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}