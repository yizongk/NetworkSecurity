#include "shinyarmor_hdr.h"

#define MAX_MSG_LEN 100
#define BUFLEN sizeof(struct shinyarmor_hdr) + MAX_MSG_LEN  /* BUG: magic number is 14 here, anything len below 14 will cause errno of invalid argument to system call for some reason. */
#define PORT_NUM 8000
#define PORT_NUM2 8001
