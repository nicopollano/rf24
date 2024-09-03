

#define DEVICE_ID 0

#define CLIENTS_ID 1

#define IRQ_PIN 22

#define TOTAL_LIGHT 1

uint8_t ADDRESS_PREFIX[4] = { 1, 2, 3, 4 };

uint8_t addressrx[5] = {ADDRESS_PREFIX[0], ADDRESS_PREFIX[1], ADDRESS_PREFIX[2], ADDRESS_PREFIX[3], DEVICE_ID};

uint8_t addresstx[5] = {ADDRESS_PREFIX[0], ADDRESS_PREFIX[1], ADDRESS_PREFIX[2], ADDRESS_PREFIX[3], CLIENTS_ID};

struct RFDATA{
    uint8_t all_devices = false;
    uint8_t id = CLIENTS_ID;
    uint8_t device_enable = true;
    uint8_t pinstate = 0;
    uint8_t cmd = 0;
    char text[10] = "";
    struct{
        uint8_t hh;
        uint8_t mm;
    }time;
}RfData;

// CMD
#define CMD_NONE        0
#define CMD_IGNORE_DATA 1
#define CMD_PIN_STATE   2
#define CMD_ENABLE_DEVICE 3
#define CMD_DISABLE_DEVICE 4
#define CMD_PING_DEVICE 5
#define CMD_REQUEST_INFO 6
#define CMD_CLEAN_LCD 7
#define CMD_SWITCH_EVENT 8

RF24 radio;
RF24 radioRx;

int LOCK_PROCESS = false;

int SYSTEM_COUNT = 0;


#include "./include/nRF24L01.h"
#include "./include/Myinterrupt.h"

#include "./include/radioHelper.h"
#include "Control.h"

ControlLight Control;


#include "./include/interrupts.h"

