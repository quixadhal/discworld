/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: usage.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: usage.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/

mixed cmd(string arg) {
	mapping info;

  info = rusage();
	
  printf("Current statistics are:\n"+
				 "Memory: %d\nMajorfaults: %d\nUser time: %d\nSystem time: %d\n",
				 info["maxrss"], info["majflt"], info["minflt"],
				 info["utime"] / 1000, info["stime"] / 1000);
  return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
