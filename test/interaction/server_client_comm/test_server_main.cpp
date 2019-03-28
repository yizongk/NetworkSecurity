#include <iostream>
#include <iomanip>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For server to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    Endpoint dummyServer(argv[1]);
    dummyServer.bootup();
    
    while(true){
        dummyServer.run_protocol_rcv(incom_buf, bytes, PORT_NUM);
    }
/*
    while(true) {
        memset(incom_buf,0,BUFLEN);
        if( dummyServer.listen(incom_buf, bytes, PORT_NUM) ) {

            /* printf("%d. Received(%zu):\n'",i,bytes);
            for(int j=0;j<bytes;++j) {
                printf("%02x ",incom_buf[j]);
            }
            printf("'\n"); */
/*
            cout << ". Received(" << bytes << " bytes):" << endl << "'";
            for(int j = 0; j < bytes; ++j) {
                cout << std::hex << (int)incom_buf[j];
                //cout << (char)incom_buf[j];
            }
            cout << std::dec << "'" << endl;

            // MAC address for interface 'lo' is: '00:00:00:00:00:00:'
            // MAC address for interface 'wlp59s0' is: '9c:b6:d0:ff:c8:75:  NOTE: This only applies to my own device(yi zong). Check out tool/getAllIndexAndMac.cpp to get your own Mac address'

            /* Extracting the Ethernet header as defined in linux/if_ether.h */
            /* struct ethhdr *eth = (struct ethhdr *)(incom_buf);
            printf("\nEthernet Header (The physical address, or in other word MAC address)\n");
            printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
            printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
            printf("\t|-Protocol : %d\n",eth->h_proto); */
/*
            printf("---------------------------------------------\n");


        }
    }
*///
//*/

    dummyServer.shutdown();

    delete[] incom_buf;

    return 0;
}

