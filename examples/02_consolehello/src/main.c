#include <stdint.h>
#include "ubox.h"

extern void printf(char* message);

void main()
{
    
    ubox_init_isr(2);

	printf("HELLO WORLD!!");
   
    while (1)
    {	
        ubox_wait();
    }
}
