/*  -*- LPC -*-  */
/*
 * $Id: garbage.c,v 1.29 2003/05/14 17:29:31 ceres Exp $
 */

/** handler to remove junk lost in null-space from the mud so they wont waste
 * memory, and (if they have a heart_beat) cpu anymore. 
 */

nosave string *trouble = ({});
nosave mapping cloned_by = ([]);
nosave object *_rooms;
nosave int lag, lastlag, maxlag, minlag, burstlag, users;

#define DEBUG

// minimum time between reboots (3600 == 1 hour)
#define MIN_TIME 219599

// max time between reboots (133200 == 1 day 13 hours). The driver gets
// slower and slower if we stay up too long.
// 604800 is one week
//#define MAX_TIME 604800
// 219600 is 61 hours
#define MAX_TIME 219600

// maximum no. of faults per timer interval
#define FAULT_RATE 4500

// how long a room is idle before cleaning up
#define ROOM_IDLE_DELAY 7200
// if a room has been idle for this long dont clean it up again since
// its probably not going to go away.
#define ROOM_STABLE_DELAY 10800

#define NO_AUTO_REBOOT 1
//#define NO_TIMED_REBOOT 1

#define REBOOTS_PER_CRASH 6 
#define CHANGE_SPEED 6
#define SAVE_FILE "/save/garbage"

int max_time;
int crash;
int limit;

nosave int faults_prev;
nosave int stime_prev;
nosave int utime_prev;
nosave int uptime_prev;
nosave int rate_exceeded;
nosave int times_run;

//Filter function for the objects() efun we only want clones without references
//from other objects that are not shadowing stuff, not in a room and are not
//rooms themselves.
/**
 * @ignore yes
 */
#if efun_defined(get_garbage)
int get_junk(object ob){
  return ob->query_dest_other() == 0;
}
#else
int get_junk(object ob){
  if(!clonep(ob) || refs(ob) != 2 || query_shadowing(ob) ||
     environment(ob) || ob->query_dest_other())
    return 0;
  return 1;
}
#endif
//call_out id
nosave int id;

/**
 * @ignore yes
 */

void create(){
  function tmp;
  mapping map;

  unguarded(function(){
    restore_object(SAVE_FILE);
    if(!max_time) {
      max_time = MAX_TIME;
      crash = 1;
      limit = 1000;
      save_object(SAVE_FILE);
    } else {
      if(crash)
        max_time -= max_time/CHANGE_SPEED;
      else
        max_time += max_time/(CHANGE_SPEED*REBOOTS_PER_CRASH);
      if(max_time > MAX_TIME)
        max_time = MAX_TIME;
      crash = 1;
      save_object(SAVE_FILE);
    }
  });

  map = rusage();
  faults_prev = map["majflt"];
  stime_prev = map["stime"];
  utime_prev = map["utime"];
  uptime_prev = uptime();
  id = call_out("housekeeping", 300);
  tmp = function(function func) {
    int diff;
    
    diff = real_time() - time();
    
    if(diff > 2){
      diff -= 2;
      lag += diff;
      if(diff > maxlag) {
        maxlag = diff;
        if(maxlag > 600) { //10 minutes lag we probably stopped working
          //emergency shutdown
          master()->crash("Too much lag to recover");
          shutdown(0);
        }
      }
    }
    if(diff < minlag) {
      minlag = diff;
    }
    if(diff > 2 && (diff - lastlag > burstlag))
      burstlag = diff - lastlag;

    lastlag = diff;

    diff = sizeof(users());
    if(diff > users) {
      users = diff;
    }

    efun::call_out(func, 1, func);
  };
  call_out(tmp, 1, tmp);
}



/* A word of explanation from Ceres
   The system reboots based on the rate of major page faults ie. the rate of
   paging.  To handle updating of the drum it will not reboot if
   prev_faults is 0 and just in case, there is a minimum uptime as well.
   */
/**
 * @ignore yes
 */

