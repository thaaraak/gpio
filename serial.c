#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() 
{

    int fd = serialOpen ("/dev/ttyS0", 9600 );
    int c;

    while (( c = serialGetchar(fd)) >= 0 )
        putchar(c);

    return (EXIT_SUCCESS);
}