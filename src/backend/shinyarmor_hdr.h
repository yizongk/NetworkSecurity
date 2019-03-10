/* 
 * Author: Yi Zong Kuang
 * This header is designed for packets passed through sockets
 * This header is intended to be appended after any TCP/IP header(if there is any), and before any payload
 * So any packets will look something like this [TCP/IP][shinyarmor_hdr][payload]
 * 
 *  */

#ifndef SHINYARMOR_HDR_H
#define SHINYARMOR_HDR_H

struct shinyarmor_hdr {
    bool eop = true;        // If true, this is the last packet of the entire transmission
};

#endif