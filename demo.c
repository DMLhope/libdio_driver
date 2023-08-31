#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include <dio_driver.h>
int main() {

    if (!init_iopl()) {
        return -1;
    }
    
    initmod_8in_8out();

    for(int i=8;i<16;i++){
        set_pin_level(i,true);
        usleep(1500000);
        set_pin_level(i,false);
    }
    
    
    while (1)
    {
        printf("state: ");
        for(int i =0;i<8;i++){
            printf("%d ",read_pin_level(i));
        }
        printf("\n");
        usleep(1500000);
    }
    
    return 0;
}
