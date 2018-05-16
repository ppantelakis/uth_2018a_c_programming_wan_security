#include "was_structs.h"
//TCP Header Length
//https://en.wikipedia.org/wiki/Transmission_Control_Protocol
//20 Bytes = 160 bits

struct tcp_header
{
    unsigned short int th_sport;
    unsigned short int th_dport;
    unsigned int th_seq;
    unsigned int th_ack;
    unsigned char th_x2:4, th_off:4;
    unsigned char th_flags;
    unsigned short int th_win;
    unsigned short int th_sum;
    unsigned short int th_urp;
};

//Struct of ip log
struct tcp_iplog_t
{
    struct in_addr addr;
    time_t first_time;
    time_t blocked_until_time;
    long   current_port;
};

