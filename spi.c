// spi.c
//
// Example program for bcm2835 library
// Shows how to interface with SPI to transfer a byte to and from an SPI device
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o spi spi.c -l bcm2835
// sudo ./spi
//
// Or you can test it before installing with:
// gcc -o spi -I ../../src ../../src/bcm2835.c spi.c
// sudo ./spi
//
// Author: Mike McCauley
// Copyright (C) 2012 Mike McCauley
// $Id: RF22.h,v 1.21 2012/05/30 01:51:25 mikem Exp $
 
#include <bcm2835.h>
#include <stdio.h>
 #include <time.h>

void delayMillis (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

void write( uint8_t reg, uint8_t data )
{
    char buf[2];
    buf[0] = reg;
    buf[1] = data;
    bcm2835_spi_writenb( buf, 2 );

}

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
// Use for testing
//        bcm2835_set_debug(1);
 
    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }
 
    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

  // Number of Digits display (Register 0x0B)
    write( 0x0b, 7 );

  // Decode mode (Register 0x09)
    write( 0x09, 0 );

  // Turn off Shutdown mode (Register 0x0C)
    write( 0x0c, 1 );

  // Test mode on for 1 second
    write( 0x0f, 1 );
    delayMillis(1000);
    write( 0x0f, 0 );
    
  // Set brightness
		write( 0x0a, 0 );

    for ( int bright = 0 ; bright < 16 ; bright++)
    {
   		write( 0x0a, bright );
  	  for ( int i = 0 ; i < 128 ; i++ ) 
      {
          for ( int j = 1 ; j <= 8 ; j++ ) 
          {
	      	  write( j, i );
          }
  		    delayMillis(3);
      }
    }

    write( 0x0c, 0 );

    bcm2835_spi_end();
    bcm2835_close();

    return 0;
}
