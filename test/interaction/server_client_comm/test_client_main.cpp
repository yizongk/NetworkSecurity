#include <iostream>
#include <iomanip>
#include "../../../src/backend/server.h"
#include "../../../src/backend/client.h"

#define BUFLEN 100

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For client to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *out_buf = new unsigned char[BUFLEN];
    memset(out_buf,0,BUFLEN);

    Client dummyClient(argv[1]);
    dummyClient.bootup();

    /* Allow client to send 10 messages to server */
    for(int i  = 0; i < 10; ++i) {
        string temp = "";
        cout << "Enter a message(less than 100 characters):" << endl;
        getline(cin, temp);
        if(temp.size() >= 100) {
            cout << "message too long(less than 100 characters)." << endl;
            cout << "-------------------------------------\n" << endl;
            continue;
        }

        memset(out_buf,0,BUFLEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        dummyClient.send(out_buf);
        cout << "-------------------------------------\n" << endl;
    }
    dummyClient.shutdown();

    delete out_buf;

    return 0;
}

