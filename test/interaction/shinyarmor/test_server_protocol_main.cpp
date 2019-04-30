#include <iostream>
#include <iomanip>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_protocol_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    // initialize random seed for server instance
    srand(time(NULL));

    bool pass = true;

    /* For server to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    Endpoint dummyServer(argv[1], SERVER_PORT_NUM);
    dummyServer.bootup();

    while(true) {
        memset(incom_buf,0,BUFLEN);
        if( dummyServer.run_protocol_listen(incom_buf, bytes) ) {

            /* cout << "Payload received(" << bytes << " bytes):" << endl << "'";
            for(int j = 0; j < bytes; ++j) {
                cout << std::hex << (int)incom_buf[j];
                //cout << (char)incom_buf[j];
            }
            cout << std::dec << "'" << endl; */

            printf("---------------------------------------------\n");


        }
    }
    dummyServer.shutdown();

    delete incom_buf;

    return 0;
}