void check_reboot() {
  int faults_now;
  int stime_now;
  int utime_now;
  mapping map;
  int reboot;
  
  map = rusage();
  stime_now = map["stime"];
  utime_now = map["utime"];
  faults_now = map["majflt"];

  event(users(), "inform",
        "Cpu usage for period = "+
        ((((utime_now-utime_prev) + (stime_now-stime_prev) ) / 10.0 )/
         (uptime() - uptime_prev))+"% Fault rate is "+
       (faults_now - faults_prev), "cpu");
  event(users(), "inform", "Machine lag is "+
        (lag/900) + " seconds (average) "+minlag+" seconds (min) "+
        maxlag+" seconds (max) "+burstlag+" seconds (burst)", "cpu");
  log_file("GARBAGE", "%s lag %d (%d/%d) burst %d users %d faults %d "
           "cpu %.1f%%\n",
           ctime(time())[4..18], (lag/900), minlag, maxlag, burstlag, 
           users, (faults_now - faults_prev),
           ((((utime_now-utime_prev) + (stime_now-stime_prev) ) /
             15.0 )/ (uptime() - uptime_prev)));
  
  // if we're already rebooting then don't do anything
  if(find_object("/obj/shut") && "/obj/shut"->query_time_to_crash() != 9999)
    return;

#ifndef NO_TIMED_REBOOT
  if(uptime() > max_time)
    reboot = 1;
#endif
    
#ifndef NO_AUTO_REBOOT
  if((uptime() > MIN_TIME) &&
     (faults_now > (faults_prev + FAULT_RATE))) {
    if(rate_exceeded) {
      reboot = 1;
    }
  } else {
    rate_exceeded = 1;
  } else {
    rate_exceeded = 0;
  }
#endif


  if(reboot) {
    shout("A'Tuin cries out: Too heavy, I need to put the Disc down for a "+
          "moment.\n");
    "/obj/shut"->shut(10);
    log_file("GARBAGE", ctime(time())[4..18]+" Auto; Faults: "+
             faults_now+", "+ faults_prev+"; uptime: "+uptime()+"; Cpu: "+
             ((((utime_now-utime_prev) + (stime_now-stime_prev) ) / 15.0 ) /
              (uptime() - uptime_prev))+
             "\n");
    /* Get a dump of the objects before we go down */
    //    dumpallobj();
    call_out(function(){
      crash = 0;
      unguarded((:save_object(SAVE_FILE):));
    }, 9*60);
    return;
  }

#if efun_defined(set_check_limit)
  if(lag > 900)
    limit += 5;
  if(lag < 300)
    limit--;
  limit = set_check_limit(limit);
#endif

  faults_prev = faults_now;
  uptime_prev = uptime();
  stime_prev = stime_now;
  utime_prev = utime_now;
  users = 0;
  lag = 0;
  maxlag = 0;
  burstlag = 0;
  minlag = 900;
} /* check_reboot() */

/** @ignore yes
 * This does a manual call of clean_up on rooms since the driver
 * doesn't seem to want to do it. :(
 * We do it every half hour after we've been up at least 1 hour.
 */
void start_cleanup() {
  int i;
  int t = real_time();
  
  // we're just doing rooms that haven't been visited for a while
  // (but not too long) and that dont want to keep loaded.
  _rooms = filter(objects((: function_exists("query_last_visited", $1) :)),
                  (: $1->query_keep_room_loaded() == 0 &&
                   $1->query_last_visited() < (time() - ROOM_IDLE_DELAY) &&
                   $1->query_last_visited() > (time() - ROOM_STABLE_DELAY):));
  
  for(i=(times_run % 2); i<sizeof(_rooms); i += 2)
    catch(_rooms[i]->clean_up());
  
  log_file("GARBAGE", "%s cleaning %d rooms. Current memory %.2fM\n",
           ctime(time())[4..18], i/2, (memory_info() / 1024000.0));
  event(users(), "inform",
        sprintf("Cleaning %d idle rooms", i/2), "cpu");

  if(real_time() - t > 1)
    log_file("GARBAGE", "%s room cleanup took %d seconds.\n",        
             ctime(time())[4..18], real_time() - t);
}

/**
 * @ignore yes
 * Do memory saving things.
 */
