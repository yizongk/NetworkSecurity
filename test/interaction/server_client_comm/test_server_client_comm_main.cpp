#include <iostream>
#include "../../../src/backend/server.h"
#include "../../../src/backend/client.h"

using namespace std;

int main(int argc, char *argv[]) {
    if( argc != 2 ) {
        printf("Usage: sudo ./test_server_client_main 'interface name(such as lo, wlp59s0)'\n");
        return 0;
    }

    return 0;
}