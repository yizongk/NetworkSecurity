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
    // Default constructor for usage in passing this struct into another function that can give the param struct a default value, in the case that no param for the struct is passed into that function.
    shinyarmor_hdr() : packet_id(0), eof(false), dst_port_num(0), src_port_num(0) { }

    unsigned int    packet_id;
    bool            eof;        // If true, this is the last packet of the entire transmission.
    unsigned int    dst_port_num;              // Specify the port number.
    unsigned int    src_port_num;           // The port number of the source of the transmission.
};

void reset_shinarmor_hdr(struct shinyarmor_hdr&);

#endif