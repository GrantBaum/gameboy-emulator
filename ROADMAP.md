# Game Boy Emulator Roadmap

Every step says what to read, what to build and how you know it works. Do them in order. Don't skip ahead.

Pan Docs files are in your offline copy at `~/pandocs/src/`. Open one in VS Code and press Ctrl+Shift+V for the nice view.

---

## Big picture resources (keep these open the whole time)

| What | Where | Use it for |
|------|-------|------------|
| Pan Docs | `~/pandocs/src/` | The main hardware reference |
| Opcode table | gbdev.io/gb-opcodes/optables/ | Every instruction on one page, with cycles and flags |
| RGBDS gbz80 page | rgbds.gbdev.io/docs/gbz80.7 | Plain English for every instruction, including exactly which flags it changes |
| The Ultimate Game Boy Talk | search "Ultimate Game Boy Talk Michael Steil" | A one hour video that explains the whole machine. Watch it once early on. |
| Game Boy Complete Technical Reference | gekkio.fi/files/gb-docs/gbctr.pdf | Deep detail when Pan Docs isn't enough. Not for beginners. Save it for later. |
| gbdev Discord | linked from gbdev.io | Real people who have built this before |

---

# Phase 1: Cartridge and Bus

## 1.1 Load a ROM and read the header

**Read**
- `The_Cartridge_Header.md`: title, type, ROM size, RAM size and header checksum
- `nombc.md`: the simplest cartridge, no bank chip

**Build**
- [x] `Cartridge` with a ROM vector and `load(const std::string& path)`
- [x] Throw if the file won't open or is smaller than `0x150` bytes
- [x] `read(address)` returns the ROM byte at that address

**Done when:** main prints the title, type, size codes and a passing checksum.

**Check yourself**
- Why does the file need to be at least `0x150` bytes?
- What cartridge type code does a plain 32 KB game with no chip use?

## 1.2 Decode the size codes

**Read:** the ROM size and RAM size tables in `The_Cartridge_Header.md`

**Build**
- [x] A helper that turns the ROM size code into bytes
- [x] A helper that turns the RAM size code into bytes
- [x] Size the external RAM vector from the RAM size code
- [x] Warn if the file size doesn't match what the header says

**Done when:** main prints real sizes like "32 KB ROM, no RAM."

**Check yourself:** ROM size code `0x00` means 32 KB. What's the pattern for the bigger codes?

## 1.3 The Bus

**Read**
- `Memory_Map.md` again, slowly
- `Hardware_Reg_List.md`: just skim, to see what lives in `FF00` to `FF7F`

**Build**
- [x] Named constants for every range's start and end
- [x] Arrays: VRAM, work RAM, OAM, I/O and high RAM, plus a `uint8_t` for IE
- [x] A reference to the Cartridge, set in the constructor
- [x] A private region enum and a private helper that maps an address to its region
- [x] Public `read(address)` and `write(address, value)` that switch on the region
- [x] Echo RAM goes to work RAM minus `0x2000`
- [x] Unusable area reads `0xFF` and ignores writes
- [x] Writes to ROM are ignored for now

**Done when:** these all pass in a test main:
- Read `0x0100` through the bus matches the ROM file
- Write `0xC005`, read it back
- Read `0xE005` matches `0xC005`
- Write and read `0xFF80`
- Write and read `0xFFFF`, and make sure it did NOT change `0xFFFE`
- Read `0xFEA0` gives `0xFF`
- Edge checks: `0x7FFF` and `0x8000`, `0xDFFF` and `0xE000`, `0xFE9F` and `0xFEA0`

