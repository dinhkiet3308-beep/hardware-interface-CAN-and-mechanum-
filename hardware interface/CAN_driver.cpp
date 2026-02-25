#include <iostream>
#include <cstring> //for memset and strcpy
#include <sys/ioctl.h>
#include <net/if.h> //for struct ifreq
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h> //for write()

int init_CAN (const char* interface_name) { //C++ does not have a variable type for string so use pointer here
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW); //open socket
    if (s < 0) {
        perror("socket error:"); //perror() used to translate the global variable errno to show the exact errors
        return -1;
    }

    struct ifreq ifr; //like a form to fill in
    strcpy(ifr.ifr_name, interface_name);
    ioctl(s, SIOCGIFINDEX, &ifr); //tell the kernel to look for the 'vcan0' index and fill in ifr

    struct sockaddr_can sockar;
    memset(&sockar, 0, sizeof(sockar)); //set all to 0 to eliminate old messages
    sockar.can_family = AF_CAN;
    sockar.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&sockar, sizeof(sockar)); //bind() only accept struct sockaddr so we need to typecast here

    return s;
}

int main () {

    float target_velocity = 2.5f;

    int socket_id = init_CAN("vcan0");

    struct can_frame canfr;
    canfr.can_id = 0x6D; //node 3 + 0x00D                                                                                                                                                                                                                                              
    canfr.can_dlc = 8;  

    memset(canfr.data, 0, 8);
    memcpy(canfr.data, &target_velocity, sizeof(target_velocity));                                                                                                                                                                                                                                                                                                                                        

    write(socket_id, &canfr, sizeof(canfr));

    return 0;
}