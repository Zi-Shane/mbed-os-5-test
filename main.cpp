#include "mbed.h"
 
DigitalOut led1(LED1);
DigitalOut led2(LED2);
Thread thread;
// volatile bool running = true;

// Blink function toggles the led in a long running loop
void blink(DigitalOut *led) {
    for(int i = 0;i < 3;i++){
        *led = !*led;
        ThisThread::sleep_for(3000);
    }
}
 
int main() {
    thread.start(callback(blink, &led1));

    // running = false;
    thread.join();
    
    while (true) {
        led2 = !led2;
        ThisThread::sleep_for(1000);
    }
}