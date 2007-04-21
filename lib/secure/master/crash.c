/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: crash.c,v 1.3 2003/03/25 20:02:49 wodan Exp $
 */
void crash(string crash_mess, object command_giver, object current_object) {
  object thing, *things;
  string crashtxt;

  reset_eval_cost();
  log_file("CRASH", "\n"+ ctime(time()) +":\n");
  if (current_object)
    log_file("CRASH", "current object: "+ file_name(current_object) +
             " ("+ current_object->query_name() +")\n");
  if (command_giver)
    log_file("CRASH", "command giver: "+ file_name(command_giver) +
             " ("+ (string)command_giver->query_name() +")\n");
  if (query_verb())
    log_file("CRASH", "command given: "+ query_verb() +"\n");
  log_file("CRASH", "crash reason: "+ crash_mess +"\n");
  things = users();
  log_file("CRASH", "["+ implode((string *)things->query_name(), ", ") +
           "]\n");
  flush_log_files();
  crashtxt = "Wodan "+({"says: I wonder what this button does...",
                        "says: Ceres, look out for that wire....",
                        "says: Wow, look at the uptime.",
                        "whispers: I don't think we should be doing this on my desk..",
                        "shouts: Look at all that xp!"})[random(5)];
  foreach (thing in things) {
    reset_eval_cost();
    efun::tell_object(thing, crashtxt + "\n("+ crash_mess +")\n");
    catch(thing->quit());
  }
} /* crash() */
