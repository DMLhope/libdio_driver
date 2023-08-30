
/************************************************/
/*-----------sf701 change list-------------*/
//091108, w39v04 fireware flash programme; 
/************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>

#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>


#define IO_MODE_IN   0
#define IO_MODE_OUT  1

#define LEVEL_LOW	0

#define port_mask(x)  (1<<(x))

#define outportb(port,data) outb(data,port) 
#define inportb(port)	inb(port)
#define outport(port,data) outl(data,port) 
#define inport(port) inl(port) 

#define GP1_GPIO_BASE 0xA06  //GP1 ADDR
#define GP2_GPIO_BASE 0xA07  //GP2 ADDR  

#define DI1_BASE 0xA06 //GP70
#define DI1_Offset (1<<0) 

#define DI2_BASE 0xA06 //GP71
#define DI2_Offset (1<<1) 

#define DI3_BASE 0xA06 //GP72
#define DI3_Offset (1<<2) 

#define DI4_BASE 0xA06 //GP73
#define DI4_Offset (1<<3) 

#define DI5_BASE 0xA06 //GP74
#define DI5_Offset (1<<4) 

#define DI6_BASE 0xA06 //GP75
#define DI6_Offset (1<<5) 

#define DI7_BASE 0xA06 //GP76
#define DI7_Offset (1<<6) 

#define DI8_BASE 0xA06 //GP77
#define DI8_Offset (1<<7)  


#define DI9_BASE 0xA07 //GP80 
#define DI9_Offset (1<<0) 

#define DI10_BASE 0xA07 //GP81
#define DI10_Offset (1<<1) 

#define DI11_BASE 0xA07 //GP82
#define DI11_Offset (1<<2) 

#define DI12_BASE 0xA07 //GP83
#define DI12_Offset (1<<3) 

#define DI13_BASE 0xA07 //GP84
#define DI13_Offset (1<<4) 

#define DI14_BASE 0xA07 //GP85
#define DI14_Offset (1<<5) 

#define DI15_BASE 0xA07 //GP86
#define DI15_Offset (1<<6) 

#define DI16_BASE 0xA07 //GP87
#define DI16_Offset (1<<7)


typedef unsigned int u32;


//int port[]={35,25,24,29,34,42,31,43};  //Step 1: modify gpio number with hardware order
//int offset[]={0xA0,0x80,0x80,0x80,0xA0,0xA0,0x80,0xA0};
//unsigned int GPIO_BASE=0x500;

//int set_lvl(int lvl);
//void detect_input(void);

//void set_io_mode(int mode);
//void gpio_init(void);
//int read_value(void);

void Test_GPO(void);
int read_value(void);
void set_lvl(int portNum,int level);
void gpio_init(void);
void set_input_mode(void);

int main(){

	//char operate[25];
	//int gpio_x;
	//int gpio_num=sizeof(port)/sizeof(port[0]);
	 //getbaseaddress(0,31,0,0x48)&(~3);
/*	
	USE_SEL=(GPIO_BASE+0x0);
    IO_SEL=(GPIO_BASE+0x04);
	LVL=(GPIO_BASE+0x0c);	
	USE_SEL2=(GPIO_BASE+0x30);
	IO_SEL2=(GPIO_BASE+0x34);
	LVL2=(GPIO_BASE+0x38);
	USE_SEL3=(GPIO_BASE+0x40);
	IO_SEL3=(GPIO_BASE+0x44);
	LVL3=(GPIO_BASE+0x48);
*/
 int value;
 char temp[20];

	if (iopl(3)<0){
		perror("iopl");
		return -1;
		}
		
   // printf("gpio_base:%p\n",GPIO_BASE);
	/****set all pin as GPIO function****/
	gpio_init();
	Test_GPO();
	printf("Soc GPIO output Test End!!\n");
	printf("\n");
	printf("Soc GPIO input Test begin!!\n");
	printf("\n");
	set_input_mode();
	while(1)
	{
		read_value();
		//itoa(value,temp,2);
		//printf("%s",temp);
		printf("input_lvl:  %x\n",value);
		usleep(1500000);
	}
	
