
int extract_from_line(char *str, unsigned long offset){
    str += offset;
    char *pointer = strstr(str, "=");
    int len = (pointer-str);
    char *stringlength_p = strstr(str, " ");
    int stringlength = 0;
    if(!stringlength_p) stringlength = strlen(str);
    else stringlength = stringlength_p - str;
    
    char *subs = (char*)malloc(len);
    memcpy(subs, pointer+1, stringlength - len);
    printf("substring[%s]: %s / %s\n", str,  pointer, subs);
    return atoi(subs);
    
}

void *fifo(void *arg){
    while(1){
        int fo = open("/tmp/clockserverfifo", O_RDONLY);
        char fifo_in[150] = "";

        read(fo, fifo_in, 150);
        printf("FIFO_IN: %s\n", fifo_in);
        
        RFDATA r;

        char *id_p = strstr(fifo_in, "-id=");

        if(id_p == nullptr) continue;

        long idoffset = (strstr(fifo_in, " ") - fifo_in) + 1;

        r.id = extract_from_line(fifo_in, 0);
        if(r.id == 0) r.all_devices = true;
        
        int val = extract_from_line(fifo_in, idoffset);

        if(strstr(fifo_in, "-turn-on-light=")){
            if(val > 1 ) continue;
                LOCK_PROCESS = true;
                delay(100);
                /*r.cmd = CMD_PIN_STATE;
                r.pinstate = val;*/
            if(val) puts("L ON");
            else puts("L OFF");
                //sprintf(r.text, "Turned %s-> %i", val ? "on" : "off", r.id);
                //rf_send(&r);
                Control.changeState(r.id, val);
        }

        if(strstr(fifo_in, "-alternate-light=")){
            Control.alternateState(r.id);
        }

        if(strstr(fifo_in, "-lock-device=")){
            if(val > 1 ) continue;
                LOCK_PROCESS = true;
                delay(100);
                /*if(val) r.cmd = CMD_ENABLE_DEVICE;
                else r.cmd = CMD_DISABLE_DEVICE;
                
                rf_send(&r);*/
                Control.Lock(r.id, val);
                puts("DEVICE LOCKED");
        }

        if(strstr(fifo_in, "-lcd_clear=1")){
            if(val !=  1 ) continue;
                LOCK_PROCESS = true;
                delay(100);
                /*r.cmd = CMD_CLEAN_LCD;
                rf_send(&r);*/
                Control.clearLCD(r.id);
                puts("LCD CLEANED");
        }
        
        if(strstr(fifo_in, "-lcd_msg=")){
            char *tc = strstr(fifo_in, "-lcd_msg=") + 9;
            delay(100);
            Control.Message(r.id, tc, strlen(tc));
            printf("Sending message: %s\n", tc);
        }

        LOCK_PROCESS = false;
    }
}