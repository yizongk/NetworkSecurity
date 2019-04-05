#include <iostream>
#include <iomanip>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For server to send/recieve off to */
    unsigned char *out_buf = new unsigned char[MAX_MSG_LEN];
    memset(out_buf,0,MAX_MSG_LEN);
    memcpy(out_buf,"Client test sending message: HAHAHA", 35);

    /* For client to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[MAX_MSG_LEN];
    memset(incom_buf,0,MAX_MSG_LEN);

    Endpoint dummyServer(argv[1]);
    Endpoint dummyClient(argv[1]);
    dummyServer.bootup();
    dummyClient.bootup();
    /* PROTOCOL GOES HERE I GUTESS? for testing only */
    for(int i  = 0; i < 1; ++i) {

        if( !dummyClient.send(out_buf, MAX_MSG_LEN, SERVER_PORT_NUM) )
            pass = false;

        memset(incom_buf,0,MAX_MSG_LEN);
        if( dummyServer.listen(incom_buf, bytes) ) {

            cout << i << ". Received(" << bytes << " bytes):" << endl << "'";
            for(int j = 0; j < bytes; ++j) {
                cout << std::hex << (int)incom_buf[j];
            }
            cout << std::dec << "'" << endl;
        }
    }
    dummyServer.shutdown();
    dummyClient.shutdown();

    delete[] out_buf;
    delete[] incom_buf;

    if( pass ) 
        cout << "\n---\n" << "all tests passed" << endl;
    else 
        cout << "\n---\n" << "some tests or all tests failed" << endl;


    return 0;
}