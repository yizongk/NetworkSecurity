#include <iostream>
#include "../../../src/backend/endpoint.h"

#define BUFLEN 100
#define PORTNUM 1000

using namespace std;

bool test_zero_arg_constructor() {

    bool pass = true;

    /* testing normal constructor and its functions */
    Endpoint dummy;

    unsigned char *buf = new unsigned char[BUFLEN];
    memset(buf,0,BUFLEN);
    memcpy(buf,"Server test sending message: HAHAHA", 35);

    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    if( !dummy.bootup() ) {
        pass = false;
    }

    if( !dummy.send(buf, BUFLEN, PORTNUM) ) {
        pass = false;
    } else {
        cout << "Send out:'" << buf << "'" << endl;
    }

    if( !dummy.listen(incom_buf, bytes, PORTNUM) ) {
        pass = false;
    } else {
        cout << "Received(" << bytes << " bytes):'" << incom_buf << "'" << endl;
    }

    if( !dummy.shutdown() ) {
        pass = false;
    }

    delete buf;
    delete incom_buf;


    return true;
}

bool test_cml_arg_constructor(char* argv[]) {

    bool pass = true;
 
    unsigned char *buf = new unsigned char[BUFLEN];
    memset(buf,0,BUFLEN);
    memcpy(buf,"Server test sending message: HAHAHA", 35);

    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(incom_buf,0,BUFLEN);

    /* Testing constructor that takes in a interace name and its functions */
    Endpoint dummyItf(argv[1]);
    /* Reusing variable, but reset it first */
    memset(buf,0,BUFLEN);
    memcpy(buf,"Server test sending message: Second constructor NOW", 35);
    bytes = -1;
    memset(incom_buf,0,BUFLEN);

    if( !dummyItf.bootup() ) {
        pass = false;
    }

    if( !dummyItf.send(buf, BUFLEN, PORTNUM) ) {
        pass = false;
    } else {
        cout << "Send out:'" << buf << "'" << endl;
    }

    if( !dummyItf.listen(incom_buf, bytes, PORTNUM) ) {
        pass = false;
    } else {
        cout << "Received(" << bytes << " bytes):'" << incom_buf << "'" << endl;
    }

    if( !dummyItf.shutdown() ) {
        pass = false;
    }


    delete buf;
    delete incom_buf;


    return pass;
}

                                        //TODO: Need to fix output formating, and makefile!
int main(int argc, char* argv[]) {

    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    cout << "-----------------PHASE 1 constructor with no argument-----------------" << endl;
    if( !test_zero_arg_constructor() ) 
        pass = false;

    cout << "-----------------PHASE 2 constructor with command line argument-----------------" << endl;
    if( !test_cml_arg_constructor(argv) )
        pass = false; 
    
    if( pass ) 
        cout << "\n---\n" << "all tests passed" << endl;
    else 
        cout << "\n---\n" << "some tests or all tests failed" << endl;

    return 0;
}