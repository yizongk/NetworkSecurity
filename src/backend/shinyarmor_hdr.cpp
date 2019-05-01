#include "shinyarmor_hdr.h"

void reset_shinarmor_hdr(struct shinyarmor_hdr& a) {
    a.packet_id = 0;
    a.eof = false;
    a.dst_port_num = 0;
    a.src_port_num = 0;

    return;
}