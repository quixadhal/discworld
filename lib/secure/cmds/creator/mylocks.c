#include <creator.h>

inherit "/cmds/base";

int cmd(string person)  {
   string *locked;
   string who_string;

   if(person == "verify") {
     locked = RCS_HANDLER->query_locks( this_player() );
     locked = sort_array( locked, 1 );
     call_out("verify_lock", 1, this_player(), locked);
     write("Verifying\n");
     return 1;
   }
   
   if (!person  ||  file_size(person) == -2)  {
      who_string = "You have ";
      locked = RCS_HANDLER->query_locks( this_player() );
      if (person)
         locked = filter(locked, (: strsrch($1, $(person)) == 0 :));
   }
   else  {
      who_string = capitalize( person ) + " has ";
      locked = RCS_HANDLER->query_locks( person );
   }

   if ( !sizeof( locked ) ) {
      add_failed_mess( who_string + "no files locked.\n" );
      return -1;
   }

   locked = sort_array( locked, 1 );

   write( who_string + "the following files locked:\n" +
     implode(locked, "\n") + "\n");
   return 1;
} /* cmd() */

void verify_lock(object pl, string *locks) {
  string file, lockline, *bits;
  int remove;
  
  file = locks[0];
  bits = explode(file, "/");
  file = "/" + (string)implode(bits[0..<2], "/") + "/RCS/" + bits[<1] + ",v";
  
  if(file_size(file) < 0)
    remove = 1;
  else {
    lockline = read_file(file, 4, 1);
    if (strsrch(lockline, "locks; strict:") == -1) {
      bits = explode(read_file(file, 5, 1), ":");
      if(bits[0][0] != '\t')
        remove = 1;
    }
  }
  
  if(remove) {
    tell_object(pl, "Removing non-existant lock for " + locks[0] + "\n");
    RCS_HANDLER->remove_lock(pl->query_name(), locks[0]);
  }
  
  if(sizeof(locks) > 1)
    call_out("verify_lock", 1, this_player(), locks[1..]);
}

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
     "<string'creator|directory|verify'>", (: cmd($4[0] ) :) });
} /* query_patterns() */
