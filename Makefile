#
# Makefile:
#	wiringPi - A "wiring" library for the Raspberry Pi
#	https://projects.drogon.net/wiring-pi
#
#	Copyright (c) 2012-2015 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#	Wiring Compatable library for the Raspberry Pi
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

#ifneq ($V,1)
#Q ?= @
#endif

DEBUG	= -g -O0
#DEBUG	= -O3
CC	?= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe $(EXTRA_CFLAGS) 

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

LIBS = /home/pi/bcm/bcm2835-1.60/src/libbcm2835.a

# Should not alter anything below this line
###############################################################################

SRC	=	main.c main2.c gpiomem.c devmem.c spi.c i2c.c button.c serial.c

OBJ	=	$(SRC:.c=.o)

BINS	=	$(SRC:.c=)

all: $(BINS)	

main:	main.o
	$Q echo [link]
	$Q $(CC) -o $@ main.o $(LDFLAGS) $(LDLIBS) $(LIBS)

main2:	main2.o
	$Q echo [link]
	$Q $(CC) -o $@ main2.o $(LDFLAGS) $(LDLIBS) $(LIBS)

gpiomem:	gpiomem.o
	$Q echo [link]
	$Q $(CC) -o $@ gpiomem.o $(LDFLAGS) $(LDLIBS) $(LIBS)

devmem:	devmem.o
	$Q echo [link]
	$Q $(CC) -o $@ devmem.o $(LDFLAGS) $(LDLIBS) $(LIBS)

button:	button.o
	$Q echo [link]
	$Q $(CC) -o $@ button.o $(LDFLAGS) $(LDLIBS) $(LIBS)

serial: serial.o
	$Q echo [link]
	$Q $(CC) -o $@ serial.o $(LDFLAGS) $(LDLIBS) $(LIBS)

spi: spi.o
	$Q echo [link]
	$Q $(CC) -o $@ spi.o $(LDFLAGS) $(LDLIBS) $(LIBS)

i2c: i2c.o
	$Q echo [link]
	$Q $(CC) -o $@ i2c.o $(LDFLAGS) $(LDLIBS) $(LIBS)

.c.o:
	$Q echo [CC] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	$Q echo [ctags]
	$Q ctags $(SRC)

depend:
	makedepend -Y $(SRC)

# DO NOT DELETE