return 0;
/*
	while(1){
		printf("cmd>");
		scanf("%s",operate);

		/*function for input test*/
/*	if(!strcmp(operate,"qtest")){
		printf("Please enter cmd 'i' or 'o':");
	        scanf("%s",operate);
		if(!strcmp(operate,"i")){	  
		 //for(index=0;index<gpio_num;index++)		
				set_io_mode(IO_MODE_IN);
		 printf("all gpios is in input mode!\n");
		 detect_input();
	      }
	     else if(!strcmp(operate,"o"))  {	  
		  //for(index=0;index<gpio_num;index++)
			  set_io_mode(IO_MODE_OUT);
		  printf("set gpios level(0 or 1): ");
		  scanf("%d",&gpio_x);
		  //for(index=0;index<gpio_num;index++) 	  
			  set_lvl(gpio_x);
	      }
	    continue;
	}
	else if(!strcmp(operate,"q"))
		break;
	else
	{
	printf("Input invalid command!\n");
	continue;
	}
 wrong:
	printf("Input invalid pin num!\n");

  }
  printf("Programm exit normally!\n");
	 return 0;*/
}

void Test_GPO(void)
{
	//将GP70-77 GP80-87拉高或者拉低输出
	int portNum;
  int gpo_level;
  for(portNum=1;portNum<17;portNum++)
  {
  	printf("set gpo pin%d level(0 or 1): \n",portNum);
  	scanf("%d",&gpo_level);
  	set_lvl(portNum,gpo_level);
  }
}

int read_value(void)
{
	//读取GP70--GP77 GP80--GP87的输入状态
   int value=0;
   int data;
   data=inportb(DI1_BASE);
   data &=DI1_Offset;
   if(data==DI1_Offset){
   	value |=0x0001;
   	printf("GPIO_input_lvl:1 ");}
   	else{
   	printf("GPIO_input_lvl:0 ");
   	}
   	
   data=inportb(DI2_BASE);
   data &=DI2_Offset;
   if(data==DI2_Offset){
   	value |=0x0002;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI3_BASE);
   data &=DI3_Offset;
   if(data==DI3_Offset){
   	value |=0x0004;   
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}	

   data=inportb(DI4_BASE);
   data &=DI4_Offset;
   if(data==DI4_Offset){
   	value |=0x0008;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI5_BASE);
   data &=DI5_Offset;
   if(data==DI5_Offset){
   	value |=0x0010; 
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI6_BASE);
   data &=DI6_Offset;
   if(data==DI6_Offset){
   	value |=0x0020;  
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI7_BASE);
   data &=DI7_Offset;
   if(data==DI7_Offset){
   	value |=0x0040;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI8_BASE);
   data &=DI8_Offset;
   if(data==DI8_Offset){
   	value |=0x0080;	
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}

	data=inportb(DI9_BASE);
   data &=DI9_Offset;
   if(data==DI9_Offset){
   	value |=0x0100;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI10_BASE);
   data &=DI10_Offset;
   if(data==DI10_Offset){
   	value |=0x0200;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI11_BASE);
   data &=DI11_Offset;
   if(data==DI11_Offset){
   	value |=0x0400;   
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}	

   data=inportb(DI12_BASE);
   data &=DI12_Offset;
   if(data==DI12_Offset){
   	value |=0x0800;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI13_BASE);
   data &=DI13_Offset;
   if(data==DI13_Offset){
   	value |=0x1000; 
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI14_BASE);
   data &=DI14_Offset;
   if(data==DI14_Offset){
   	value |=0x2000;  
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI15_BASE);
   data &=DI15_Offset;
   if(data==DI15_Offset){
   	value |=0x4000;
   	printf("1 ");}
   	else{
   	printf("0 ");
   	}
   	
   data=inportb(DI16_BASE);
   data &=DI16_Offset;
   if(data==DI16_Offset){
   	value |=0x8000;
   	printf("1 \n");}
   	else{
   	printf("0 \n");
   	}
   
   return value;
}



