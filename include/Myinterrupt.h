#ifndef __MYINTERRUPT_H__
#define __MYINTERRUPT_H__
#endif

using namespace std;

class Interrupt{
    public:
        void create(RFDATA *r);
        void set(RFDATA *r, uint8_t id);
        RFDATA get(uint8_t id);
        RFDATA getByPosition(uint8_t p);

        bool exist(uint8_t id);
        uint8_t Total();

    //private:
        RFDATA *_rfdata;
        uint8_t _rfdata_size = 0;
        int16_t findById(uint8_t id);
};

void Interrupt::create(RFDATA *r){
    if(_rfdata_size == 0){
        _rfdata = (RFDATA*)malloc(sizeof(RFDATA));
    }
    else{
        _rfdata = (RFDATA*)realloc(_rfdata, sizeof(RFDATA)*(_rfdata_size+1));
    }
    memcpy(_rfdata + (sizeof(RFDATA) * _rfdata_size), r, sizeof(RFDATA));
    
    cout << "\tTABLE: " << (int)_rfdata_size << endl;
    cout << "\tall_devices: " << (int)_rfdata[_rfdata_size].all_devices << endl;
    cout << "\tID: " << (int)_rfdata[_rfdata_size].id << endl;
    cout << "\tPIN_STATE: " << (int)_rfdata[_rfdata_size].pinstate << endl;
    cout << "\tCMD: " << (int)_rfdata[_rfdata_size].cmd << endl;
    cout << "\tTime: " << (int)_rfdata[_rfdata_size].time.hh << ":" << _rfdata[_rfdata_size].time.mm << endl;
    _rfdata_size++;
}

void Interrupt::set(RFDATA *r, uint8_t id){
    int16_t p = findById(id);
    if(p < 0) return;
    memcpy(_rfdata + (sizeof(RFDATA) * findById(id)), r, sizeof(RFDATA));
}


RFDATA Interrupt::get(uint8_t id){
    if(findById(id) < 0) return RFDATA{.id=255};
    return _rfdata[findById(id)];
}

RFDATA Interrupt::getByPosition(uint8_t p){
    if(p >= _rfdata_size) return RFDATA{.id=255};
    return _rfdata[p];
}

int16_t Interrupt::findById(uint8_t id){
    for(int x = 0;  x < _rfdata_size; x++){
        if(_rfdata[x].id == id) 
            return x;
    }
    return -1;
}

bool Interrupt::exist(uint8_t id){
    if(findById(id) >= 0) return true;
    return false;
}

uint8_t Interrupt::Total(){
    return _rfdata_size;
}