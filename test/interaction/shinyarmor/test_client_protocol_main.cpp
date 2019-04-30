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

    // initialize random seed for client instance
    srand(time(NULL));

    /* For client to send/recieve off to */
    unsigned char *out_buf = new unsigned char[MAX_MSG_LEN];
    memset(out_buf,0,MAX_MSG_LEN);

    Endpoint dummyClient(argv[1], CLIENT_PORT_NUM);
    dummyClient.bootup();

    while(true) {

        string temp = "";
        cout << "Enter a message(less than 100 characters)" << endl;
        getline(cin, temp);
        if(temp.size() >= 100) {     //should make sure the temp.size() is converted to number of bytes small than MAX_MSG_LEN
            cout << "message too long(less than 100 characters)" << endl;
            cout << "-------------------------------------\n" << endl;
            continue;
        }

        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        cout << "DEBUG: Buf:'";
        for( int i = 0; i < MAX_MSG_LEN; ++i ) {
            cout << std::hex << (int)out_buf[i];
        }
        cout << std::dec << "' (Payload to be sent)" << endl;
        dummyClient.run_protocol_send(out_buf, temp.length(), SERVER_PORT_NUM);



        /* cout << "Enter a handshake" << endl;
        getline(cin, temp);
        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        dummyClient.send(out_buf, temp.length(), SERVER_PORT_NUM);

        unsigned char *in_buf = new unsigned char[MAX_MSG_LEN];
        memset(in_buf,0,MAX_MSG_LEN);
        ssize_t in_bytes = -1;
        dummyClient.listen(in_buf,in_bytes);

        cout << "Enter a payload" << endl;
        getline(cin, temp);
        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        dummyClient.send(out_buf, temp.length(), SERVER_PORT_NUM); */
        


        cout << "-------------------------------------\n" << endl;
    }
    dummyClient.shutdown();

    delete out_buf;

    return 0;
}