/*
void detect_input(void){
	/*int i;
	unsigned int data;
	int lvl;
	while(1){

		sleep(1);
		for(i=0;i<gpio_num;i++){
		  if(port[i]>31){ 
		   data=inport(GPIO_BASE+0xA8);	
                   data &=port_mask(port[i]-32);
		   if(data==0)
			lvl=0;
		   else
			lvl=1;
		  }
		  else	{
			data=inport(GPIO_BASE+0x88);	
			data &=port_mask(port[i]);
		   if(data==0)
			lvl=0;
		   else
			lvl=1;
		  }
		  if(lvl)
	           printf("1 ");
                  else
	           printf("0 ");  	
		}
                printf("\n");
		
	}//while*/
/*	
	int change,newdata,olddata;
	int i=0;
	int count=0;
	int TEST_PASS=0x0000;
	olddata=read_value();
	while(1){
		//if(kbhit()){
		//	if(getch() == 27) return;
		//}
			//delay(50);
			//Sleep(50);
			//mdelay(50);
			usleep(500); //500ms
			newdata=read_value();
			change=(olddata^newdata);
	    	if(change){
				for(i=0;i<16;i++){       //change to 16 
					if((change & (1<<i)))
					{
						 if((newdata&(1<<i)))
						 	{
					  	 printf("GPI-%d input value is high----pass\n",i+1);
					  	}
					   else
					    {
					   	 printf("GPI-%d input value is low----pass\n",i+1);
					   	}
					   	 TEST_PASS |=(1<<i);
					   goto xy;
					}
				}  //for

			//	;
		
		    }// if
xy:
		olddata=newdata;
		if(TEST_PASS==0xFFFF)   //change to 0xFFFF
		{
			printf("Test ALL PASS \n");
			//printf("Press any key to continue...\n");
			return;
			//_asm{
			//MOV AH,01
			//INT 21H
			//}
			//if(getche()==27) return ;
		}
		//delay(500); 
	}//while
	
	
}*/

