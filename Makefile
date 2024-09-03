rf24:  ./include/nRF24L01.h enviroment.h ./include/Myinterrupt.h ./include/fifo.h ./include/radioHelper.h ./Control.h
	g++ rf24.cpp -o rt -lpigpio -Wall -lrf24-bcm -lpthread
