#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <stdbool.h>

#define outportb(port, data) outb(data, port)
#define inportb(port) inb(port)

#define GP1_GPIO_BASE 0xA06 // GP1 ADDR
#define GP2_GPIO_BASE 0xA07 // GP2 ADDR

// Define pin configuration as an array of base and offset
#define NUM_PINS 16
const struct {
    unsigned short base;
    unsigned char offset;
} pinConfig[NUM_PINS] = {
    {0xA06, 0},  // DI1
    {0xA06, 1},  // DI2
    {0xA06, 2},  // DI3
    {0xA06, 3},  // DI4
    {0xA06, 4},  // DI5
    {0xA06, 5},  // DI6
    {0xA06, 6},  // DI7
    {0xA06, 7},  // DI8
    {0xA07, 0},  // DI9
    {0xA07, 1},  // DI10
    {0xA07, 2},  // DI11
    {0xA07, 3},  // DI12
    {0xA07, 4},  // DI13
    {0xA07, 5},  // DI14
    {0xA07, 6},  // DI15
    {0xA07, 7}   // DI16
};

bool mod_all_out = false;
bool mod_all_input = false;
bool mod_8in_8out = false;

bool init_iopl() {
    if (iopl(3) < 0) {
        perror("iopl");
        return false;
    }
    return true;
}

void enter_pnp_mode() {
    // 进入SIO的 pnp 模式，便于访问SIO 内部寄存器
    outportb(0x2E, 0x87); // Enter PNP mode
    outportb(0x2E, 0x01);
    outportb(0x2E, 0x55);
    outportb(0x2E, 0x55);

    // 选型SIO的逻辑设备7
    outportb(0x2E, 0x07); // Select LDN=7 GPIO
    outportb(0x2F, 0x07);

    //根据SIO spec，使用GP80--GP87，需要 disable SMBUS Isolastion
	outportb(0x2E,0x2C);
	outportb(0x2F,inportb(0x2F)|0x80);
}

void exit_pnp_mode() {
    // 访问结束后，退出PNP 模式
    outportb(0x2E, 0x02);
    outportb(0x2F, 0x02);
}


void initmod_all_out(void)
{
    mod_all_out = true;
    
    enter_pnp_mode();

    // 将GP70--GP77 设置为output模式
    outportb(0x2E, 0xCE); // GPIO output init
    outportb(0x2F, 0xFF);

    // 将GP80--GP87 设置为output模式
    outportb(0x2E, 0xCF); // GPIO output init
    outportb(0x2F, 0xFF);

    // 将GP70--GP77 输出低电平
    outportb(GP1_GPIO_BASE, 0x00); // Output low

    // 将GP80--GP87 输出低电平
    outportb(GP2_GPIO_BASE, 0x00); // Output low

    exit_pnp_mode();
}

void initmod_all_input(void)
{
    mod_all_input = true;
    
    enter_pnp_mode();

    // 将GP70--GP77 设置为input模式
    outportb(0x2E, 0xCE); // GPIO input init
    outportb(0x2F, 0x00);

    // 将GP80--GP87 设置为input模式
    outportb(0x2E, 0xCF); // GPIO input init
    outportb(0x2F, 0x00);

    exit_pnp_mode();
}

void initmod_8in_8out(void)
{
    mod_8in_8out = true;
    
    enter_pnp_mode();

    // 将GP70--GP77 设置为input模式
    outportb(0x2E, 0xCE); // GPIO input init
    outportb(0x2F, 0x00);

    // 将GP80--GP87 设置为output模式
    outportb(0x2E, 0xCF); // GPIO output init
    outportb(0x2F, 0xFF);

    // 将GP80--GP87 输出低电平
    outportb(GP2_GPIO_BASE, 0x00); // Output low

    exit_pnp_mode();
}

// 定义设置pin口电平函数
bool set_pin_level(unsigned int pinIndex, bool state_on) {
    if (pinIndex < NUM_PINS) {
        unsigned short base = pinConfig[pinIndex].base;
        unsigned char offset = pinConfig[pinIndex].offset;
        
        unsigned char reg_value = inportb(base);
        if (state_on) {
            reg_value |= (1 << offset);
        } else {
            reg_value &= ~(1 << offset);
        }
        outportb(base, reg_value);

        // 验证是否成功设置
        unsigned char new_reg_value = inportb(base);
        if ((new_reg_value & (1 << offset)) == (state_on << offset)) {
            return true; // 设置成功
        } else {
            return false; // 设置失败
        }
    }
    return false;
}

//读取pin口电频函数
bool read_pin_level(unsigned int pinIndex) {
    if (pinIndex < NUM_PINS) {
        unsigned short base = pinConfig[pinIndex].base;
        unsigned char offset = pinConfig[pinIndex].offset;
        
        unsigned char reg_value = inportb(base);
        return (reg_value & (1 << offset)) != 0;
    }
    return false;
}