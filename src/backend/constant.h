#include "shinyarmor_hdr.h"

#define MAX_MSG_LEN 100
#define BUFLEN sizeof(struct shinyarmor_hdr) + MAX_MSG_LEN  /* BUG: magic number is 14 here, anything len below 14 will cause errno of invalid argument to system call for some reason. */
#define SERVER_PORT_NUM 8000
#define CLIENT_PORT_NUM 8001

/* Below are place holders, for regression testing, where old pieces of code do not have certain parameters to newer version of certain functions
 * it assumes the newer functions will have parameter that can take default values.
 */
extern struct shinyarmor_hdr PLACE_HOLDER_SHINYARMOR;
