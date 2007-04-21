/*  -*- LPC -*-  */
/*
 * $Id: ftpwho.c,v 1.5 2003/05/21 03:46:41 ceres Exp $
 */

inherit "/cmds/base";

#define FTPD "/secure/ftpd"
#define UNIX_FTPD "/secure/ftp_auth"

mixed cmd(string) {
  mixed *who_unix, *who;
  int i;

  if(!find_object(FTPD)) {
    write("MUD FTP is currently disabled.\n");
  } else {
    who = FTPD->query_connections();
    if ((i = sizeof(who))) {
      write("People currently using MUD FTP :\n");
      while (i--)
        write(" - "+who[i]+"\n");
    } else {
      write("No one is using MUD FTP at the moment.\n");
    }
  }
  who_unix = UNIX_FTPD->query_connections();
  if ((i = sizeof(who_unix))) {
    write("People currently using UNIX FTP :\n");
    while (i--)
      write(" - "+who_unix[i]+"\n");
  } else {
    write("No one is using UNIX FTP at the moment.\n");
  }
  return 1;
}
