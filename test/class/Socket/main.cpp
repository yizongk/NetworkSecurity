#include <iostream>
#include <string>
#include "../../../src/socket.h"

using namespace std;

/* Passes if the following condition are met:
 * - file descriptor is make (fd != -1)
 */
bool test_creation_of_socket(Socket& Dummy) {
    if( Dummy.getFd() == -1 ) {
        cout << "(X) " << "test_creation_of_socket()..." << endl;
        cout<< "\t" << "Creation of socket resulted in no file descriptor" << endl;
        cout << "\t" << "Error(" << Dummy.getErrorCode() << ") - " << Dummy.getErrorStr() << endl;

        return false;
    }

    cout << "test_creation_of_socket()..." << endl;
    return true;
}

int main(int argc, char *argv[]) {
    bool pass = false;
    Socket dummy;
    
    /* Feature testing */
    // Creation of socket
    pass = test_creation_of_socket(dummy);











    if( pass ) 
        cout << "\n" << "all tests passed" << endl;
    else 
        cout << "\n" << "some tests or all tests failed" << endl;

    return 0;
}