**C++ to know:** [std::array](https://en.cppreference.com/w/cpp/container/array), [enum class](https://en.cppreference.com/w/cpp/language/enum), [reference members and initializer lists](https://en.cppreference.com/w/cpp/language/constructor)

**Commit.**

## 1.4 Sketch the other headers

**Build**
- [ ] `cpu.h`, `ppu.h`, `timer.h` and `joypad.h` with public methods only and a one line comment each
- [ ] Most chips will want `read(address)`, `write(address, value)` and `step(cycles)`

**Done when:** you can explain how a byte gets from the cartridge into a CPU register, using only your headers.

---

# Phase 2: The CPU

This is the biggest phase. It's where most of your time goes.

## 2.1 Learn the CPU before you code it

**Read**
- `CPU_Registers_and_Flags.md`
- `CPU_Instruction_Set.md`
- `CPU_Comparison_with_Z80.md`: skim. It explains why some docs mention the Z80.
- RGBDS gbz80 page: read the intro and a few instructions to see the format

**Watch:** The Ultimate Game Boy Talk, at least the CPU part

**Check yourself**
- What are the 8 registers and how do they pair up?
- What do Z, N, H and C mean?
- What are the low 4 bits of F always set to?
- What does "little endian" mean, and how is it different from CHIP-8?

## 2.2 Registers and flags

**Build**
- [ ] A, F, B, C, D, E, H and L as `uint8_t`
- [ ] SP and PC as `uint16_t`
- [ ] Get and set for AF, BC, DE and HL as 16 bit pairs
- [ ] Setting AF always clears the low 4 bits of F
- [ ] Get and set for each flag
- [ ] A reference to the Bus

**Done when:** unit tests pass. Set BC to `0x1234` and check B is `0x12` and C is `0x34`. Set AF to `0xFFFF` and check F is `0xF0`.

## 2.3 Starting values

**Read:** `Power_Up_Sequence.md`, the part about register values after the boot ROM

**Build**
- [ ] Constructor sets every register to its after boot value for the original Game Boy (DMG)
- [ ] PC starts at `0x0100`

**Check yourself:** why does PC start at `0x0100` and not `0x0000`?

## 2.4 Fetch helpers

**Build**
- [ ] `fetch8()`: read the byte at PC, then move PC forward 1
- [ ] `fetch16()`: two fetch8 calls, combined **low byte first**
- [ ] `push16()` and `pop16()` for the stack. SP counts **down** when you push.

**Done when:** unit test. Put `0x34` then `0x12` in work RAM, point PC there, and fetch16 gives `0x1234`.

**Check yourself:** when you push, does SP go up or down? Which byte goes in first?

## 2.5 The step function and cycles

**Read:** the opcode table. Hover a few instructions and look at the cycle counts.

**Build**
- [ ] `step()` fetches one opcode, runs it and returns how many cycles it took
- [ ] An unknown opcode throws with the opcode and PC, like CHIP-8

**Decide how to decode:**
- **Giant switch:** 256 cases plus 256 more for `0xCB`. Simple. Lots of typing.
- **Bit patterns:** read "Decoding Z80 Opcodes" at z80.info/decoding.htm. Groups whole families of instructions into one case. Less code, more thinking.

## 2.6 Build instructions in groups

For each group, read it in the RGBDS page and the opcode table first. Write unit tests for each before moving on.

- [ ] **Do nothing and jumps:** `NOP`, `JP`, `JP cc`, `JR`, `JR cc`, `JP HL`
- [ ] **Calls:** `CALL`, `CALL cc`, `RET`, `RET cc`, `RETI`, `RST`
- [ ] **8 bit loads:** every `LD r, r`, `LD r, n`, `LD r, (HL)`, `LD (HL), r`
- [ ] **Special loads:** `LD A, (BC)`, `LD A, (DE)`, `LD (HL+), A`, `LD (HL-), A` and friends
- [ ] **High page loads:** `LDH (n), A`, `LDH A, (n)`, `LD (C), A`, `LD A, (C)`
- [ ] **16 bit loads:** `LD rr, nn`, `LD SP, HL`, `LD (nn), SP`, `LD HL, SP+e`
- [ ] **Stack:** `PUSH` and `POP` for every pair. `POP AF` must clear the low 4 bits of F.
- [ ] **8 bit math:** `ADD`, `ADC`, `SUB`, `SBC`, `AND`, `OR`, `XOR`, `CP`
- [ ] **Increment and decrement:** `INC r`, `DEC r`, `INC rr`, `DEC rr`. The 16 bit ones change no flags.
- [ ] **16 bit math:** `ADD HL, rr`, `ADD SP, e`
- [ ] **Rotates on A:** `RLCA`, `RRCA`, `RLA`, `RRA`
- [ ] **Flag tricks:** `CPL`, `SCF`, `CCF`
- [ ] **The CB prefix:** `RLC`, `RRC`, `RL`, `RR`, `SLA`, `SRA`, `SRL`, `SWAP`, `BIT`, `RES`, `SET`
- [ ] **Control:** `DI`, `EI`, `HALT`, `STOP`
- [ ] **Last:** `DAA`

**Watch out**
- **Half carry.** Set when bit 3 carries into bit 4 for 8 bit math. For `ADD HL, rr` it's bit 11 into bit 12. Test it carefully.
- **The `cc` versions** take a different number of cycles depending on whether the jump happens. The opcode table shows both.
- **`(HL)` means the byte in memory at HL,** not HL itself. Those versions also take more cycles.
- **`DAA`** is weird. Look up "DAA Game Boy" on the gbdev wiki and read it slowly.
- There are 11 opcodes that don't exist, like `0xD3`. Throw on those.

**C++ to know:** bitwise ops and shifts, which you already know. [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) or function pointers if you want a lookup table instead of a switch.

## 2.7 Serial output for tests

**Read:** `Serial_Data_Transfer_(Link_Cable).md`. Only the first part.

**Build**
- [ ] When the bus gets a write of `0x81` to `0xFF02`, print the byte stored at `0xFF01` as a character

**Why:** Blargg's tests print "Passed" or "Failed" this way. You get test results before you have a screen.

## 2.8 Gameboy Doctor

**Read:** the Gameboy Doctor README at github.com/robert/gameboy-doctor. All of it.

**Build**
- [ ] A log mode that prints the registers in Doctor's exact format before each instruction
- [ ] While logging, LY at `0xFF44` always reads `0x90`. The README explains why.

**Done when:** you can run a test ROM, save the log, feed it to Doctor and read its answer.

## 2.9 Pass Blargg's CPU tests

**Get:** the `cpu_instrs/individual/` ROMs from github.com/retrio/gb-test-roms

**Do:** run them one at a time, in order.

- [ ] 01-special
- [ ] 02-interrupts (comes back in Phase 3, skip for now if it hangs)
- [ ] 03-op sp,hl
- [ ] 04-op r,imm
- [ ] 05-op rp
- [ ] 06-ld r,r
- [ ] 07-jr,jp,call,ret,rst
- [ ] 08-misc instrs
- [ ] 09-op r,r
- [ ] 10-bit ops
- [ ] 11-op a,(hl)

**For every failure:** log it, run Doctor, find the first bad instruction, reread its docs, write a unit test for that exact case and fix it.

**Done when:** every one prints "Passed." **Huge milestone. Commit and screenshot it.**

---

# Phase 3: Timers and interrupts

## 3.1 Interrupts

**Read**
- `Interrupts.md`
- `Interrupt_Sources.md`
- `halt.md`

**Build**
- [ ] IME flag inside the CPU
- [ ] IF at `0xFF0F` in the bus. Its top 3 bits always read as 1.
- [ ] Before each instruction: if IME is on and `IE & IF` isn't 0, handle the lowest set bit
- [ ] Handling one: turn IME off, clear that IF bit, push PC, jump to its address and take 20 cycles
- [ ] `EI` turns IME on after the NEXT instruction, not right away
- [ ] `RETI` returns and turns IME on right away
- [ ] `HALT` sleeps until `IE & IF` isn't 0, even if IME is off

**Done when:** Blargg 02-interrupts passes.

**Check yourself**
- What are the 5 interrupts and their addresses?
- If two are waiting, which goes first?
- What does HALT do when IME is off but an interrupt is waiting?

## 3.2 The timer

**Read**
- `Timer_and_Divider_Registers.md`
- `Timer_Obscure_Behaviour.md`: skim it now, come back when tests fail

**Build**
- [ ] A `Timer` class with DIV, TIMA, TMA and TAC
- [ ] `step(cycles)` moves the timer forward
- [ ] DIV goes up once every 256 cycles. Any write to it sets it to 0.
- [ ] TIMA goes up at the speed TAC picks, if TAC turns it on
- [ ] When TIMA overflows, it reloads from TMA and requests the Timer interrupt
- [ ] The bus sends `FF04` to `FF07` to the Timer

**Done when:** a timer test passes. Try `instr_timing` from Blargg's set, which needs a working timer.

## 3.3 Wire the loop

**Build**
- [ ] A `GameBoy` class that owns the Cartridge, Bus, CPU and Timer
- [ ] `runFrame()`: keep calling CPU step and Timer step with the same cycles until 70,224 cycles have passed

**Done when:** all of cpu_instrs passes through `GameBoy` instead of a test main.

---

# Phase 4: Graphics

## 4.1 Learn how the screen works first

**Read, in this order**
- `Graphics.md`: the overview
- `Tile_Data.md`
- `Tile_Maps.md`
- `Palettes.md`
- `Scrolling.md`
- `LCDC.md`
- `STAT.md`
- `Rendering.md`

**Watch:** the graphics part of The Ultimate Game Boy Talk

**Do on paper:** decode one tile by hand. Take 16 bytes from Pan Docs' example and draw the 8 by 8 picture with 4 shades.

**Check yourself**
- How many bytes is one tile? How many bits per pixel?
- What do the two tile data modes `0x8000` and `0x8800` mean?
- How many tiles fit across the screen? How many across the whole background map?
- What are the 4 PPU modes and how long is each line?

## 4.2 PPU timing without drawing

**Build**
- [ ] `PPU` class. Move VRAM and OAM here from the bus.
- [ ] LCD registers: LCDC, STAT, SCY, SCX, LY, LYC, BGP, OBP0, OBP1, WY and WX
- [ ] `step(cycles)` walks through the modes: 80 cycles of mode 2, about 172 of mode 3, the rest of 456 as mode 0
- [ ] LY goes up each line. After 144 lines, 10 lines of mode 1 (VBlank), then back to 0.
- [ ] Request the VBlank interrupt when LY hits 144
- [ ] STAT shows the mode and the LY equals LYC flag, and can fire the LCD interrupt
- [ ] When LCDC bit 7 is off, the screen is off. LY stays 0.
- [ ] The bus sends VRAM, OAM and `FF40` to `FF4B` to the PPU

**Done when:** you can print LY every frame and it counts 0 to 153 over and over.

## 4.3 Draw the background

**Build**
- [ ] A 160 by 144 screen buffer of colors 0 to 3, inside the PPU
- [ ] When a line enters HBlank, draw that whole line:
  - For each of the 160 pixels, add the scroll values to find the spot in the 256 by 256 background
  - Look up which tile is there in the tile map
  - Look up that tile's pixel in tile data
  - Run it through the BGP palette
- [ ] Respect LCDC bits for which tile map and which tile data mode

**Display it:** main.cpp draws the buffer with SDL at 160 by 144, scaled up 3 or 4 times. Pick 4 grays you like.

**Done when:** a test ROM or Tetris shows its background, even if sprites are missing.

## 4.4 Window

**Read:** `Window.md`

**Build**
- [ ] When LCDC turns it on and the line is past WY, draw the window over the background, starting at WX minus 7
- [ ] The window keeps its own line counter. It only moves forward on lines where the window was drawn.

## 4.5 Sprites

**Read**
- `OAM.md`
- `OAM_DMA_Transfer.md`

**Build**
- [ ] During mode 2, find up to 10 sprites on the current line
- [ ] Draw them with X flip, Y flip, priority and the right palette
- [ ] 8 by 16 sprite mode from LCDC
- [ ] Color 0 in a sprite is see through
- [ ] **OAM DMA:** a write to `0xFF46` copies 160 bytes into OAM. Almost every game uses this.

**Done when:** **dmg-acid2** from github.com/mattcurrie/dmg-acid2 shows a perfect face. Compare against the reference picture in its README.

**Commit and screenshot.**

---

# Phase 5: Timing and main loop

**Build**
- [ ] main.cpp runs one frame, draws it and waits out the rest of about 16.74 ms
- [ ] 70,224 cycles per frame, at 4,194,304 cycles per second
- [ ] Optional: a fast forward key that skips the wait

**Done when:** games run at the right speed.

---

# Phase 6: Input

**Read:** `Joypad_Input.md`

**Build**
- [ ] A `Joypad` class with 8 button states
- [ ] `FF00`: the game writes bits 4 and 5 to choose arrows or buttons, then reads the low 4 bits
- [ ] **0 means pressed.** Backwards from what you'd expect.
- [ ] Request the Joypad interrupt when a button goes down
- [ ] main maps keyboard keys to the 8 buttons

**Done when:** you can play Tetris.

---

# Phase 7: Bank switching chips

**Read**
- `MBCs.md`
- `MBC1.md`
- `MBC3.md`
- `MBC5.md` (later)

**Build**
- [ ] Decide on a design: a switch on cartridge type, or a base `MBC` class with versions for each chip
- [ ] **MBC1:** writes to `0x2000` to `0x3FFF` pick the ROM bank. Writes to `0x0000` to `0x1FFF` turn external RAM on and off.
- [ ] **MBC3:** like MBC1 plus a real time clock
- [ ] Save external RAM to a `.sav` file when you quit and load it when you start, for games with a battery

**C++ to know if you go with classes:** [virtual functions](https://en.cppreference.com/w/cpp/language/virtual), [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)

**Done when:** an MBC1 game gets past its title screen, and a save survives restarting.

---

# Phase 8: Sound (optional)

**Read**
- `Audio.md`
- `Audio_Registers.md`
- `Audio_details.md`

**Build, one channel at a time**
- [ ] Channel 2: square wave. The simplest. Start here.
- [ ] Channel 1: square wave with a sweep
- [ ] Channel 3: custom wave from wave RAM
- [ ] Channel 4: noise
- [ ] Mix them and feed SDL's audio callback, like your CHIP-8 beep

**Done when:** game music sounds right. This one is hard. Lots of good emulators skip it.

---

# Phase 9: Extra tests and polish

**Extra test ROMs**
- [ ] Blargg `instr_timing` and `mem_timing`
- [ ] Mooneye test suite at github.com/Gekkio/mooneye-test-suite. Very fussy. Pass what you can.

**Polish**
- [ ] ROM path from the command line, which you already have
- [ ] Clean errors and crash messages
- [ ] Update the README with screenshots

---

# Phase 10: Hardware (your Pi handheld)

- [ ] Learn GPIO on a breadboard: LEDs and buttons first
- [ ] Get an SPI screen showing a test pattern from a small separate program
- [ ] Swap SDL in main for the screen and buttons
- [ ] Your chip classes don't change

---

# Rules

1. **One step at a time.** Finish it, test it, commit it.
2. **Read the docs before writing code.** Then read them again when it breaks.
3. **Test ROMs are your answer key.** Doctor for the CPU, dmg-acid2 for graphics.
4. **Write a unit test for every bug you fix.** Then it can't come back.
5. **When stuck for real:** bring the exact failing instruction or test, plus your code for it.