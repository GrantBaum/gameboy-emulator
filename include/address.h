#include <cstdint>

//address constants
static const uint16_t ROM_O_START = 0x0000;
static const uint16_t ROM_O_END = 0x3FFF;
static const uint16_t ROM_N_START = 0x4000;
static const uint16_t ROM_N_END = 0x7FFF;
static const uint16_t VRAM_START = 0x8000;
static const uint16_t VRAM_END = 0x9FFF;
static const uint16_t EXT_START = 0xA000;
static const uint16_t EXT_END = 0xBFFF;
static const uint16_t WORK_START = 0xC000;
static const uint16_t WORK_END = 0xDFFF;
static const uint16_t ECHO_START = 0xE000;
static const uint16_t ECHO_END = 0xFDFF;
static const uint16_t OAM_START = 0xFE00;
static const uint16_t OAM_END = 0xFE9F;
static const uint16_t IO_START = 0xFF00;
static const uint16_t IO_END = 0xFF7F;
static const uint16_t HIGH_START = 0xFF80;
static const uint16_t HIGH_END = 0xFFFE;

//unusable range
static const uint16_t UNUSABLE_START = 0xFEA0;
static const uint16_t UNUSABLE_END = 0xFEFF;

//IE
static const uint16_t IE = 0xFFFF;

//header locations
static const uint16_t romCode = 0x0148;
static const uint16_t ramCode = 0x0149;