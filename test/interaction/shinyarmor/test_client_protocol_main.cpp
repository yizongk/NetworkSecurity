#include <iostream>
#include <iomanip>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_client_protocol_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For client to send/recieve off to */
    ssize_t bytes = -1;
    unsigned char *out_buf = new unsigned char[MAX_MSG_LEN];
    memset(out_buf,0,MAX_MSG_LEN);

    Endpoint dummyClient(argv[1]);
    dummyClient.bootup();

    while(true) {
        string temp = "";
        cout << "Enter a message(less than 10 characters) the 10 char dones't apply just yet:" << endl;
        getline(cin, temp);
        if(temp.size() >= 10) {     //should amek sure the tmep.size() is converted to number of bytes small than MAX_MSG_LEN
            cout << "message too long(less than 10 characters) the 10 char dones't apply just yet." << endl;
            cout << "-------------------------------------\n" << endl;
            continue;
        }

        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        cout << "DEBUG: Buf:'";
        for( int i = 0; i < MAX_MSG_LEN; ++i ) {
            cout << std::hex << (int)out_buf[i];
        }
        cout << std::dec << "' (passing into)" << endl;
        dummyClient.send(out_buf, temp.size(), PORT_NUM);
        cout << "-------------------------------------\n" << endl;
    }
    dummyClient.shutdown();

    delete out_buf;

    return 0;
}

