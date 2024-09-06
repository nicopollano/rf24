void ControlLight::setTime(uint8_t id, uint8_t hh, uint8_t mm){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_IGNORE_DATA;
    if(t.id != 255 && t.id > 0){
        rft.device_enable = t.device_enable;
        rft.pinstate = t.pinstate;
    }
    send(&rft);
    StructCopyLocal(&rft, 3);
}

void ControlLight::Message(uint8_t id, char *txt, uint8_t len){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_CLEAN_LCD;
    memcpy((char*)rft.text, txt, (len > 10) ? 10 : len);
    if(t.id != 255 && t.id > 0){
        rft.device_enable = t.device_enable;
        rft.pinstate = t.pinstate;
        rft.time.hh = t.time.hh;
        rft.time.mm = t.time.mm;
    }
    send(&rft);
    StructCopyLocal(&rft, 4);
}

void ControlLight::Lock(uint8_t id, bool state){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.device_enable = !state;
    rft.cmd = state ? CMD_DISABLE_DEVICE : CMD_ENABLE_DEVICE;
    if(t.id != 255 && t.id > 0){
        rft.pinstate = t.pinstate;
        rft.time.hh = t.time.hh;
        rft.time.mm = t.time.mm;
    }
    StructCopyLocal(&rft, 1);
    send(&rft);
    
}

void ControlLight::On(uint8_t id){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = 1;
    if(t.id != 255 && t.id > 0){
        rft.device_enable = t.device_enable;
        rft.time.hh = t.time.hh;
        rft.time.mm = t.time.mm;
    }
    send(&rft);
    StructCopyLocal(&rft, 2);
}

void ControlLight::Off(uint8_t id){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = 0;
    if(t.id != 255 && t.id > 0){
        rft.device_enable = t.device_enable;
        rft.pinstate = t.pinstate;
        rft.time.hh = t.time.hh;
        rft.time.mm = t.time.mm;
    }
    send(&rft);
    StructCopyLocal(&rft, 2);
}

void ControlLight::changeState(uint8_t id, uint8_t state){
    RFDATA t = interrupt_local.getById(id);
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = state;
    if(t.id != 255 && t.id > 0){
        rft.device_enable = t.device_enable;
        rft.time.hh = t.time.hh;
        rft.time.mm = t.time.mm;
    }
    send(&rft);
    StructCopyLocal(&rft, 2);
}

void ControlLight::alternateState(uint8_t id){
    uint8_t i = 0;
    uint8_t d = 0;
    if(id == 0){
        i = 0;
        d = interrupt_local.Total();
    }
    else{
        i = 0;
        d = 1;
    }
    for(uint8_t x = i; x < d; x++){
        printf("p=%i\nid=%i\npinstate:%i\n", x, interrupt_local.getByPosition(x).id, interrupt_local.getByPosition(x).pinstate);
        changeState(interrupt_local.getByPosition(x).id, !interrupt_local.getByPosition(x).pinstate);
    }
}

void ControlLight::StructCopyLocal(RFDATA *rf1, uint8_t lock1_light2_time3_msg4){
    if(rf1->all_devices){
        for(uint8_t i = 0; i < interrupt_local.Total(); i++){
            rf1->all_devices = false;
            
            RFDATA t = interrupt_local.getById(i + 1);
            
            if(t.id != 255 && t.id > 0){
                rf1->id = t.id;
                switch(lock1_light2_time3_msg4){
                    case 1:{
                        t.device_enable = rf1->device_enable;
                        break;
                    }
                    case 2:{
                        t.pinstate = rf1->pinstate;
                        break;
                    }
                    case 3:{
                        t.time.hh = rf1->time.hh;
                        t.time.mm = rf1->time.mm;
                        break;
                    }
                }
                interrupt_local.set(&t, (t.id));
            }
            rf1->all_devices = true;
        }
        return;
    }

    if(interrupt_local.exist((rf1->id))){
        //printf("finded: %i\n", )
        interrupt_local.set(rf1, (rf1->id));
    }
    else {
        interrupt_local.create(rf1);
        interrupt_private.create(rf1);
    }
}


void ControlLight::requestInfo(uint8_t id){
    RFDATA rft = interrupt_private.get(id);
    rft.all_devices = false;
    rft.id = id;
    rft.cmd = CMD_REQUEST_INFO;
    send(&rft);
}

void ControlLight::clearLCD(uint8_t id){
    RFDATA rft;
    RfData.all_devices = false;
    rft.id = id;
    RfData.cmd = CMD_NONE;
    send(&RfData);
}

void ControlLight::Check(uint8_t id, RFDATA *rf){
    if(!interrupt_private.exist(id)){
        interrupt_private.create(rf);
        interrupt_local.create(rf);
    }
    else interrupt_private.set(rf, id);
}

void ControlLight::Thread(){
    RfData.all_devices = true;
    RfData.cmd = CMD_IGNORE_DATA;
    send(&RfData);
    delay(150);
    
    RfData.cmd = CMD_PING_DEVICE;
    RfData.all_devices = true;
    send(&RfData);
    SYSTEM_COUNT++;
    delay(150);

    for(uint8_t light = 0; light < interrupt_private.Total(); light++){
        printf("\033[1;33m");
        
        printf(":::interrupt_private.Total(): %i\n", interrupt_private.Total());
        
        RFDATA rft_l = interrupt_local.getByPosition(light);
        RFDATA rft_p = interrupt_private.getByPosition(light);

        for(int i = 0; i < sizeof(RFDATA); i++) printf("%x ", ((uint8_t*)&rft_l)[i]);
        printf("  <-\nid:%i   e:%i\n",rft_l.id, rft_l.device_enable);
        printf("\033[1;34m");
        for(int i = 0; i < sizeof(RFDATA); i++) printf("%x ", ((uint8_t*)&rft_p)[i]);
        printf("  <=\nid: %i   e:%i\n",rft_p.id, rft_p.device_enable);
        printf("\033[0m");
        if(rft_l.id == 255 || rft_p.id == 255) continue;
        
        if(rft_l.device_enable != rft_p.device_enable){
            rft_p.cmd = (rft_l.device_enable ? CMD_ENABLE_DEVICE : CMD_DISABLE_DEVICE);
            rft_p.device_enable = rft_l.device_enable;
            interrupt_private.set(&rft_p, rft_p.id);
            send(&rft_p);
        }
        delay(150);

        /*
        rft_l.device_enable = rft_p.device_enable;
        interrupt_private.set(&rft_l, rft_l.id);
        */

        requestInfo(rft_p.id);
        delay(700);
    }
}

void ControlLight::send(RFDATA *rf){
    //if(LOCK_PROCESS) delay(100);
    //rf_send(rf);
    rf24_sender.write(rf);
}