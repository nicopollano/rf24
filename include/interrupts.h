void interruptHandler(){
    bool tx_ds, tx_df, rx_dr;
    radioRx.whatHappened(tx_ds, tx_df, rx_dr);
    if(rx_dr){
        printf("\033[1;32m");
        //cout << "======== INPUT INPUT INPUT ==============" << endl;
        while(radioRx.available()){
            RFDATA temp;
            radioRx.read(&temp, sizeof(RFDATA));
            puts("::::DATA::::");
            for(uint8_t x = 0; x < sizeof(RFDATA); x++){
                printf("%x  ", ((uint8_t*)&temp)[x]);
            }
            printf("\n");
        
            if(temp.cmd == CMD_REQUEST_INFO) SYSTEM_COUNT = 8;
            Control.Check(temp.id, &temp);
            radioRx.flush_rx();
        }
        printf("\033[0m");
    }
    
}

void shutdownHandler(int signum) {
  gpioTerminate();
  exit(0);
}