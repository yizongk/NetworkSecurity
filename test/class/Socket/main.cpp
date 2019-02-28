#include <iostream>
#include <string>
#include <linux/if_ether.h>
#include <net/if.h>
#include <arpa/inet.h>
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
    cout << "test_binding_of_socket()..." << endl;

    unsigned short sll_protocol = htons(ETH_P_ALL);
    int sll_ifindex = 1;        

    if( dummy.bindSocket() == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }

    return true;
}

/*  Passess if the following condition are met:
 *  - Message is send through socket with no errno
 * 
 *  Consequence of not passing:
 *  - Socket is not sending msg
 *  */
bool test_sendmsg_of_socket(Socket& dummy) {
    cout << "test_sendmsg_of_socket()..." << endl;

    size_t buff_len = 100;
    unsigned char *buff = new unsigned char[buff_len];

    if( dummy.sendMsg(buff,buff_len,0) == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }

    delete[] buff;
    return true;
}

/*  Passess if the following condition are met:
 *  - Message is recieved from socket to buf
 * 
 *  Consequence of not passing:
 *  - Socket is not recieving msg
 *  */
bool test_recvmsg_of_socket(Socket& dummy) {
    cout << "test_recvmsg_of_socket()..." << endl;

    size_t buff_len = 100;
    unsigned char *buff = new unsigned char[buff_len];

    if( dummy.recvMsg(buff,buff_len) == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }

    delete[] buff;
    return true;
}



int main(int argc, char *argv[]) {
    bool pass = true;
    Socket dummy;
    
    /* Feature testing */
    // Opening the socket
    if(!test_opening_of_socket(dummy)) 
        pass = false;

    // Binding the socket
    if(!test_binding_of_socket(dummy))
        pass = false;

    // Send message through socket
    if(!test_sendmsg_of_socket(dummy))
        pass = false;

    // Recieve message from socket
    if(!test_recvmsg_of_socket(dummy))
        pass = false;
    
    // Closing the socket
    if(!test_closing_of_socket(dummy)) 
        pass = false;

    








    if( pass ) 
        cout << "---\n" << "all tests passed" << endl;
    else 
        cout << "---\n" << "some tests or all tests failed" << endl;

    return 0;
}