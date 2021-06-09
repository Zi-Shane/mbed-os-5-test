#include <algorithm>
#include "mbed.h"
#include "mbed_stats.h"
#include "TCPSocket.h"
#include "mbed_events.h"

DigitalOut led1(LED1);

#define MBED_HEAP_STATS_ENABLED 1

#define PORT 4063
#define BUFFSIZE 10

int main() {
#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    int rc = 0;

    printf("Start Connection ... \r\n");

    NetworkInterface *network_interface = NetworkInterface::get_default_instance();
    if (NULL == network_interface) {
        printf("NULL network interface! Exiting application....\r\n");
        return 1;
    }

    printf("\n\rUsing WiFi \r\n");
    printf("\n\rConnecting to WiFi..\r\n");
    rc = network_interface->connect();
    if(rc == 0) {
        printf("\n\rConnected to Network successfully\r\n");
    } else {
        printf("\n\rConnection to Network Failed %d! Exiting application....\r\n", rc);
        return 1;
    }

    TCPSocket server;
    TCPSocket* client;
    SocketAddress clientAddress;
    
    int err;
    char   *in_buffer;
    char   *out_buffer;

    SocketAddress ip; 
    network_interface->get_ip_address(&ip);
    const char *p_ip = ip.get_ip_address();
    printf("IP address: %s and Port: %d\n", p_ip ? p_ip : "None", PORT);

    // SocketAddress mask;
    // network_interface->get_netmask(&mask);
    // const char *p_mask = mask.get_ip_address();
    // printf("Netmask: %s\n", p_mask ? p_mask : "None");

    // SocketAddress gateway;
    // network_interface->get_gateway(&gateway);
    // const char *p_gateway = gateway.get_ip_address();
    // printf("Gateway: %s\n", p_gateway ? p_gateway : "None");

    if (ip) {
        err = server.open(network_interface);
        if (err != 0) {
            printf("Error! open() returned: %d\r\n", err);
        } else {
            printf("open() successfully\n");
        }

        server.bind(PORT);
        if (err != 0) {
            printf("Error! bind() returned: %d\r\n", err);
        } else {
            printf("bind() successfully\n");
        }
        server.listen(1);
        if (err != 0) {
            printf("Error! listen() returned: %d\r\n", err);
        } else {
            printf("listen() successfully\n");
        }

        while (1) {
            // printf("Server bound and listening\n");
            client = server.accept(&err);
            if (err != NULL) {
                printf("Error! listen() returned: %d\r\n", err);
            } else {
                printf("accept() successfully\n");
            }
            // client->getpeername(&clientAddress);
            // const char *p_clientAddress = clientAddress.get_ip_address();
            // printf("Client connected from IP address: %s\n", p_clientAddress ? p_clientAddress : "None");

            bool b = true;
            while(b) {
                in_buffer = new char[BUFFSIZE];
                int n = client->recv(in_buffer, BUFFSIZE);
                if (n == 0) {
                    printf("Client disconnected\n");
                    b = false;
                } else {
                    in_buffer[n] = '\0';
                    printf("Received message from Client :'%s'\n", in_buffer);
                    out_buffer = new char[BUFFSIZE];
                    n = sprintf(out_buffer,"Echo - %s", in_buffer);

                    printf("Sending echo to client\n");
                    client->send(out_buffer, n);
                    delete []out_buffer;
                }
            }
            client->close();
        }
    } else {
        printf("No IP\n");
        network_interface->disconnect();
        printf("Program end\n");
    }
  
    // while (true){
    //     led1 = !led1;
    //     ThisThread::sleep_for(1000);
    // }

}
