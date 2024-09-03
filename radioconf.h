
/**************************************
**********    RADIO TX    *************            
**************************************/
radio.setChannel(100);
radio.setDataRate(RF24_2MBPS);
radio.setRetries(2, 15);
radio.enableDynamicPayloads();
radio.enableDynamicAck();
radio.enableAckPayload();
radio.setPALevel(RF24_PA_MAX);
radio.openReadingPipe(1, addressrx);
radio.openWritingPipe(addresstx);
radio.setCRCLength(rf24_crclength_e::RF24_CRC_8);
//radio.maskIRQ(1,1,0);

/**************************************
**********    RADIO RX    *************            
**************************************/

radioRx.setChannel(100);
radioRx.setDataRate(RF24_2MBPS);
radioRx.setRetries(2, 15);
radioRx.enableDynamicPayloads();
radioRx.enableDynamicAck();
radioRx.enableAckPayload();
radioRx.setPALevel(RF24_PA_MAX);
radioRx.openReadingPipe(1, addressrx);
radioRx.openWritingPipe(addresstx);
radioRx.setCRCLength(rf24_crclength_e::RF24_CRC_8);
radioRx.maskIRQ(1,1,0);
radioRx.startListening();


signal(SIGHUP, shutdownHandler);
signal(SIGINT, shutdownHandler);
signal(SIGCONT, shutdownHandler);
signal(SIGTERM, shutdownHandler);

