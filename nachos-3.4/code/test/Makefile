# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort test ascii help syscall_int_demo syscall_char_demo syscall_string_demo ping pong ping_pong sinhvien_voinuoc sinhvien voinuoc

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

test.o: test.c
	$(CC) $(CFLAGS) -c test.c
test: test.o start.o
	$(LD) $(LDFLAGS) start.o test.o -o test.coff
	../bin/coff2noff test.coff test

ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii

help.o: help.c
	$(CC) $(CFLAGS) -c help.c
help: help.o start.o
	$(LD) $(LDFLAGS) start.o help.o -o help.coff
	../bin/coff2noff help.coff help

syscall_int_demo.o: syscall_int_demo.c
	$(CC) $(CFLAGS) -c syscall_int_demo.c
syscall_int_demo: syscall_int_demo.o start.o
	$(LD) $(LDFLAGS) start.o syscall_int_demo.o -o syscall_int_demo.coff
	../bin/coff2noff syscall_int_demo.coff syscall_int_demo

syscall_char_demo.o: syscall_char_demo.c
	$(CC) $(CFLAGS) -c syscall_char_demo.c
syscall_char_demo: syscall_char_demo.o start.o
	$(LD) $(LDFLAGS) start.o syscall_char_demo.o -o syscall_char_demo.coff
	../bin/coff2noff syscall_char_demo.coff syscall_char_demo

syscall_string_demo.o: syscall_string_demo.c
	$(CC) $(CFLAGS) -c syscall_string_demo.c
syscall_string_demo: syscall_string_demo.o start.o
	$(LD) $(LDFLAGS) start.o syscall_string_demo.o -o syscall_string_demo.coff
	../bin/coff2noff syscall_string_demo.coff syscall_string_demo

test.o: test.c
	$(CC) $(CFLAGS) -c test.c
test: test.o start.o
	$(LD) $(LDFLAGS) start.o test.o -o test.coff
	../bin/coff2noff test.coff test

ping.o: ping.c
	$(CC) $(CFLAGS) -c ping.c
ping: ping.o start.o
	$(LD) $(LDFLAGS) start.o ping.o -o ping.coff
	../bin/coff2noff ping.coff ping

pong.o: pong.c
	$(CC) $(CFLAGS) -c pong.c
pong: pong.o start.o
	$(LD) $(LDFLAGS) start.o pong.o -o pong.coff
	../bin/coff2noff pong.coff pong

ping_pong.o: ping_pong.c
	$(CC) $(CFLAGS) -c ping_pong.c
ping_pong: ping_pong.o start.o
	$(LD) $(LDFLAGS) start.o ping_pong.o -o ping_pong.coff
	../bin/coff2noff ping_pong.coff ping_pong

sinhvien_voinuoc.o: sinhvien_voinuoc.c
	$(CC) $(CFLAGS) -c sinhvien_voinuoc.c
sinhvien_voinuoc: sinhvien_voinuoc.o start.o
	$(LD) $(LDFLAGS) start.o sinhvien_voinuoc.o -o sinhvien_voinuoc.coff
	../bin/coff2noff sinhvien_voinuoc.coff sinhvien_voinuoc

sinhvien.o: sinhvien.c
	$(CC) $(CFLAGS) -c sinhvien.c
sinhvien: sinhvien.o start.o
	$(LD) $(LDFLAGS) start.o sinhvien.o -o sinhvien.coff
	../bin/coff2noff sinhvien.coff sinhvien

voinuoc.o: voinuoc.c
	$(CC) $(CFLAGS) -c voinuoc.c
voinuoc: voinuoc.o start.o
	$(LD) $(LDFLAGS) start.o voinuoc.o -o voinuoc.coff
	../bin/coff2noff voinuoc.coff voinuoc