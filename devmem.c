#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5

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

void delayusec (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000000) ;
  sleeper.tv_nsec = (long)(howLong % 1000000) * 1000 ;

  nanosleep (&sleeper, &dummy) ;
}

void delaynsec (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000000000) ;
  sleeper.tv_nsec = (long)(howLong % 1000000000) ;

  nanosleep (&sleeper, &dummy) ;
}

int main(int argc, char** argv) 
{
    int pin = 18;


    int memfd = open("/dev/mem", O_RDWR | O_SYNC);
    uint32_t * map = (uint32_t *)mmap(
                        NULL,
                        4*1024,
                        (PROT_READ | PROT_WRITE),
                        MAP_SHARED, 
                        memfd, 
                        0xfe200000);
    if (map == MAP_FAILED)
        printf("mmap failed: %s\n", strerror(errno));    
    close(memfd);

    volatile uint32_t* paddr = map + GPFSEL1;


    int value = 0b001 << ( (pin-10)*3 );
    int mask = 0b111 << ( (pin-10)*3 );
    *paddr = (*paddr & ~mask) | (value & mask);

 //   volatile uint32_t* pup = map + GPIO_PUP_PDN_CNTRL_REG1;


//    *paddr= 1 << ( (pin-10)*3 );

    volatile uint32_t* paddr1 = map + 0x1C/4;
    volatile uint32_t* paddr2 = map + 0x28/4;

    volatile int i;

    for(;;)
    {
        *paddr1= 1 << pin;
        //
        delay(1000);
        for ( i = 0 ; i < 300 ; i++ );

        *paddr2= 1 << pin;
        delay(1000);
        for ( i = 0 ; i < 300 ; i++ );
    };

    return (EXIT_SUCCESS);
}
