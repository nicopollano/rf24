#include <mutex>

void rf_send(RFDATA *r){
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    r->time.hh = info->tm_hour;
    r->time.mm = info->tm_min;
    radio.flush_tx();
    memset(r->text, 0, 10);
    uint8_t count = 0;
    while(!radio.write((uint8_t*)r, sizeof(RFDATA))){
        if(count >= 15) break;
        count++;
        delay(25);
        radio.flush_tx();
    }
    delay(40);
}

class RadioTxQueue{
    public:
        RadioTxQueue(RF24 *r){
            local_radio = r;
        }
        void write(RFDATA *data);

    private:
        void add(RFDATA *data);
        RFDATA *pQueue = nullptr;
        RF24 *local_radio;
        static bool nextP;
        static pthread_mutex_t Mymutex;

        static void* QueueThread(void *data){
            pthread_mutex_lock(&Mymutex);
            RFDATA *datad = (RFDATA*)data;
            for(int x = 0; x < sizeof(RFDATA); x++){
                printf("%x  ",  ((uint8_t*)datad)[x]);
            }
            puts("");
            while(!nextP);
            nextP = false;
            if(datad->cmd == CMD_PIN_STATE) delay(100);
            rf_send((RFDATA*)datad);
            nextP = true;
            pthread_mutex_unlock(&Mymutex);
            free(datad);
            pthread_detach(pthread_self());
            pthread_exit((void*)42);
        }
};

bool RadioTxQueue::nextP = true;
pthread_mutex_t RadioTxQueue::Mymutex = PTHREAD_MUTEX_INITIALIZER;

void RadioTxQueue::write(RFDATA *data){
    add(data);
}

void RadioTxQueue::add(RFDATA *data){
    pthread_t r;
    uint8_t *data_t;
    data_t = (uint8_t*)malloc(sizeof(RFDATA));
    memcpy(data_t, data, sizeof(RFDATA));
    
    /*
    printf("==> ");
    for(int x = 0; x < sizeof(RFDATA); x++){
        printf("%x   ",  ((uint8_t*)data_t)[x]);
    }
    puts("");
    */
    pthread_create(&r, NULL, QueueThread, data_t);
}


RadioTxQueue rf24_sender(&radio);