#include <stdio.h>
#define PORT1 43657
#define PORT2 55981
//IP Header Length 
//https://en.wikipedia.org/wiki/IPv4
//20 bytes = 160 bits
//
//TCP Header Length
//https://en.wikipedia.org/wiki/Transmission_Control_Protocol
//20 Bytes = 160 bits
main()
{
	printf("Port sequence first:%d, second:%d\n",PORT1,PORT2);
	return 0;
}
