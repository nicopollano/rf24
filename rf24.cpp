
#pragma pack(1)

#include<signal.h>
#include <RF24/RF24.h>
#include <iostream>
#include "enviroment.h"
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "./include/fifo.h"

using namespace std;
RF24 radio2;

int main(){
    RFDATA ttemp = {.id=1, .device_enable=true};
    
    Control.Check(1, &ttemp);
    if(!radio.begin(24, 7)){
        cout << "RadioRx check: ERROR.\nProgran stopped.";
        exit(1);
    }
    else cout << "RadioRx check: OK" << endl;

    if(!radioRx.begin(25, 8)){
        cout << "RadioTx check: ERROR.\nProgran stopped.";
        exit(1);
    }
    else cout << "RadioTx check: OK" << endl;

#include "radioconf.h"

    pinMode(IRQ_PIN, INPUT);
    attachInterrupt(IRQ_PIN, INT_EDGE_FALLING, &interruptHandler);
    radio.stopListening();
    pthread_t p;
    mkfifo("/tmp/clockserverfifo", 0666);
    system("chmod 777 /tmp/clockserverfifo");
    pthread_create(&p, NULL, &fifo, NULL);

    while(1){
        Control.Thread();
    }
}
