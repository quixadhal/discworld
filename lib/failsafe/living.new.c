inherit "/failsafe/container";

string msgout,
       msgin,
       mmsgout,
       mmsgin;
int verbose, coins;

void create() {
  container::create();
}

/* move stuff
 */
int test_add(object ob, int flag) { return !flag; }
int test_remove(object ob, int flag) { return !flag; }

/* thats it... 
 * no need for stupid checking to see if we are attacjking something in
 * here stuff.
 */
move_player(dir, dest, message) {
  string arr, leave;

  if (!msgout) {
    msgout = "leaves";
    msgin = "arrives";
    mmsgout = "dissapears in a puff of smoke";
    mmsgin = "appears out of the ground";
  }
/* we are teleporting */
  if (dir == "login") {
    arr = this_object()->query_cap_name()+" enters the game.\n";
  } else if (!dir || dir == "X") {
    arr = this_object()->query_cap_name()+" "+mmsgin+"\n";
    leave = this_object()->query_cap_name()+" "+msgout+"\n";
  } else {
    arr = this_object()->query_cap_name()+" "+msgin+"\n";
    leave = this_object()->query_cap_name()+" "+msgout+" "+dir+"\n";
  }
  move(dest, leave, arr);
  if (interactive(this_object()))
    if (verbose)
      write(dest->long());
    else
      write(dest->short()+".\n");
}
