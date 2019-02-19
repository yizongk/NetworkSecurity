#include <iostream>
#include <string>
#include "../../../src/socket.h"

using namespace std;

/* Passes if the following condition are met:
 * - file descriptor is make (fd != -1)
 */
bool test_creation_of_socket(Socket& Dummy) {
    if( Dummy.getFd() == -1 )
        return false;
    return true;
}

int main(int argc, char *argv[]) {
    bool pass = false;
    Socket dummy;
    
    /* Feature testing */
    // Creation of socket
    pass = test_creation_of_socket(dummy);

    if( pass ) 
        cout << "all tests passed" << endl;
    else 
        cout << "some tests or all tests failed" << endl;

    return 0;
}