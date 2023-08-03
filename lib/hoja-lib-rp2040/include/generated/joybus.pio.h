// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------ //
// joybus //
// ------ //

#define joybus_wrap_target 0
#define joybus_wrap 24

#define joybus_offset_joybusin 0u
#define joybus_offset_joybusout 12u

static const uint16_t joybus_program_instructions[] = {
            //     .wrap_target
    0xe080, //  0: set    pindirs, 0                 
    0xe027, //  1: set    x, 7                       
    0x20a0, //  2: wait   1 pin, 0                   
    0x2720, //  3: wait   0 pin, 0               [7] 
    0xa742, //  4: nop                           [7] 
    0xa342, //  5: nop                           [3] 
    0x4001, //  6: in     pins, 1                    
    0x20a0, //  7: wait   1 pin, 0                   
    0x0043, //  8: jmp    x--, 3                     
    0x8020, //  9: push   block                      
    0xc000, // 10: irq    nowait 0                   
    0x0000, // 11: jmp    0                          
    0xe081, // 12: set    pindirs, 1                 
    0x7f21, // 13: out    x, 1            side 1 [7] 
    0x1733, // 14: jmp    !x, 19          side 0 [7] 
    0xbf42, // 15: nop                    side 1 [7] 
    0xa042, // 16: nop                               
    0x1fed, // 17: jmp    !osre, 13       side 1 [7] 
    0x0016, // 18: jmp    22                         
    0xb742, // 19: nop                    side 0 [7] 
    0xa042, // 20: nop                               
    0x17ed, // 21: jmp    !osre, 13       side 0 [7] 
    0xbf42, // 22: nop                    side 1 [7] 
    0xb742, // 23: nop                    side 0 [7] 
    0x000d, // 24: jmp    13                         
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program joybus_program = {
    .instructions = joybus_program_instructions,
    .length = 25,
    .origin = -1,
};

static inline pio_sm_config joybus_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + joybus_wrap_target, offset + joybus_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

static inline void joybus_set_in(bool in, PIO pio, uint sm, uint offset, pio_sm_config *c)
{
    // Disable SM
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_init(pio, sm, offset + (in ? joybus_offset_joybusin : joybus_offset_joybusout), c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void joybus_program_init(PIO pio, uint sm, uint offset, uint pin, pio_sm_config *c) {
    *c = joybus_program_get_default_config(offset);
    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);
    gpio_pull_up(pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    // Must run 12800000hz
    float div = clock_get_hz(clk_sys) / (8000000 );
    sm_config_set_clkdiv(c, div);
    // Set sideset pin
    sm_config_set_sideset_pins(c, pin);
    sm_config_set_out_shift(c, true, true, 8);
    sm_config_set_fifo_join(c, PIO_FIFO_JOIN_TX);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, c);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

#endif

