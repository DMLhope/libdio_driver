#ifndef DIO_DRIVER_H
#define DIO_DRIVER_H

void initmod_all_out(void);
void initmod_all_input(void);
void initmod_8in_8out(void);

void set_00_level(bool state_on);
void set_01_level(bool state_on);
void set_02_level(bool state_on);
void set_03_level(bool state_on);
void set_04_level(bool state_on);
void set_05_level(bool state_on);
void set_06_level(bool state_on);
void set_07_level(bool state_on);
void set_10_level(bool state_on);
void set_11_level(bool state_on);
void set_12_level(bool state_on);
void set_13_level(bool state_on);
void set_14_level(bool state_on);
void set_15_level(bool state_on);
void set_16_level(bool state_on);
void set_17_level(bool state_on);

int read_00_value();
int read_01_value();
int read_02_value();
int read_03_value();
int read_04_value();
int read_05_value();
int read_06_value();
int read_07_value();
int read_10_value();
int read_11_value();
int read_12_value();
int read_13_value();
int read_14_value();
int read_15_value();
int read_16_value();
int read_17_value();

#endif /* DIO_DRIVER_H */
