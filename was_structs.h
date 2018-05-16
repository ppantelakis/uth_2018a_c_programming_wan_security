//IP Header Length
//https://en.wikipedia.org/wiki/IPv4
//20 bytes = 160 bits
//https://en.wikipedia.org/wiki/Internet_protocol_suite#/media/File:UDP_encapsulation.svg
struct ip_header
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
