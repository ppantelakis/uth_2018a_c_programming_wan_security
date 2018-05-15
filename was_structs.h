//IP Header Length
//https://en.wikipedia.org/wiki/IPv4
//20 bytes = 160 bits
//https://en.wikipedia.org/wiki/Internet_protocol_suite#/media/File:UDP_encapsulation.svg
struct ipheader
{
    unsigned char ip_v:4, ip_hl:4;
    unsigned char ip_tos_ecn;
    unsigned short int ip_len;
    unsigned short int ip_id;
    unsigned short int ip_flags_off;
    unsigned char ip_ttl;
    unsigned char ip_p;
    unsigned short int ip_sum;
    unsigned int ip_src;
    unsigned int ip_dst;
};

//TCP Header Length
//https://en.wikipedia.org/wiki/Transmission_Control_Protocol
//20 Bytes = 160 bits

struct tcpheader
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
struct iplog_t
{
    struct in_addr addr;
    time_t first_time;
    time_t blocked_until_time;
    long   current_port;
};
