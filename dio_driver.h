#ifndef DIO_DRIVER_H
#define DIO_DRIVER_H

#include <stdbool.h>

bool init_iopl();

// void enter_pnp_mode();
// void exit_pnp_mode();
void initmod_all_out(void);
void initmod_all_input(void);
void initmod_8in_8out(void);
bool set_pin_level(unsigned int pinIndex, bool state_on);
bool read_pin_level(unsigned int pinIndex);

#endif /* DIO_DRIVER_H */
