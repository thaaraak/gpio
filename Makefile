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
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt -lbcm2835

#LIBS = /home/xenir/bcm/bcm2835-1.68/src/libbcm2835.a

# Should not alter anything below this line
###############################################################################

SRC	=	main.c main2.c gpiomem.c devmem.c spi.c

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
	$Q sudo chown root devmem
	$Q sudo chmod 4755 devmem

spi: spi.o
	$Q echo [link]
	$Q $(CC) -o $@ spi.o $(LDFLAGS) $(LDLIBS) $(LIBS)
	$Q sudo chown root spi
	$Q sudo chmod 4755 spi

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
