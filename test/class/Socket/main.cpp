#include <iostream>
#include <string>
#include "../../../src/socket.h"

using namespace std;

/* Passes if the following condition are met:
 * - file descriptor is make (fd != -1)
 */
bool test_opening_of_socket(Socket& dummy) {
    if( dummy.openSocket() == false ) {
        cout << "(X) " << "test_opening_of_socket()..." << endl;
        cout << "\t" << "opening of socket resulted in no file descriptor" << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;

        return false;
    }

    cout << "test_opening_of_socket()..." << endl;
    return true;
}

/* 
 *
 */
bool test_closing_of_socket(Socket& dummy) {
    if( dummy.closeSocket() == false ) {
        cout << "(X) " << "test_closing_of_socket()..." << endl;
        cout << "\t" << "closinging of socket resulted in no file descriptor" << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;

        return false;
    }

    cout << "test_closing_of_socket()..." << endl;
    return true;
}






int main(int argc, char *argv[]) {
    bool pass = false;
    Socket dummy;
    
    /* Feature testing */
    // Opening the socket
    pass = test_opening_of_socket(dummy);

    // Closing the socket
    pass = test_closing_of_socket(dummy);









    if( pass ) 
        cout << "\n" << "all tests passed" << endl;
    else 
        cout << "\n" << "some tests or all tests failed" << endl;

    return 0;
}