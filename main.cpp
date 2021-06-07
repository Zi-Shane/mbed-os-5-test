#include "mbed.h"
 
DigitalOut led1(LED1);
DigitalOut led2(LED2);
Thread thread;
 
void led2_thread() {
    while (true) {
        led2 = !led2;
        ThisThread::sleep_for(2000);
    }
}
 
int main() {
    thread.start(led2_thread);
    
    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(1000);
    }
}