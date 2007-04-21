/* 
 * written by Dwayne Fontenot (Jacques)
 * last modified: 1992 October 19 (runs on the Basis mudlib)
 *
 * This object implements a telnet client (providing a subset of the telnet
 * protocol) using STREAM mode of MudOS 0.9 LPC sockets.  See the init()
 * function // to find out the commands this terminal understands.
 * This object may be used from within a MudOS mud to connect to any
 * networked server that understands the telnet protocol (including
 * another LPmud).
 */

#include <socket.h>
#include <socket_errors.h>

#define WRITE_WAIT_CALLBACK 0
#define WRITE_GO_AHEAD      1

#define DISCONNECTED "an internet terminal"
#define CONNECTED "an internet terminal (connected)"

#define IAC  255
#define DONT 254
#define DO   253
#define WONT 252
#define WILL 251

#define TELOPT_ECHO   1
#define TELOPT_SGA    3
#define TELOPT_TTYPE 24
#define TELOPT_NAWS  31

nosave string *telopts = ({"BINARY", "ECHO", "RCP", "SUPPRESS GO AHEAD",
			"NAME", "STATUS", "TIMING MARK", "RCTE", "NAOL", "NAOP",
		    "NAOCRD", "NAOHTS", "NAOHTD", "NAOFFD", "NAOVTS",
		    "NAOVTD", "NAOLFD", "EXTEND ASCII", "LOGOUT", "BYTE MACRO",
		    "DATA ENTRY TERMINAL", "SUPDUP", "SUPDUP OUTPUT",
		    "SEND LOCATION", "TERMINAL TYPE", "END OF RECORD",
		    "TACACS UID", "OUTPUT MARKING", "TTYLOC",
		    "3270 REGIME", "X.3 PAD", "NAWS", "TSPEED", "LFLOW",
		    "LINEMODE"});

nosave string s_iac_dont_echo;
nosave string s_iac_do_echo;
nosave string s_iac_wont_echo;
nosave string s_iac_will_echo;
nosave string s_iac_dont_sga;
nosave string s_iac_do_sga;
nosave string s_iac_wont_sga;
nosave string s_iac_will_sga;
nosave string s_iac_wont_ttype;
nosave string s_iac_wont_naws;
nosave string s_iac;
nosave string s_dont_echo;
nosave string s_do_echo;

private string callback;

private int conn_fd;
private int connected;
private int verbose;

private int write_state = WRITE_WAIT_CALLBACK;
private string write_message = "";

protected void init_tel_neg()
{
  s_iac_dont_echo  = sprintf("%c%c%c",IAC,DONT,TELOPT_ECHO);
  s_iac_do_echo    = sprintf("%c%c%c",IAC,DO  ,TELOPT_ECHO);
  s_iac_wont_echo  = sprintf("%c%c%c",IAC,WONT,TELOPT_ECHO);
  s_iac_will_echo  = sprintf("%c%c%c",IAC,WILL,TELOPT_ECHO);
  s_iac_dont_sga   = sprintf("%c%c%c",IAC,DONT,TELOPT_SGA);
  s_iac_do_sga     = sprintf("%c%c%c",IAC,DO  ,TELOPT_SGA);
  s_iac_wont_sga   = sprintf("%c%c%c",IAC,WONT,TELOPT_SGA);
  s_iac_will_sga   = sprintf("%c%c%c",IAC,WILL,TELOPT_SGA);
  s_iac_wont_ttype = sprintf("%c%c%c",IAC,WONT,TELOPT_TTYPE);
  s_iac_wont_naws  = sprintf("%c%c%c",IAC,WONT,TELOPT_NAWS);
  s_iac            = sprintf("%c",    IAC);
  s_dont_echo      = sprintf("%c%c",  DONT,TELOPT_ECHO);
  s_do_echo        = sprintf("%c%c",  DO,  TELOPT_ECHO);
}

void
set_callback(string arg)
{
	callback = arg;
}

void create()
{
  init_tel_neg();
  connected = 0;
  verbose = 0;
  set_callback("handler");
}

void
set_verbosity(int v)
{
	verbose = v;
}

int
query_connected()
{
	return connected;
}

void disconnected()
{
  call_other(this_object(), callback, "close");
  connected = 0;
}

int connected()
{
  call_other(this_object(), callback, "open");
  connected = 1;
}

void my_socket_write(int fd, string message)
{
  int ret;

  write_message = write_message + message;
  if(write_state == WRITE_GO_AHEAD){
    ret = socket_write(fd, write_message);
    write_message = "";
    if(ret == EESUCCESS) write_state = WRITE_GO_AHEAD;
    else if(ret == EECALLBACK) write_state = WRITE_WAIT_CALLBACK;
  }
}

int line(string str)
{
  if(connected){
    my_socket_write(conn_fd,s_iac_dont_sga+s_iac_dont_echo);
    write("SENT dont SUPPRESS GO AHEAD\nSENT dont ECHO\n");
    return(1);
  }
  return(0);
}

int char(string str)
{
  if(connected){
    my_socket_write(conn_fd,s_iac_do_sga+s_iac_do_echo);
    write("SENT do SUPPRESS GO AHEAD\nSENT do ECHO\n");
    return(1);
  }
  return(0);
}

int connect(string str)
{
  int ret;

  if(!str) return(0);
  conn_fd = socket_create(STREAM,"socket_shutdown");
  ret = socket_connect(conn_fd,str,"receive_data","write_data");
  if(ret != EESUCCESS){
    notify_fail("unable to connect: " + socket_error(ret) + "\n");
    return(0);
  }
  connected();
  return(1);
}

int send(string str)
{
  if(connected){
    if(!str){
      write("Sending CR.\n");
      my_socket_write(conn_fd,"\n");
      return(1);
    }
    my_socket_write(conn_fd,str + "\n");
    return(1);
  }
  return(0);
}

int disconnect(string str)
{
  int ret;

  ret = socket_close(conn_fd);
  if(ret <= 0){
    notify_fail("unable to disconnect.\n");
    return(0);
  }
  disconnected();
  return(1);
}

void receive_data(int rec_fd, string msg) {
  string *chunks;
  int i;
  object hearer;

  this_object()->recieve_message(msg);
} /* recieve_message() */

void write_data(int fd) {
  write_state = WRITE_GO_AHEAD;
  my_socket_write(fd,"");
} /* write_data() */

void socket_shutdown(int fd)
{
  object hearer;

  if(fd == conn_fd){
    hearer = environment(this_object());
    disconnected();
    return;
  }
}
