#include <iostream>
#include <string>
#include "../../../src/socket.h"

using namespace std;

/* Passes if the following condition are met:
 * - file descriptor is made 
 * 
 * Consequence of not passing:
 * - opening of socket resulted in no file descriptor
 */
bool test_opening_of_socket(Socket& dummy) {
    cout << "test_opening_of_socket()..." << endl;

    if( dummy.openSocket() == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }
    
    return true;
}

/* Passes if the following condition are met:
 * - file descriptor unassigned
 *
 * Consequence of not passing:
 * - fd could be a bad fd, or fd could be lock, who knows.
 */
bool test_closing_of_socket(Socket& dummy) {
    cout << "test_closing_of_socket()..." << endl;

    if( dummy.closeSocket() == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }
    
    return true;
}

/*  Passess if the following condition are met:
 *  - NOT SURE
 * 
 *  Consequence of not passing:
 *  - there won't be meta-data such as address attached to this socket
 * */
bool test_binding_of_socket(Socket& dummy) {
    cout << "testing_binding_of_socket()" << endl;

    unsigned short sll_protocol = 1;
    int sll_ifindex = 1;

    if( dummy.bindSocket(sll_protocol, sll_ifindex) == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }

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

    // Binding the socket
    pass = test_binding_of_socket(dummy);








    if( pass ) 
        cout << "\n" << "all tests passed" << endl;
    else 
        cout << "\n" << "some tests or all tests failed" << endl;

    return 0;
}