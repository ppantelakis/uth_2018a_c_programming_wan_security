#include "was_functions.h"
#include "was_tcp_globals.h"
#include "was_tcp_structs.h"

//Function to add a firewall rule to iptables
char* was_tcp_iptables_add_rule( char *);
//Function to get the last element of the list of ips. If there are not elemenst on the list return NULL
struct tcp_iplog_t * was_tcp_iplog_get_last();
//Function to return the total elements in the arry of ips
long was_tcp_iplog_get_total();
//Check if a specific ip exists to the list of ips
//Return null if does not exist this ip
struct tcp_iplog_t * was_tcp_iplog_find(struct in_addr);

//Check if IP is blocked
int was_tcp_iplog_is_blocked(struct tcp_iplog_t *);

//Block IP for PORT_SCANNING_WAIT
void was_tcp_iplog_block(struct tcp_iplog_t *);

//Remove ip from array of ips
void was_tcp_iplog_remove(struct tcp_iplog_t *iplog_ptr);

//Allocate memory for new ip. Return pointer of new or NULL if not succeeds.
struct tcp_iplog_t * was_tcp_iplog_alloc();

//Add an ip to array of ips
void was_tcp_iplog_add(struct in_addr,long);

int was_tcp_listen();

