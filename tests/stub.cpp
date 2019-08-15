#include <cstdint>

extern "C" {
    // asm関数群
    void enable_irq() {}
    void disable_irq() {}
    void __switch(uint64_t**, uint64_t**) {}
    void __switch_el0(uint64_t, uint64_t, uint64_t) {}
    uint64_t get_sp();
    uint64_t get_el();
}
