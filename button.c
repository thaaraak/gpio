#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5

#define GPLEV0  (0x34/4)
#define GPLEV1  (0x38/4)

#define GPIO_PUP_PDN_CNTRL_REG0 (0xe4/4)
#define GPIO_PUP_PDN_CNTRL_REG1 (0xe8/4)
#define GPIO_PUP_PDN_CNTRL_REG2 (0xec/4)
#define GPIO_PUP_PDN_CNTRL_REG3 (0xf0/4)


void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

int main() {

    int pin = 17;


    int memfd = open("/dev/mem", O_RDWR | O_SYNC);
    uint32_t * map = (uint32_t *)mmap(
                        NULL,
                        4*1024,
                        (PROT_READ | PROT_WRITE),
                        MAP_SHARED, 
                        memfd, 
                        0xfe200000);
    if (map == MAP_FAILED)
        printf("bcm2835_init: mmap failed: %s\n", strerror(errno));    
    close(memfd);
    

    volatile uint32_t* paddr = map + GPFSEL1;

    int value = 0 << ( (pin-10)*3 );
    int mask = 0b111 << ( (pin-10)*3 );
    *paddr = (*paddr & ~mask) | (value & mask);

    volatile uint32_t* pup = map + GPIO_PUP_PDN_CNTRL_REG1;
    value = 0b01 << ( (pin-16)*2 );
    mask = 0b11 << ( (pin-16)*2 );
    *pup = (*pup & ~mask) | (value & mask);

    volatile uint32_t* buttonval = map + GPLEV0;
    value = 1 << pin;

    int lastval = -1; 
    for(;;)
    {
        int butval = (*buttonval & value) != 0;
        if ( lastval != butval )
        {
            printf( "Value: %d\n", butval );
            lastval = butval;
        }

        delay(1);
    };

    return (EXIT_SUCCESS);
}