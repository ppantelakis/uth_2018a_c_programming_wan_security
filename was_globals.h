//First Port
#define PORT1 43657

//Second Port
#define PORT2 55981

//Command size
#define CMD_SIZE        512

//Command line in iptables usually 2-3 first lines are used for loopback and established connections
#define CMD_LINE        3

//Read buffer size
#define BUFFER_SIZE      8192

//How many seconds wait for second hit
//Be carefully with this constant on slow networks
#define MAX_WAIT_SECOND_HIT 4
