#include "cartridge.h"
#include "bus.h"
#include <iostream>
#include <format>
#include <string>
#include <exception>

// Phase 1.3 test: only checks Bus::read.
// It puts bytes straight into the Bus's arrays, then makes sure read() finds them
// at the right address. That way it doesn't depend on write() working yet.
//
// SETUP: add this line inside the Bus class in bus.h:
//
//     friend struct BusTest;
//
// Expects private arrays named vram, work_ram, oam, io_ram and high_ram.
//
// RUN:
//     ./gameboy roms/01-special.gb

static int passed = 0;
static int failed = 0;

static void check(const std::string& name, int got, int expected) {
    if (got == expected) {
        std::cout << "  PASS  " << name << "\n";
        passed++;
    } else {
        std::cout << std::format("  FAIL  {}  (expected 0x{:02X}, got 0x{:02X})\n", name, expected, got);
        failed++;
    }
}

struct BusTest {
    static void run(Cartridge& cart) {
        Bus bus(cart);

        std::cout << "\nROM\n";
        check("0x0000 matches cartridge", bus.read(0x0000), cart.read(0x0000));
        check("0x0100 matches cartridge", bus.read(0x0100), cart.read(0x0100));
        check("0x0148 matches cartridge", bus.read(0x0148), cart.read(0x0148));
        check("0x7FFF matches cartridge", bus.read(0x7FFF), cart.read(0x7FFF));

        std::cout << "\nVRAM\n";
        bus.vram[0x0000] = 0x11;
        bus.vram[0x1FFF] = 0x12;
        check("0x8000 is first VRAM byte", bus.read(0x8000), 0x11);
        check("0x9FFF is last VRAM byte", bus.read(0x9FFF), 0x12);

        std::cout << "\nWork RAM\n";
        bus.work_ram[0x0000] = 0x21;
        bus.work_ram[0x0005] = 0x22;
        bus.work_ram[0x1FFF] = 0x23;
        check("0xC000 is first work RAM byte", bus.read(0xC000), 0x21);
        check("0xC005 reads work RAM", bus.read(0xC005), 0x22);
        check("0xDFFF is last work RAM byte", bus.read(0xDFFF), 0x23);

        std::cout << "\nEcho RAM\n";
        check("0xE000 mirrors 0xC000", bus.read(0xE000), 0x21);
        check("0xE005 mirrors 0xC005", bus.read(0xE005), 0x22);
        bus.work_ram[0x1DFF] = 0x24;
        check("0xFDFF mirrors 0xDDFF (last echo byte)", bus.read(0xFDFF), 0x24);

        std::cout << "\nOAM\n";
        bus.oam[0x00] = 0x31;
        bus.oam[0x9F] = 0x32;
        check("0xFE00 is first OAM byte", bus.read(0xFE00), 0x31);
        check("0xFE9F is last OAM byte", bus.read(0xFE9F), 0x32);

        std::cout << "\nUnusable\n";
        check("0xFEA0 reads 0xFF", bus.read(0xFEA0), 0xFF);
        check("0xFEFF reads 0xFF", bus.read(0xFEFF), 0xFF);

        std::cout << "\nI/O\n";
        bus.io_ram[0x00] = 0x41;
        bus.io_ram[0x7F] = 0x42;
        check("0xFF00 is first I/O byte", bus.read(0xFF00), 0x41);
        check("0xFF7F is last I/O byte", bus.read(0xFF7F), 0x42);

        std::cout << "\nHigh RAM\n";
        bus.high_ram[0x00] = 0x51;
        bus.high_ram[0x7E] = 0x52;
        check("0xFF80 is first high RAM byte", bus.read(0xFF80), 0x51);
        check("0xFFFE is last high RAM byte", bus.read(0xFFFE), 0x52);

        std::cout << "\nIE\n";
        check("0xFFFF starts at 0", bus.read(0xFFFF), 0x00);
        check("0xFFFF is not the last high RAM byte", bus.read(0xFFFF) != 0x52, true);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom.gb>\n";
        return 1;
    }

    try {
        Cartridge cart;
        cart.load(argv[1]);
        BusTest::run(cart);
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 1;
    }

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}