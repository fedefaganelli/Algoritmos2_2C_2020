CC=g++
CFLAGS=-std=c++11 -O0 -g3 -Wall -fmessage-length=0


default: tp0 clean

tp0: main.o PrintService.o ReadService.o Transaction.o Body.o Input.o Output.o Block.o Header.o cmdline.o sha256.o
	$(CC) $(CFLAGS) -o tp0 main.o PrintService.o ReadService.o Transaction.o Body.o Input.o Output.o Block.o Header.o cmdline.o sha256.o 

main.o: main.cpp  PrintService.h ReadService.h   Block.h  cmdline.h sha256.h
	$(CC) $(CFLAGS) -c main.cpp

Body.o:  Body.cpp Body.h Transaction.h
	$(CC) $(CFLAGS) -c Body.cpp

ReadService.o:  ReadService.cpp ReadService.h Transaction.h
	$(CC) $(CFLAGS) -c ReadService.cpp


PrintService.o:  PrintService.cpp PrintService.h
	$(CC) $(CFLAGS) -c PrintService.cpp

Transaction.o:  Transaction.cpp Transaction.h Input.h Output.h lista.h
	$(CC) $(CFLAGS) -c Transaction.cpp

cmdline.o:  cmdline.cc  cmdline.h
	$(CC) $(CFLAGS) -c cmdline.cc


Output.o:  Output.cpp Output.h
	$(CC) $(CFLAGS) -c Output.cpp

Input.o:  Input.cpp Input.h
	$(CC) $(CFLAGS) -c Input.cpp

sha256.o:  sha256.cpp sha256.h
	$(CC) $(CFLAGS) -c sha256.cpp

Block.o:  Block.cpp Block.h Header.h Body.h
	$(CC) $(CFLAGS) -c Block.cpp

Header.o:  Header.cpp Header.h
	$(CC) $(CFLAGS) -c Header.cpp


clean: 
	$(RM) count *.o *~
