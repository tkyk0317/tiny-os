#include <stdint.h>

/**
 * Memory Mapped IO
 */
class MMIO {
public:
    /**
     * output Memory-Mapped IO
     */
    static inline void write(uint32_t reg, uint32_t data) { *reinterpret_cast<volatile uint32_t*>(reg) = data; }

    /**
     * input Memory-Mapped IO
     */
    static inline uint32_t read(uint32_t reg) { return *reinterpret_cast<volatile uint32_t*>(reg); }

private:
    MMIO();
};

