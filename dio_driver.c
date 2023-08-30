#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>
#include <stdbool.h>

#include "dio_driver.h"

#define outportb(port, data) outb(data, port)
#define inportb(port) inb(port)

#define GP1_GPIO_BASE 0xA06 // GP1 ADDR
#define GP2_GPIO_BASE 0xA07 // GP2 ADDR

#define DI1_BASE 0xA06 // GP70
#define DI1_Offset (1 << 0)

#define DI2_BASE 0xA06 // GP71
#define DI2_Offset (1 << 1)

#define DI3_BASE 0xA06 // GP72
#define DI3_Offset (1 << 2)

#define DI4_BASE 0xA06 // GP73
#define DI4_Offset (1 << 3)

#define DI5_BASE 0xA06 // GP74
#define DI5_Offset (1 << 4)

#define DI6_BASE 0xA06 // GP75
#define DI6_Offset (1 << 5)

#define DI7_BASE 0xA06 // GP76
#define DI7_Offset (1 << 6)

#define DI8_BASE 0xA06 // GP77
#define DI8_Offset (1 << 7)

#define DI9_BASE 0xA07 // GP80
#define DI9_Offset (1 << 0)

#define DI10_BASE 0xA07 // GP81
#define DI10_Offset (1 << 1)

#define DI11_BASE 0xA07 // GP82
#define DI11_Offset (1 << 2)

#define DI12_BASE 0xA07 // GP83
#define DI12_Offset (1 << 3)

#define DI13_BASE 0xA07 // GP84
#define DI13_Offset (1 << 4)

#define DI14_BASE 0xA07 // GP85
#define DI14_Offset (1 << 5)

#define DI15_BASE 0xA07 // GP86
#define DI15_Offset (1 << 6)

#define DI16_BASE 0xA07 // GP87
#define DI16_Offset (1 << 7)

bool mod_all_out = false;
bool mod_all_input = false;
bool mod_8in_8out = false;

void initmod_all_out(void)
{
    mod_all_out = true;
    // 进入SIO的 pnp 模式，便于访问SIO 内部寄存器
    outportb(0x2E, 0x87); // Enetr PNP mode
    outportb(0x2E, 0x01);
    outportb(0x2E, 0x55);
    outportb(0x2E, 0x55);

    // 选型SIO的逻辑设备7
    outportb(0x2E, 0x07); // Select LDN=7 GPIO
    outportb(0x2F, 0x07);

    // 根据SIO spec，使用GP80--GP87，需要 disable SMBUS Isolastion
    outportb(0x2E, 0x2C);
    outportb(0x2F, inportb(0x2F) | 0x80);

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

    // 访问结束后，退出PNP 模式
    outportb(0x2E, 0x02);
    outportb(0x2F, 0x02);
}

void initmod_all_input(void)
{
    mod_all_input = true;
    // 进入SIO的 pnp 模式，便于访问SIO 内部寄存器
    outportb(0x2E, 0x87); // Enetr PNP mode
    outportb(0x2E, 0x01);
    outportb(0x2E, 0x55);
    outportb(0x2E, 0x55);

    // 选型SIO的逻辑设备7
    outportb(0x2E, 0x07); // Select LDN=7 GPIO
    outportb(0x2F, 0x07);

    // 将GP70--GP77 设置为input模式
    outportb(0x2E, 0xCE); // GPIO input init
    outportb(0x2F, 0x00);

    // 将GP80--GP87 设置为input模式
    outportb(0x2E, 0xCF); // GPIO input init
    outportb(0x2F, 0x00);

    // 访问结束后，退出PNP 模式
    outportb(0x2E, 0x02);
    outportb(0x2F, 0x02);
}

void initmod_8in_8out(void)
{
    mod_8in_8out = true;
    // 进入SIO的 pnp 模式，便于访问SIO 内部寄存器
    outportb(0x2E, 0x87); // Enetr PNP mode
    outportb(0x2E, 0x01);
    outportb(0x2E, 0x55);
    outportb(0x2E, 0x55);

    // 选型SIO的逻辑设备7
    outportb(0x2E, 0x07); // Select LDN=7 GPIO
    outportb(0x2F, 0x07);

    // 根据SIO spec，使用GP80--GP87，需要 disable SMBUS Isolastion
    outportb(0x2E, 0x2C);
    outportb(0x2F, inportb(0x2F) | 0x80);

    // 将GP70--GP77 设置为input模式
    outportb(0x2E, 0xCE); // GPIO input init
    outportb(0x2F, 0x00);

    // 将GP80--GP87 设置为output模式
    outportb(0x2E, 0xCF); // GPIO output init
    outportb(0x2F, 0xFF);

    // 将GP80--GP87 输出低电平
    outportb(GP2_GPIO_BASE, 0x00); // Output low

    // 访问结束后，退出PNP 模式
    outportb(0x2E, 0x02);
    outportb(0x2F, 0x02);
}


