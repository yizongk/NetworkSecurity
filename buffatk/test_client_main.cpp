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

    cout << "The first message is length zero." << endl;
    string empty = "";
    memset(out_buf,0,MAX_MSG_LEN);
    memcpy(out_buf,empty.c_str(),empty.size());
    dummyClient.send(out_buf, empty.size(), SERVER_PORT_NUM);
    cout << "-------------------------------------\n" << endl;

    cout << "The second message is length 50." << endl;
    string temp = "thismessageis50byteslong..........................";
    memset(out_buf,0,MAX_MSG_LEN);
    memcpy(out_buf,temp.c_str(),temp.size());
    dummyClient.send(out_buf, temp.size(), SERVER_PORT_NUM);
    cout << "-------------------------------------\n" << endl;
  
    cout << "The third message is max length 100." << endl;
    string temp2 = "thismessageis100byteslong...........................................................................";
    memset(out_buf,0,MAX_MSG_LEN);
    memcpy(out_buf,temp2.c_str(),temp2.size());
    dummyClient.send(out_buf, temp2.size(), SERVER_PORT_NUM);
    cout << "-------------------------------------\n" << endl;

    cout << "The final message exceeds the buffer size, at size 101." << endl;
    string temp3 = "thismessageis101byteslong............................................................................";
    memset(out_buf,0,MAX_MSG_LEN);
    memcpy(out_buf,temp3.c_str(),temp3.size());
    dummyClient.send(out_buf, temp3.size(), SERVER_PORT_NUM);
    cout << "-------------------------------------\n" << endl;

    dummyClient.shutdown();

    delete[] out_buf;

    return 0;
}

