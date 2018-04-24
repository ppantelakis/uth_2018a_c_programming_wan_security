//Function for getting current timestamp
char* was_get_curr_time()
{
    //We need to use pointer due to return the address of variable, otherwise the variable is local
    char *buff_time = malloc(100 * sizeof(char));
    time_t now = time (0);
    strftime (buff_time, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
    return buff_time;
}

//Function for insert iptables rule to iptables firewall
char* was_iptables_add_rule( char * ip )
{
    static char cmd[ CMD_SIZE ];
    //iptables -I INPUT 2 -p tcp -m state --state NEW -m tcp -s 8.8.8.8 -m comment --comment \"was opened port at 12:23 28/02/2018\"  -j ACCEPT
    sprintf( cmd, "iptables -I INPUT %d -p tcp -m state --state NEW -m tcp -s %s -m comment --comment \"was opened port at %s\"  -j ACCEPT", CMD_LINE, ip,  was_get_curr_time());
    syslog( LOG_AUTH, "Executed command : %s ", cmd);
    return cmd;
}

//Function for application signals
//http://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html
//https://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
void was_app_signals( int sig )
{
    if(sig==SIGHUP)
    {
        syslog(LOG_WARNING, "Received SIGHUP signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGTERM)
    {
        syslog(LOG_WARNING, "Received SIGTERM signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGINT)
    {
        syslog(LOG_WARNING, "Received SIGINT signal.");
        exit(EXIT_FAILURE);
    }
    else if(sig==SIGINT)
    {
        syslog(LOG_WARNING, "Received SIGQUIT signal.");
        exit(EXIT_FAILURE);
    }
    else
    {
        syslog(LOG_WARNING, "Unhandled signal (%d) %s", sig, strsignal(sig) );
    }

}

void was_enable_signals()
{
    //Set the log level
    //http://www.gnu.org/software/libc/manual/html_node/setlogmask.html
    //https://linux.die.net/man/3/setlogmask
    setlogmask( LOG_UPTO(LOG_INFO) );
    //http://www.gnu.org/software/libc/manual/html_node/openlog.html
    openlog( "WAS", LOG_CONS, LOG_USER );
    //Initialize signals
    signal(SIGHUP, was_app_signals);
    signal(SIGTERM, was_app_signals);
    signal(SIGINT, was_app_signals);
    signal(SIGQUIT, was_app_signals);
    //Start the Daemon process the core of the programm
    syslog( LOG_INFO, "WAS is starting up" );
    syslog( LOG_INFO, "WAS is trying to start Daemon" );
}

//Created function for Daemon
//https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux

static void was_daemon()
{
    pid_t pid, sid; //pid: Main proccess id, sid:Child proccess id where is running as daemon
    //http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html
    pid = fork();
    if (pid < 0)
    {
        //Couldn't create a child proccess
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        //We are in the main proccess, the operation succeded to create a child proccess so we can exit the main proccess
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Create a new SID for the child process
        //https://linux.die.net/man/2/setsid
        sid = setsid();
        if (sid < 0) {
            // could not create a new session
            exit(EXIT_FAILURE);
        }
    }
    syslog( LOG_INFO, "Daemon was started successfully" );
}
