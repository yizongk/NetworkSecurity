#include <iostream>
#include <string>
#include <linux/if_ether.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "../../../src/backend/socket.h"

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
    memset(buff,0,buff_len);
    memcpy( buff, "Hello World! YOU!", buff_len );

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

    ssize_t bytes = -1;
    size_t buff_len = 100;
    unsigned char *buff = new unsigned char[buff_len];

    if( dummy.recvMsg(buff,buff_len,bytes) == false ) {
        cout << "(X) " << endl;
        cout << "\t" << "error(" << dummy.getErrorCode() << ") - " << dummy.getErrorStr() << endl;
        return false;
    }

    printf("\tGot Message(%ld bytes):'%s'\n", bytes, buff);

    delete[] buff;
    return true;
}



int main(int argc, char *argv[]) {

    if( argc != 2 ) {
        printf("Usage: sudo ./test_socket_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;
    Socket dummy;               // do a test for interace argu
    
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

    printf("\n---------------------------------------\n");
    /* Testing the second constructor */
    Socket dummy1(argv[1]);

    // Opening the socket
    if(!test_opening_of_socket(dummy1)) 
        pass = false;

    // Binding the socket
    if(!test_binding_of_socket(dummy1))
        pass = false;

    // Send message through socket
    if(!test_sendmsg_of_socket(dummy1))
        pass = false;

    // Recieve message from socket
    if(!test_recvmsg_of_socket(dummy1))
        pass = false;
    
    // Closing the socket
    if(!test_closing_of_socket(dummy1)) 
        pass = false;









    if( pass ) 
        cout << "\n---\n" << "all tests passed" << endl;
    else 
        cout << "\n---\n" << "some tests or all tests failed" << endl;

    return 0;
}