void set_00_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x01);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xfe);
    }
}

void set_01_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x02);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xfd);
    }
}

void set_02_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x04);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xfb);
    }
}

void set_03_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x08);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xf7);
    }
}

void set_04_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x10);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xef);
    }
}

void set_05_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x20);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xdf);
    }
}

void set_06_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x40);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0xbf);
    }
}

void set_07_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) | 0x80);
    }
    else
    {
        outportb(GP1_GPIO_BASE, inportb(GP1_GPIO_BASE) & 0x7f);
    }
}

void set_10_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x01);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xfe);
    }
}

void set_11_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x02);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xfd);
    }
}

void set_12_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x04);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xfb);
    }
}

void set_13_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x08);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xf7);
    }
}

void set_14_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x10);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xef);
    }
}

void set_15_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x20);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xdf);
    }
}

void set_16_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x40);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0xbf);
    }
}

void set_17_level(bool state_on)
{
    if (state_on)
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) | 0x80);
    }
    else
    {
        outportb(GP2_GPIO_BASE, inportb(GP2_GPIO_BASE) & 0x7f);
    }
}


int read_00_value(){
    int value=0;
    int data;
    data=inportb(DI1_BASE);
   data &=DI1_Offset;
   if(data==DI1_Offset){
   	value |=0x0001;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_01_value(){
    int value=0;
    int data;
    data=inportb(DI2_BASE);
   data &=DI2_Offset;
   if(data==DI2_Offset){
   	value |=0x0002;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_02_value(){
    int value=0;
    int data;
    data=inportb(DI3_BASE);
   data &=DI3_Offset;
   if(data==DI3_Offset){
   	value |=0x0004;   
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_03_value(){
    int value=0;
    int data;
    data=inportb(DI4_BASE);
   data &=DI4_Offset;
   if(data==DI4_Offset){
   	value |=0x0008;
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_04_value(){
    int value=0;
    int data;
    data=inportb(DI5_BASE);
   data &=DI5_Offset;
   if(data==DI5_Offset){
   	value |=0x0010; 
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_05_value(){
    int value=0;
    int data;
    data=inportb(DI6_BASE);
   data &=DI6_Offset;
   if(data==DI6_Offset){
   	value |=0x0020;  
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_06_value(){
    int value=0;
    int data;
    data=inportb(DI7_BASE);
   data &=DI7_Offset;
   if(data==DI7_Offset){
   	value |=0x0040;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_07_value(){
    int value=0;
    int data;
    data=inportb(DI8_BASE);
   data &=DI8_Offset;
   if(data==DI8_Offset){
   	value |=0x0080;	
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_10_value(){
    int value=0;
    int data;
    data=inportb(DI9_BASE);
   data &=DI9_Offset;
   if(data==DI9_Offset){
   	value |=0x0100;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_11_value(){
    int value=0;
    int data;
    data=inportb(DI10_BASE);
   data &=DI10_Offset;
   if(data==DI10_Offset){
   	value |=0x0200;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_12_value(){
    int value=0;
    int data;
    data=inportb(DI11_BASE);
   data &=DI11_Offset;
   if(data==DI11_Offset){
   	value |=0x0400;   
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_13_value(){
    int value=0;
    int data;
    data=inportb(DI12_BASE);
   data &=DI12_Offset;
   if(data==DI12_Offset){
   	value |=0x0800;
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_14_value(){
    int value=0;
    int data;
    data=inportb(DI13_BASE);
   data &=DI13_Offset;
   if(data==DI13_Offset){
   	value |=0x1000; 
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_15_value(){
    int value=0;
    int data;
    data=inportb(DI14_BASE);
   data &=DI14_Offset;
   if(data==DI14_Offset){
   	value |=0x2000;  
   	return 1;}
   	else{
   	return 0;
   	}
}


int read_16_value(){
    int value=0;
    int data;
    data=inportb(DI15_BASE);
   data &=DI15_Offset;
   if(data==DI15_Offset){
   	value |=0x4000;
   	return 1;}
   	else{
   	return 0;
   	}
}

int read_17_value(){
    int value=0;
    int data;
    data=inportb(DI16_BASE);
   data &=DI16_Offset;
   if(data==DI16_Offset){
   	value |=0x8000;
   	return 1;}
   	else{
   	return 0;
   	}
}

