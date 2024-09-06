#ifndef __LIGHTCONTROL_H__
#define __LIGHTCONTROL_H__
#endif

#define ALL_DEV 0

class ControlLight{
    public: 
        void setTime(uint8_t id, uint8_t hh, uint8_t mm);
        void Message(uint8_t id, char *txt, uint8_t len);
        void Lock(uint8_t id, bool state);
        void On(uint8_t id);
        void Off(uint8_t id);
        void changeState(uint8_t id, uint8_t state);
        void alternateState(uint8_t id);
        void clearLCD(uint8_t id);
        void Check(uint8_t id, RFDATA *rf);
        void Thread();
        void StructCopyPrivate(RFDATA* rf1, uint8_t lock1_light2_time3_msg4);
        
    //private:
        Interrupt interrupt_local;
        Interrupt interrupt_private;
        void StructCopyLocal(RFDATA *rf1, uint8_t lock1_light2_time3_msg4);
        void requestInfo(uint8_t id);
        void send(RFDATA *rf);
};

#include "Control.cpp"