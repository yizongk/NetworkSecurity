#include <iostream>
#include <iomanip>
#include <string>
#include "../../../src/backend/endpoint.h"
#include "../../../src/backend/constant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_client_protocol_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    bool pass = true;

    /* For client to send/recieve off to */
    unsigned char *out_buf = new unsigned char[MAX_MSG_LEN];
    memset(out_buf,0,MAX_MSG_LEN);

    Endpoint dummyClient(argv[1], CLIENT_PORT_NUM);
    dummyClient.bootup();

    while(true) {

        string temp = "";
        cout << "Enter a message(less than 100 characters)" << endl;
        getline(cin, temp);
        
        if(temp.size() >= 100) {     //should make sure the temp.size() is converted to number of bytes small than MAX_MSG_LEN
            cout << "message too long(less than 100 characters)" << endl;
            cout << "-------------------------------------\n" << endl;
            
            int curr_index = 0;
            size_t chunk_s = MAX_MSG_LEN;
            while(curr_index < temp.size()){

                string temp_partial = "";
                
                temp_partial = temp.substr(curr_index, chunk_s);
                memset(out_buf,0,chunk_s);
                memcpy(out_buf, temp_partial.c_str(), temp_partial.size());
                
                cout << "DEBUG: Buf:'";
                for( int i = 0; i < chunk_s; ++i ) {
                    cout << std::hex << (int)out_buf[i];
                }
        
                cout << std::dec << "' (Payload to be sent)" << endl;
                dummyClient.run_protocol_send(out_buf, temp_partial.length(), SERVER_PORT_NUM);
                cout << "-------------------------------------\n" << endl;

                int last_chunk = temp.size() - curr_index;
                if(curr_index < temp.size() && last_chunk < 100){

                    chunk_s = last_chunk;
                }
                curr_index += 100;


             }


            //continue;
        }

        memset(out_buf,0,MAX_MSG_LEN);
        memcpy(out_buf,temp.c_str(),temp.size());
        cout << "DEBUG: Buf:'";
        for( int i = 0; i < MAX_MSG_LEN; ++i ) {
            cout << std::hex << (int)out_buf[i];
        }
        cout << std::dec << "' (Payload to be sent)" << endl;
        dummyClient.run_protocol_send(out_buf, temp.length(), SERVER_PORT_NUM);
        cout << "-------------------------------------\n" << endl;
    }
    dummyClient.shutdown();

    delete out_buf;

    return 0;
}

