#Makefile for assignment 1

CC = gcc
WFLAG = -Wall
CFLAG = -c
OFLAG = -o
DFLAG = -ggdb
IFLAG = -I


BINARY = bin
OBJECT = build
SOURCE = src
INCLUDE = include


${BINARY}/360shell: ${OBJECT}/main.o ${OBJECT}/360shell.o
	${CC} ${WFALG} ${DFLAG} ${OFLAG} ${BINARY}/360shell ${OBJECT}/main.o ${OBJECT}/360shell.o


${OBJECT}/main.o: ${INCLUDE}/360shell.h ${SOURCE}/main.c
	${CC} ${WFLAG} ${CFLAG} ${DFLAG} ${IFLAG} ${INCLUDE} ${OFLAG} ${OBJECT}/main.o ${SOURCE}/main.c


${OBJECT}/360shell.o: ${INCLUDE}/360shell.h ${SOURCE}/360shell.c
	${CC} ${WFLAG} ${CFLAG} ${DFLAG} ${IFLAG} ${INCLUDE} ${OFLAG} ${OBJECT}/360shell.o ${SOURCE}/360shell.c


.PHONY: clean run


clean:
	rm -rf ${BINARY}/* ${OBJECT}/*

run: ${BINARY}/360shell
	${BINARY}/360shell


