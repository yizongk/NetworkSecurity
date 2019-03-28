#include <iostream>
#include <iomanip>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For client to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *out_buf = new unsigned char[MAX_MSG_LEN];
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(out_buf,0,MAX_MSG_LEN);

    Endpoint dummyClient(argv[1]);
    dummyClient.bootup();

    /* Allow client to send 10 messages to server */
    for(int i  = 0; i < 10; ++i) {
        
        dummyClient.run_protocol_send(out_buf, PORT_NUM);
/*
        string temp = "";
        cout << "Enter a message(less than 100 characters):" << endl;
        getline(cin, temp);
        if(temp.size() >= 100) {
            cout << "message too long(less than 100 characters)." << endl;
            cout << "-------------------------------------\n" << endl;
            continue;
        }

        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        dummyClient.send(out_buf, temp.size(), PORT_NUM);
        cout << "-------------------------------------\n" << endl;
*/  
    }
    dummyClient.shutdown();

    delete out_buf;

    return 0;
}