void tidy_up() {
  int rmem, ocount;

  // Perform reclamation of unreferenced objects.
  rmem = memory_info();
  ocount = reclaim_objects();
  rmem -= memory_info();
  
  event(users(), "inform",
        sprintf("Reclaimed %d object%s (%d byte%s)",
                ocount, (ocount == 1 ? "" : "s"),
                rmem, (rmem == 1 ? "" : "s") ), "cpu");
}

/**
 * @ignore yes
 */
void housekeeping() {
  string *tmpkeys;
  int i;
  object *junk;
  int t = real_time();

  //get the list of junk
  i = set_eval_limit(0);
  set_eval_limit(5000000);
#if efun_defined(get_garbage)
  junk = filter(get_garbage(), (:get_junk:));
#else
  junk = objects((:get_junk:));
#endif
  set_eval_limit(i);

  times_run++;
  
  call_out("check_reboot", 10);

  if(!(times_run % 3))          /* memory reclamation */
    call_out("tidy_up", 30);
  
  if(times_run > 4 && (times_run % 2 == 1)) /* room cleanup */
    call_out("start_cleanup", 60);

  //make sure it's in the list of trouble causing objects
  foreach(object ob in junk)
    trouble |= ({base_name(ob)});

  // Do this every 15 minutes.
  id = call_out((:housekeeping:), 15 * 60);

  // Log what caused the junk, and move the junk to the rubbish room
  // This needs alt_move because not all objects have a move function
  // alt_move is NOT for general usage.
  foreach(object ob in junk) {
#ifdef DEBUG
    if(cloned_by[file_name(ob)])
      log_file("GARBAGE_DEBUG", "%O cloned by %O\n", ob,
               cloned_by[file_name(ob)]);
#endif
    reset_eval_cost(); //sigh, need to fix the chatters!
    ob->dest_me();
  }
  
  //don't waste too much memory on that mapping
  tmpkeys = keys(cloned_by);
  if(sizeof(tmpkeys)>10000){
    cloned_by = ([]);
  }
#if !efun_defined(add_action)
  "/obj/handlers/livings"->remove_garbage();
#endif
  
  if(real_time() - t > 1)
    log_file("GARBAGE", "%s garbage cleanup took %d seconds.\n",        
             ctime(time())[4..18], real_time() - t);
}

/**
 * @ignore yes
 */

nosave private object armoury, cloner;

//called by clone_object
void cloned(object ob){
  object obcloner;
  if(!armoury)
    armoury = find_object("/obj/handlers/armoury");

  if(!cloner)
    cloner = find_object("/global/cloner");

  if(previous_object(1) != armoury && previous_object() != cloner){
    obcloner = previous_object(1);
  } else catch {
    int i = 2;
    obcloner = armoury;
    while(obcloner == armoury || obcloner == cloner)
      obcloner = previous_object(i++);
  };
    
  if(member_array(base_name(ob), trouble) != -1){
    //if objects of this type caused trouble before store how it got cloned
    //only store the object for now, we crash on this once per week otherwise
    string tmp = "";
#if 0
    int i, j;
    for(i = 2; i < sizeof(call_stack()); i++){
      if(!call_stack(1)[i])
        break;
      for(j = 0; j < 4; j++)
        tmp = sprintf("%s %O", tmp, call_stack(j)[i]);
      tmp += "\n";
    }
#endif
    cloned_by[file_name(ob)] = sprintf("%O\n%s", obcloner, tmp);
  }
  
  //make sure the call_out loop is still going
  if((find_call_out(id) < 0) && ((uptime() - uptime_prev) > (30 * 60)))
    id = call_out((:housekeeping:), 0);
}

/**
 * Return the number of seconds until the next reboot.
 */
int query_next_reboot() {
#ifndef NO_TIMED_REBOOT
  return max_time - uptime();
#else
  return -1;
#endif
}

int query_faults_prev() {
   return faults_prev;
} /* query_faults_prev() */

// change the max time, sometimes it's necessary.
void set_max_time(int i) { max_time = i; save_object(SAVE_FILE); }

void dest_me(){
  crash = 0;
  unguarded((:save_object(SAVE_FILE):));
  destruct(this_object());
}
