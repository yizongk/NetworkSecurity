#include <iostream>
#include <iomanip>
#include "../../../src/backend/server.h"
#include "../../../src/backend/client.h"

#define BUFLEN 100

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For server to send/recieve off to */
    unsigned char *out_buf = new unsigned char[BUFLEN];
    memset(out_buf,0,BUFLEN);
    memcpy(out_buf,"Client test sending message: HAHAHA", 35);

    /* For client to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    Server dummyServer(argv[1]);
    Client dummyClient(argv[1]);
    dummyServer.bootup();
    dummyClient.bootup();
    /* PROTOCOL GOES HERE I GUTESS? for testing only */
    for(int i  = 0; i < 10; ++i) {
        if( dummyServer.listen(incom_buf, bytes) ) {

            /* printf("%d. Received(%zu):\n'",i,bytes);
            for(int j=0;j<bytes;++j) {
                printf("%02x ",incom_buf[j]);
            }
            printf("'\n"); */

            cout << i << ". Received(" << bytes << " bytes):" << endl << "'";
            for(int j = 0; j < bytes; ++j) {
                cout << std::hex << (int)incom_buf[j];
            }
            cout << std::dec << "'" << endl;
        }
    }
    dummyServer.shutdown();
    dummyClient.shutdown();

    delete out_buf;
    delete incom_buf;

    return 0;
}