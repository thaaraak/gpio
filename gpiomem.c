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

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

int main(int argc, char** argv) 
{
    int pin = 18;

    int memfd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    uint32_t * map = (uint32_t *)mmap(
                        NULL,
                        4*1024,
                        (PROT_READ | PROT_WRITE),
                        MAP_SHARED, 
                        memfd, 
                        0x200000);
    if (map == MAP_FAILED)
        printf("bcm2835_init: mmap failed: %s\n", strerror(errno));    
    close(memfd);

    volatile uint32_t* paddr = map + GPFSEL1;

    *paddr= 1 << ( (pin-10)*3 );

    volatile uint32_t* paddr1 = map + 0x1C/4;
    volatile uint32_t* paddr2 = map + 0x28/4;

    for(;;)
    {
        *paddr1= 1 << pin;
        delay(100);
        *paddr2= 1 << pin;
        delay(100);
    };

    return (EXIT_SUCCESS);
}