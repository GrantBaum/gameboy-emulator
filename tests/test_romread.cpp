#include "cartridge.h"
#include <iostream>
#include <format>
#include <exception>

//Phase 1 test: load a ROM and print what's in its header
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom.gb>\n";
        return 1;
    }

    try {
        Cartridge cart;
        cart.load(argv[1]);

        //title: 0x0134 to 0x0143, stop early at a zero byte
        std::string title;
        for (uint16_t addr = 0x0134; addr <= 0x0143; addr++) {
            uint8_t c = cart.read(addr);
            if (c == 0) break;
            title += static_cast<char>(c);
        }
        std::cout << "Title:          " << title << "\n";

        //cartridge type, ROM size and RAM size codes
        std::cout << std::format("Cartridge type: 0x{:02X}\n", cart.read(0x0147));
        std::cout << std::format("ROM size code:  0x{:02X}\n", cart.read(0x0148));
        std::cout << std::format("RAM size code:  0x{:02X}\n", cart.read(0x0149));

        //header checksum: the game stores it at 0x014D, we work it out ourselves and compare
        uint8_t checksum = 0;
        for (uint16_t addr = 0x0134; addr <= 0x014C; addr++) {
            checksum = checksum - cart.read(addr) - 1;
        }
        uint8_t stored = cart.read(0x014D);
        std::cout << std::format("Checksum:       0x{:02X} (header says 0x{:02X}) {}\n",
                                 checksum, stored, checksum == stored ? "PASS" : "FAIL");
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 1;
    }

    return 0;
}