void set_lvl(int portNum,int level)
{	  
	  switch(portNum){
	  case 1: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x01);printf("GPIO 1 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfe);printf("GPIO 1 set low\n");break;
				}
	  case 2: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x02);printf("GPIO 2 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfd);printf("GPIO 2 set low\n");break;
				}
      case 3: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x04);printf("GPIO 3 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfb);printf("GPIO 3 set low\n");break;
				}
	  case 4: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x08);printf("GPIO 4 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xf7);printf("GPIO 4 set low\n");break;
				}
	  case 5: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x10);printf("GPIO 5 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xef);printf("GPIO 5 set low\n");break;
				}
	  case 6: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x20);printf("GPIO 6 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xdf);printf("GPIO 6 set low\n");break;
				}
      case 7: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x40);printf("GPIO 7 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xbf);printf("GPIO 7 set low\n");break;
				}
	  case 8: if(level)
	            {
	            	 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x80);printf("GPIO 8 set high\n");break;
				 } 
				else
				{
					 outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0x7f);printf("GPIO 8 set low\n");break;
				}						
	  case 9: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x01);printf("GPIO 9 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfe);printf("GPIO 9 set low\n");break;
				}
	  case 10: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x02);printf("GPIO 10 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfd);printf("GPIO 10 set low\n");break;
				}
      case 11: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x04);printf("GPIO 11 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfb);printf("GPIO 11 set low\n");break;
				}
	  case 12: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x08);printf("GPIO 12 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xf7);printf("GPIO 12 set low\n");break;
				}
	  case 13: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x10);printf("GPIO 13 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xef);printf("GPIO 13 set low\n");break;
				}
	  case 14: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x20);printf("GPIO 14 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xdf);printf("GPIO 14 set low\n");break;
				}
      case 15: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x40);printf("GPIO 15 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xbf);printf("GPIO 15 set low\n");break;
				}
	  case 16: if(level)
	            {
	            	 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x80);printf("GPIO 16 set high\n");break;
				 } 
				else
				{
					 outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0x7f);printf("GPIO 16 set low\n");break;
				}									  
	  }
          
}
/* 
int set_lvl(int lvl)
{
	//int group,shift;
	//unsigned int mask;
	//group=port[index]/32;
	//shift=port[index]%32;
	//mask=port_mask(shift);
        int num;
	if(lvl==LEVEL_LOW){
         printf("switch gpio num to set low :");
           scanf("%d",&num);
             switch(num)
{
case 1: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfe);printf("GPIO 1 set low\n");break;
case 2: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfd);printf("GPIO 2 set low\n");break;
case 3: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xfb);printf("GPIO 3 set low\n");break;
case 4: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xf7);printf("GPIO 4 set low\n");break;
case 5: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xef);printf("GPIO 5 set low\n");break;
case 6: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xdf);printf("GPIO 6 set low\n");break;
case 7: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0xbf);printf("GPIO 7 set low\n");break;
case 8: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)&0x7f);printf("GPIO 8 set low\n");break;

case 9: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfe);printf("GPIO 9 set low\n");break;
case 10: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfd);printf("GPIO 10 set low\n");break;
case 11: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xfb);printf("GPIO 11 set low\n");break;
case 12: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xf7);printf("GPIO 12 set low\n");break;
case 13: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xef);printf("GPIO 13 set low\n");break;
case 14: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xdf);printf("GPIO 14 set low\n");break;
case 15: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0xbf);printf("GPIO 15 set low\n");break;
case 16: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)&0x7f);printf("GPIO 16 set low\n");break;


}

}
else
{
printf("switch gpio num to set high :");
           scanf("%d",&num);
 switch(num)
{
case 1: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x01);printf("GPIO 1 set high\n");break;
case 2: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x02);printf("GPIO 2 set high\n");break;
case 3: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x04);printf("GPIO 3 set high\n");break;
case 4: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x08);printf("GPIO 4 set high\n");break;
case 5: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x10);printf("GPIO 5 set high\n");break;
case 6: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x20);printf("GPIO 6 set high\n");break;
case 7: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x40);printf("GPIO 7 set high\n");break;
case 8: outportb(GP1_GPIO_BASE,inportb(GP1_GPIO_BASE)|0x80);printf("GPIO 8 set high\n");break;

case 9: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x01);printf("GPIO 9 set high\n");break;
case 10: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x02);printf("GPIO 10 set high\n");break;
case 11: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x04);printf("GPIO 11 set high\n");break;
case 12: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x08);printf("GPIO 12 set high\n");break;
case 13: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x10);printf("GPIO 13 set high\n");break;
case 14: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x20);printf("GPIO 14 set high\n");break;
case 15: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x40);printf("GPIO 15 set high\n");break;
case 16: outportb(GP2_GPIO_BASE,inportb(GP2_GPIO_BASE)|0x80);printf("GPIO 16 set high\n");break;


}

} */
	/*//outport(GPIO_BASE+offset[index]+0x08,0);	
		outportb(GP1_GPIO_BASE,0x00);  //Output low
		outportb(GP2_GPIO_BASE,0x00);  //Output low  
	}
	else{
	//outport(GPIO_BASE+offset[index]+0x08,0xFFFFFFFF);
		outportb(GP1_GPIO_BASE,0xFF);  //Output Hign
		outportb(GP2_GPIO_BASE,0xFF);  //Output Hign 
	}*/
/*	return 0;
}*/

