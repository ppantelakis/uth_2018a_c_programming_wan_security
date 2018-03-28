//Function for getting current timestamp
char* get_curr_time()
{
    //We need to use pointer due to return the address of variable, otherwise the variable is local
    char *buff_time = malloc(100 * sizeof(char));
    time_t now = time (0);
    strftime (buff_time, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
    return buff_time;
}

//Function for insert iptables rule to iptables firewall
char* iptables_add_rule( char * ip )
{
    static char cmd[ CMD_SIZE ];
    //iptables -I INPUT 2 -p tcp -m state --state NEW -m tcp -s 8.8.8.8 -m comment --comment \"was opened port at 12:23 28/02/2018\"  -j ACCEPT
    sprintf( cmd, "iptables -I INPUT %d -p tcp -m state --state NEW -m tcp -s %s -m comment --comment \"was opened port at %s\"  -j ACCEPT", CMD_LINE, ip,  get_curr_time());
    return cmd;
}

//Function for application signals
void app_signals( int sig )
{
    syslog(LOG_WARNING, "Received SIGHUP signal.");
    syslog(LOG_WARNING, "Unhandled signal (%d) %s", sig, strsignal(sig) );
}
