//IP Header Length 
//https://en.wikipedia.org/wiki/IPv4
//20 bytes = 160 bits

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