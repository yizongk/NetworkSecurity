#include <iostream>
#include "../../../src/backend/endpoint.h"

#define BUFLEN 100

using namespace std;
                                        //TODO: Need to fix output formating, and makefile!
int main(int argc, char* argv[]) {

    if( argc != 2 ) {
        printf("Usage: sudo ./test_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;
    /* testing normal constructor and its functions */
    Endpoint dummy;

    unsigned char *buf = new unsigned char[BUFLEN];
    memset(buf,0,BUFLEN);
    memcpy(buf,"Client test sending message: HAHAHA", 35);

    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    if( !dummy.bootup() ) {
        pass = false;
    }

    if( !dummy.send(buf) ) {
        pass = false;
    } else {
        cout << "Send out:'" << buf << "'" << endl;
    }

    if( !dummy.listen(incom_buf, bytes) ) {
        pass = false;
    } else {
        cout << "Received(" << bytes << " bytes):'" << incom_buf << "'" << endl;
        cout << "\tPacket source addr size: '" << dummy.get_src_addr_len() << "'" << endl;
        cout << "\tPacket sockaddr content: '" << dummy.get_string_src_addr() << "'" << endl;
    }

    if( !dummy.shutdown() ) {
        pass = false;
    }

    printf("-------------------------------------------------------------------\n");
    /* Testing constructor that takes in a interace name and its functions */
    Endpoint dummyItf(argv[1]);
    /* Reusing variable, but reset it first */
    memset(buf,0,BUFLEN);
    memcpy(buf,"Client test sending message: Second constructor NOW", 35);
    bytes = -1;
    memset(incom_buf,0,BUFLEN);

    if( !dummyItf.bootup() ) {
        pass = false;
    }

    if( !dummyItf.send(buf) ) {
        pass = false;
    } else {
        cout << "Send out:'" << buf << "'" << endl;
    }

    if( !dummyItf.listen(incom_buf, bytes) ) {
        pass = false;
    } else {
        cout << "Received(" << bytes << " bytes):'" << incom_buf << "'" << endl;
        cout << "\tPacket source addr size: '" << dummy.get_src_addr_len() << "'" << endl;
        cout << "\tPacket sockaddr content: '" << dummy.get_string_src_addr() << "'" << endl;
    }

    if( !dummyItf.shutdown() ) {
        pass = false;
    }


    delete buf;
    delete incom_buf;
    return 0;
}