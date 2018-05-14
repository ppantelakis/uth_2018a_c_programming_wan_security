//Function for getting current timestamp in milliseconds
//https://stackoverflow.com/questions/12722904/how-to-use-struct-timeval-to-get-the-execution-time
long int was_get_curr_time_ms();

//Function for getting current timestamp
char* was_get_curr_time();

//Function for insert iptables rule to iptables firewall
char* was_iptables_add_rule( char * ip );

//Function for application signals
//http://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html
//https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
void was_app_signals( int sig );

void was_enable_signals();

//Created function for Daemon
//https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux

void was_daemon();

void was_listen_port2(struct in_addr);

void was_listen();
