#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include "Serial.h"

int Write_Char_9bit(int fd,unsigned char data,bool wakeupbit){
    Set_Parity_Bit(&data, fd, wakeupbit);
    return (write(fd, &data, 1) == -1)? -1:0;
}
// wakeupbitset is a bitset of which characters are going to have the 9th bit high. 
// 0b00000001 or 1 means the first character will have the 9th bit high
// 0b00000011 or 3 means the first and second character will have the 9th bit high
// etc
int Write_String_9bit(int fd, char* data,int wakeupbitset){ 
    int len = strlen(data);
    for (uint32_t i ; i<len; i++){
        if(-1==Write_Char_9bit(fd, data[i],wakeupbitset&1)){
            return -1;
        }
        wakeupbitset = wakeupbitset>>1;
    }
    return 0;
}


void* SendingThread(void* port){
    int returnval =0;
    printf("Sending Thread started\n");
    port = (char*)port;
    int fd;
    fd = Setup_Serial_Send(port);
    Write_String_9bit(fd, "Hello World", 0b01000001);// parity bit on 'W' and 'H'

    pthread_exit( &returnval);
}