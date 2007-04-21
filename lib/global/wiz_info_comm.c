/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wiz_info_comm.c,v 1.2 1998/04/13 14:36:08 pinkfish Exp $
 * $Log: wiz_info_comm.c,v $
 * Revision 1.2  1998/04/13 14:36:08  pinkfish
 * Added documentation.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
 */
/**
 * This file contains all the information related creator commands.
 * @author Ember
 */
#define PEOPLER "/obj/handlers/peopler"

protected int do_gauge(string str);
int add_command(string name, object ob, string format, function func);

/** @ignore yes */ 
protected void wiz_commands() {
  add_command("gauge", this_object(), "<string>", (: do_gauge($4[0]) :));
  if(clonep())
    call_out("driver_version", 20);
} /* wiz_commands() */
     

/* small addition by a retired creator aka wonda */
/**
 * This method is called slighty after the login time.  It checks
 * to see if the driver version has changed and pooints to the
 * change log for more information.
 */
int driver_version(){
  string old = this_object()->query_property("driver_version");

  if(!old) {
    return this_object()->add_property("driver_version", __VERSION__);
  }
  if(old != __VERSION__){
    this_object()->add_property("driver_version", __VERSION__);
    write("%^RED%^The driver has been changed to version " + 
          __VERSION__ + ", see /doc/driver/Changes for more information.%^RESET%^\n");
  }
  return 1;
} /* driver_version() */

/** @ignore yes */
int review() {
  PEOPLER->review();
  return 1;
} /* review() */

/**
 * This method returns the number of evaluation cycles the
 * particular command took to run.  It is kind of useful for
 * figuring out how much time a given command takes
 * to execute
 * @param str the command to execute
 * @return 1 on success, 0 on failure
 */
protected int do_gauge(string str) {
  int eval_cost;

  if (this_player(1) != this_object()) return 0;
  if(!str) {
    notify_fail("Usage : gauge command.\n");
    return 0;
  }
  eval_cost = command(str);
  write("\nThe command '" + str + "' took: " + eval_cost + " CPU cycles.\n");
  return 1;
} /* do_gauge() */
