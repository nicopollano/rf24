void ControlLight::setTime(uint8_t id, uint8_t hh, uint8_t mm){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_IGNORE_DATA;
    send(&rft);
    //StructCopyLocal(&rft);
}

void ControlLight::Message(uint8_t id, char *txt, uint8_t len){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_CLEAN_LCD;
    memcpy((char*)rft.text, txt, (len > 10) ? 10 : len);
    send(&rft);
    StructCopyPrivate(&rft);
}

void ControlLight::Lock(uint8_t id, bool state){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = state ? CMD_DISABLE_DEVICE : CMD_ENABLE_DEVICE;
    send(&rft);
    StructCopyPrivate(&rft);
}

void ControlLight::On(uint8_t id){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = 1;
    send(&rft);
    //StructCopyLocal(&rft);
}

void ControlLight::Off(uint8_t id){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = 0;
    send(&rft);
    //StructCopyLocal(&rft);
}

void ControlLight::changeState(uint8_t id, uint8_t state){
    RFDATA rft;
    rft.all_devices = !id;
    rft.id = id;
    rft.cmd = CMD_PIN_STATE;
    rft.pinstate = state;
    send(&rft);
    //StructCopyLocal(&rft);
}

void ControlLight::alternateState(uint8_t id){
    uint8_t i = 0;
    uint8_t d = 0;
    if(id == 0){
        i = 0;
        d = interrupt_private.Total();
    }
    else{
        i = 0;
        d = 1;
    }
    for(uint8_t x = i; x < d; x++){
        printf("p=%i\nid=%i\npinstate:%i\n", x, interrupt_private.getByPosition(x).id, interrupt_private.getByPosition(x).pinstate);
        puts(":::::::::::::::::");
        changeState(interrupt_private.getByPosition(x).id, !interrupt_private.getByPosition(x).pinstate);
        puts(":::::::::::::::::");
    }
}

void ControlLight::StructCopyLocal(RFDATA *rf1){
    if(rf1->all_devices){
        for(uint8_t i = 0; i < interrupt_local.Total(); i++){
            rf1->id = i;
            interrupt_local.set(rf1, (rf1->id)-1);
        }
        return;
    }

    if(interrupt_local.exist((rf1->id)-1)){
        interrupt_local.set(rf1, (rf1->id)-1);
    }
    else {
        interrupt_local.create(rf1);
        interrupt_private.create(rf1);
    }
}

void ControlLight::StructCopyPrivate(RFDATA *rf1){

    if(rf1->all_devices){

        for(uint8_t i = 0; i < interrupt_private.Total(); i++){
            rf1->id = i;
            interrupt_private.set(rf1, (rf1->id)-1);
        }
        return;
    }

    if(interrupt_private.exist((rf1->id)-1)){
        interrupt_private.set(rf1, (rf1->id)-1);
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
    else interrupt_local.set(rf, id);
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

    for(uint8_t light = 0; light < interrupt_local.Total(); light++){
        printf("\033[1;33m");
        
        printf(":::interrupt_local.Total(): %i\n", interrupt_local.Total());
        
        RFDATA rft_l = interrupt_local.getByPosition(light);
        RFDATA rft_p = interrupt_private.getByPosition(light);

        for(int i = 0; i < sizeof(RFDATA); i++) printf("%x ", ((uint8_t*)&rft_l)[i]);
        printf("\n");
        printf("\033[1;34m");
        for(int i = 0; i < sizeof(RFDATA); i++) printf("%x ", ((uint8_t*)&rft_p)[i]);
        printf("\n");
        printf("\033[0m");
        if(rft_l.id == 255 || rft_p.id == 255) continue;
        
        if(rft_l.device_enable != rft_p.device_enable){
            rft_p.cmd = (!rft_p.device_enable ? CMD_ENABLE_DEVICE : CMD_DISABLE_DEVICE);
            send(&rft_p);
        }
        delay(150);

        rft_l.device_enable = rft_p.device_enable;
        interrupt_private.set(&rft_l, rft_l.id);
        requestInfo(rft_p.id);
        delay(700);
    }
}

void ControlLight::send(RFDATA *rf){
    //if(LOCK_PROCESS) delay(100);
    //rf_send(rf);
    rf24_sender.write(rf);
}