void gpio_init(void){
	//int index;
	//int shift=0;

	  
	//for(index=0;index<gpio_num;index++){    //step2:modify pin_num refer to MB hardware
    //    	if(port[index]>31)
	//  shift=port[index]-32;
	//else
	//  shift=port[index];
	  
	//	outport(GPIO_BASE+offset[index],(inport(GPIO_BASE+offset[index]) | (1<<shift)));
		
	//}
	//进入SIO的 pnp 模式，便于访问SIO 内部寄存器
	outportb(0x2E,0x87); //Enetr PNP mode
	outportb(0x2E,0x01);
	outportb(0x2E,0x55);
	outportb(0x2E,0x55);
	
	//选型SIO的逻辑设备7
	outportb(0x2E,0x07); //Select LDN=7 GPIO
	outportb(0x2F,0x07);
	
	//根据SIO spec，使用GP80--GP87，需要 disable SMBUS Isolastion
	outportb(0x2E,0x2C);
	outportb(0x2F,inportb(0x2F)|0x80);
	
	//outportb(0x2E,0xE9);
	//outportb(0x2F,inportb(0x2F)|0x20);
	
	//将GP70--GP77 设置为output模式
	outportb(0x2E,0xCE); //GPIO output init
	outportb(0x2F,0xFF);

   //将GP80--GP87 设置为output模式
	outportb(0x2E,0xCF); //GPIO output init
	outportb(0x2F,0xFF);
	
	//将GP70--GP77 输出低电平
	outportb(GP1_GPIO_BASE,0x00);  //Output low
	
	//将GP80--GP87 输出低电平
	outportb(GP2_GPIO_BASE,0x00);  //Output low  
	
	//访问结束后，退出PNP 模式
	outportb(0x2E,0x02); 
	outportb(0x2F,0x02);
	
}

void set_input_mode(void)
{	

//进入SIO的 pnp 模式，便于访问SIO 内部寄存器
	outportb(0x2E,0x87); //Enetr PNP mode
	outportb(0x2E,0x01);
	outportb(0x2E,0x55);
	outportb(0x2E,0x55);
	
	//选型SIO的逻辑设备7
	outportb(0x2E,0x07); //Select LDN=7 GPIO
	outportb(0x2F,0x07);

//将GP70--GP77 设置为input模式
		outportb(0x2E,0xCE); //GPIO input init
		outportb(0x2F,0x00);

//将GP80--GP87 设置为input模式
		outportb(0x2E,0xCF); //GPIO input init  
		outportb(0x2F,0x00);

//访问结束后，退出PNP 模式
	outportb(0x2E,0x02); 
	outportb(0x2F,0x02);	
}
/*
void set_io_mode(int mode){
	//int shift=0;
	//if(port[index]>31)
	//  shift=port[index]-32;
	//else
	//  shift=port[index];
	outportb(0x2E,0x87); //Enetr PNP mode
	outportb(0x2E,0x01);
	outportb(0x2E,0x55);
	outportb(0x2E,0x55);
	
	outportb(0x2E,0x07); //Select LDN=7 GPIO
	outportb(0x2F,0x07);


	if(mode==IO_MODE_IN){
		//outport(GPIO_BASE+offset[index]+0x04,(inport(GPIO_BASE+offset[index]+0x04) | (1<<shift)));
		outportb(0x2E,0xCE); //GPIO input init
		outportb(0x2F,0x00);

		outportb(0x2E,0xCF); //GPIO input init  
		outportb(0x2F,0x00);
	}
	else
	{
	//outport(GPIO_BASE+offset[index]+0x04,(inport(GPIO_BASE+offset[index]+0x04) & ~(1<<shift)));
		outportb(0x2E,0xCE); //GPIO output init
		outportb(0x2F,0xFF);

		outportb(0x2E,0xCF); //GPIO output init  
		outportb(0x2F,0xFF);
	}
	
	outportb(0x2E,0x02); 
	outportb(0x2F,0x02);
} */




