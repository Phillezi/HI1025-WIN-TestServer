# Makefile for Windows
SRCDIR=./source
CC=gcc
CFLAGS = -g -c 
LDFLAGS = -Wall -lmingw32 -lws2_32

client:
	@echo "Building client"
	$(CC) $(SRCDIR)/client.c  -o ./out/client.exe $(LDFLAGS)

server:
	@echo "Building server"
	$(CC) $(SRCDIR)/server.c  -o ./out/server.exe $(LDFLAGS)