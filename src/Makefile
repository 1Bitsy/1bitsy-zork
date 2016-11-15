##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

#V=1

SOURCES := button_boot.c \
	   actors.c ballop.c cdcacm.c clockr.c demons.c dgame.c        \
           dinit.c dmain.c dso1.c dso2.c dso3.c dso4.c dso5.c dso6.c   \
           dso7.c dsub.c dverb1.c dverb2.c gdt.c led-ping.c lightp.c   \
           local.c nobjs.c np.c np1.c np2.c np3.c nrooms.c objcts.c    \
           rooms.c sobjs.c supp.c sverbs.c systick.c tty.c             \
           tty-stdio.c verbs.c villns.c main.c
OBJS     := $(SOURCES:.c=.o)

BINARY = zork

LDSCRIPT = ../stm32f4-1bitsy.ld
CPPFLAGS += -DALLOW_GDT
CPPFLAGS += -Wno-parentheses -Werror -Wno-error=char-subscripts -std=gnu11

include ../libopencm3.target.mk

# Ancient C code is full of undefined behavior.  Don't let the optimizer
# touch anything.
CFLAGS := $(CFLAGS:-O%=-O0)

textarray.h: gen-textarray.py dtextc.dat
	python3.5 gen-textarray.py dtextc.dat > $@

dinit.o: textarray.h

clean::
	$(Q)$(RM) textarray.h

