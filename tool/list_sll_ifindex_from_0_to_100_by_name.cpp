#include <net/if.h>
#include <iostream>
#include <string.h>struct ifreq ifr;

using namespace std;
// List all sll_ifindex by name from 0 to 100
int main() {
	char *buf = new char[100];

	for(int i = 0; i < 100; ++i ) {
		memset(buf,0,100);
		if_indextoname(i,buf);
		printf("buf (%d): '%s'\n",i, buf);
	}

	delete[] buf;
	

	struct ifreq ifr; // Not sure what to do with this one


	return 0;
}
