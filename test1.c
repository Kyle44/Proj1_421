// Name: Kyle Fritz
// File: test1.c
// Date Created: 3/26/16

#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define CREATE_MBOX 377
#define REMOVE_MBOX 378
#define __NR_COUNT_MBOX 379 // void params
#define LIST_MBOX 380
#define SEND_MSG 381
#define RECV_MSG 382
#define COUNT_MSG 383

long CountMBox(void){
	return syscall(__NR_COUNT_MBOX);
}

int main(int argc, char *argv[]){
	CountMBox();
	return 0;
}
