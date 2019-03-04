#include <iostream>
#include "../../../src/backend/server.h"

#define BUFLEN 100

using namespace std;
                                        //TODO: Need to fix output formating, and makefile!
int main(int argc, char* argv[]) {

    bool pass = true;
    Server dummy;

    unsigned char *buf = new unsigned char[BUFLEN];
    memset(buf,0,BUFLEN);
    memcpy(buf,"Server test sending message: HAHAHA", 35);

    ssize_t bytes = -1;
    unsigned char *incom_buf = new unsigned char[BUFLEN];
    memset(buf,0,BUFLEN);

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
    }

    if( !dummy.shutdown() ) {
        pass = false;
    }

    delete buf;
    delete incom_buf;
    return 0;
}