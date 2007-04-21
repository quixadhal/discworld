#include <am_time.h>
#include <situations.h>

#define RANDOM(a,b) ("/obj/handlers/random_num.c"->random(a,b))
#define MIN_DURATION  1
/* seconds between checks for situation changes */
#define SECTOCHECK    15
/* maximum random install delay (seconds) */
#define INSTALLDELAY  15
#define MAXROOMS      30

/* Room info
 * (roominfo[i])[0]=currentsits [1]=offset [2]=els [3]=sittiming 
 *              [4]=chatmin [5]=chatmax [6]=awake
 * int *currentsits;
 * string *els;
 * int offset;
 * mixed *sittiming;
 * int chatmin, chatmax;
 * int awake;  0 off 1 on 
 */
nosave mapping roominfo;
nosave mixed *rooms;

/* global situation info */
nosave mapping sitdata;

/**
 * situation changer object associated with multiple room objects.
 * It keeps a list of rooms and checks them all in turn. It 
 * behaves similarly to the room_handler.  It's main purpose is
 * to avoid duplicating a large chat database for many rooms
 * that all use it. 
 */

/** 
 * returns status of situation manager.  
 * If it is sleeping it will turn on again if a
 * player enters the room.
 * @return status 0 off 1 on 2 sleeping
 */
int query_status() { return 1; }

/** 
 * returns situations currently turned on.
 * @return int array of situation labels
 */
int *query_current_situations(object room) 
{ 
  if (!roominfo[file_name(room)]) return 0;
  return (roominfo[file_name(room)])[0]; 
}

void dest_me() {
  destruct(this_object());
} /* dest_me() */

/**
 * Adds a situation for the rooms.  These situations can be
 * invoked manually with start_situation or automatically via
 * automate_situations.
 * @param num number labelling the situation
 * @param func function to be called at start of situation 
 *             that might be used to load NPC's.  If it is
 *             a set of two function pointers ({ f1, f2 })
 *             the second function is called when the
 *             situation is ended.  The start function is 
 *             passed the num label of the room and a
 *             do_start_mess flag.  If the flag is one
 *             the situation is starting rather than
 *             being reloaded.  The end function is only
 *             passed the num label.
 * @param startmess message told to the room at start of situation
 * @param endmess message told to the room at end of situation
 * @param extralooksting extra look string appended to rooms long 
 * description during the situation
 * @param chats an array of chat strings to be active 
 * during the situation 
 * @param add_items a mixed array of ({ item, item description }) 
 * pairs to be active during the situation
 * @see start_situation
 * @see automate_situations
 * @see add_item
 * @see room_chat
 * @see add_extra_look
 * @example
 *  add_situation( 0, 0, "Water seeps out of the ground to form puddles.", 
 *     "",  "There are large puddles on the ground here.",
 *     ({"A hidden frog croaks quietly.",
 *       "There is a blooping sound." }),
 *     ({ ({"puddle", "The puddles are dark and murky.  " 
 *                    "They will probably dry up given time." }) }) );
 */
void add_situation(int num, function *func, string startmess, string endmess, 
                   string extralookstring, string *chats, mixed *add_items ) {
  if (!sizeof(sitdata))
    sitdata= ([ num : 
              ({ startmess, endmess, extralookstring, chats, add_items, func }) ]);
  else
    sitdata+= ([ num : 
               ({ startmess, endmess, extralookstring, chats, add_items, func }) ]);
}

/**
 * Starts a situation previously added for the room
 * that is managed by this object.
 * These situations can be invoked manually with 
 * start_situation or automatically via
 * automate_situations. 
 * @param num number labelling the situation
 * @param do_start_mess 0 to supress the start_mess string
 *        This is to fake it that a situation has been 
 *        going for a while when really you just loaded it.
 * @param room room to add situation to
 * @see add_situation
 * @see end_situation
 * @see automate_situations
 */
void start_situation(int num, int do_start_mess, object room) {
  mixed *sdata;
  string *chats;
  mixed item,*items;
  int *currentsits;
  string *els;
  object chatter;

  if (!(sdata=sitdata[num]) || !room) return;
  if (!roominfo[file_name(room)]) roominfo[file_name(room)]=
                 ({ 0,0,0,0,60,120,1 });
  currentsits=(roominfo[file_name(room)])[0];
  els=(roominfo[file_name(room)])[1];
  if (!currentsits) currentsits=({ num });
  else currentsits=currentsits+({ num });

  if (sizeof(sdata[2])) {
    if (els && sizeof(els)) els+=({ sdata[2] });
    else { 
      els=({ sdata[2] });   
      room->add_extra_look(this_object()); 
    }
  }
  if (do_start_mess && sizeof(sdata[0])) 
    tell_room(room, sdata[0]+"\n");
  chats=sdata[3];
  if (chats && sizeof(chats)) {
    //    tell_creator("shaggy","Adding: %O\n",chats);
    if (!(chatter=room->query_chatter())) {
      room->room_chat( ({ (roominfo[file_name(room)])[4],(roominfo[file_name(room)])[5], chats }) );
      chatter=room->query_chatter();
      chatter->check_chat();
    }
    else chatter->add_room_chats(chats);
  }
  items=sdata[4];
  if (sizeof(items)) {
    foreach(item in items) room->add_item(item[0],item[1]);
  }
  if (sdata[5]) { /* functionp */
    if (functionp(sdata[5])) (*(sdata[5]))(num,do_start_mess);
    else if (arrayp(sdata[5]) && functionp((sdata[5])[0])) 
      (*((sdata[5])[0]))(num,do_start_mess);
  }
  (roominfo[file_name(room)])[0]=currentsits;
  (roominfo[file_name(room)])[1]=els;
} /* start_situation() */

/**
 * Ends a situation previously added and started on the room
 * that is managed by this object.
 * These situations can be invoked manually with start_situation 
 * or automatically via automate_situations. 
 * @param num number labelling the situation
 * @param room room to end situation on
 * @see add_situation
 * @see start_situation
 * @see automate_situations
 */
void end_situation(int num, object room) {
  mixed *sdata;
  string *chats;
  mixed item,*items;
  int *currentsits;
  string *els;
  object chatter;
  
  if (!(sdata=sitdata[num]) || !room) return;
  if (!roominfo[file_name(room)]) return;
  currentsits=(roominfo[file_name(room)])[0];
  els=(roominfo[file_name(room)])[1];
  if (currentsits) currentsits=currentsits-({ num });
  if (els && sizeof(sdata[2])) { 
    els-=({ sdata[2] });
    if (!sizeof(els)) room->remove_extra_look(this_object());
  }

  chats=sdata[3];
  if (chats && sizeof(chats) && (chatter=room->query_chatter())) {
    //    tell_creator("shaggy","Removing: %O\n",chats);
    chatter->remove_room_chats(chats);
  }
  items=sdata[4];
  if (sizeof(items)) {
    foreach(item in items) room->remove_item(item[0],item[1]);
  }
  if (sizeof(sdata[1])) 
    tell_room(room,sdata[1]+"\n");
  if (sdata[5]) { /* functionp */
    if (arrayp(sdata[5]) && functionp((sdata[5])[1])) 
      (*((sdata[5])[1]))(num);
  }
  (roominfo[file_name(room)])[0]=currentsits;
  (roominfo[file_name(room)])[1]=els;
} /* end_situation() */

/**
 * Starts and ends situations according to the information
 * in the sittiming mapping.  It is called continuously 
 * automatically while there are interactives in the room.
 */
void manage_situations(object room) {
  mixed *sit;
  int num,t,it,cnt,possible;
  int *currentsits,*newsits,*changes;
  int ttmp,tstep,offset,awake;
  mixed data;

//  tell_creator("shaggy","Managing %O %O %O \n",room,roominfo[file_name(room)],
//    filter_array( all_inventory( room ),(: interactive($1) :)));
  if (!room || !(data=roominfo[file_name(room)])) return;
  if (!sizeof(filter_array( all_inventory( room ),(: interactive($1) :)))) {
    (roominfo[file_name(room)])[6]=0;  
    return;
  }
//  tell_creator("shaggy","No abort \n");

  currentsits=data[0];
  offset=data[2];
  awake=data[6];
  t=time()+offset;
  newsits=({});
  tstep=2147483647;
  
  cnt=0;
  foreach(sit in data[3]) {
    cnt++;
    ttmp=sit[1];
    it=t/ttmp;
    possible=0;
    if (sit[2]) {
      if (functionp(sit[2])) possible=(*(sit[2]))();
      else possible=sit[2];
      possible=
	(possible&(1<<((AM_TIME_HANDLER)->query_am_hour(it*ttmp-offset))))
	&&
	(RANDOM(1000,it*1147483647+cnt*2047483243) < sit[3]);
    }
    if (possible) {
      if (arrayp(sit[0])) {
	num=(sit[0]) [((t % ttmp)*sizeof(sit[0]))/ttmp];
	ttmp=ttmp/sizeof(sit[0]);
      }
      else num=sit[0];
      if (member_array(num,newsits)==-1) newsits=newsits+({ num });
    }
    if (ttmp<tstep) tstep=ttmp;
  }
  if (currentsits && sizeof(currentsits)) {
    changes=currentsits-newsits;
    if (sizeof(changes)) foreach(num in changes) {
      if (awake) call_out("end_situation",random(INSTALLDELAY),
        num,room);
      else end_situation(num,room);
    }
    changes=newsits-currentsits;
    }
  else changes=newsits;
  if (sizeof(changes)) foreach(num in changes) {
    if (awake) call_out("start_situation",random(INSTALLDELAY),
       num,awake,room);
    else start_situation(num,awake,room);
  }
  if ((tstep/=SECTOCHECK)>=MAXROOMS) tstep=MAXROOMS-1; 
  if (!rooms[tstep]) rooms[tstep]=({ room });
  else rooms[tstep]=rooms[tstep]+({ room });

  (roominfo[file_name(room)])[6]=1;  
} /* manage_situations() */

/**
 * Starts and ends situations according to the information
 * in the sittiming mapping for each room in the rooms array.  
 * It is called continuously 
 * automatically while there are interactives in the room.
 */
void manage_rooms_situations() {
  object room,*current_rooms;

  current_rooms = rooms[ 0 ];
  rooms[ 0 .. <2 ] = rooms[ 1 .. <1 ];
  rooms[ <1 ] = 0;
  call_out("manage_rooms_situations",SECTOCHECK);
  if ( !pointerp( current_rooms ) ) return;
  foreach ( room in current_rooms ) manage_situations(room);
} /* manage_room_situations() */

/**
 * Tests for enabling situation managing.
 * If situation managing is already active or turned off
 * it does nothing.
 * @param room the room
 * @see automate_situations
 */
void check_situations(object room) {
  if (room && roominfo[file_name(room)] 
    && !((roominfo[file_name(room)])[6])
    && interactive(this_player())) manage_situations(room); 
}
/**
 * Awakes starting and ending of situations.
 * These situations can be invoked manually with start_situation.
 * The awaked starting and ending is unaffected by the room 
 * unloading.  When the room reloads the situation will be 
 * restarted unless its duration is up.
 * @param chatargs same as arguments to room chat:
 *    ({ min, max, ({ chat1, chat2, ... }) })
 *    if you want no non-situational chats just put ({ min, max, ({ }) })
 *    min, max are always needed being the minimum/maximum time
 *    between all chats.  chat1 is a string containing a chat.
 * @param xval First random seed number (eg. x coordinate)
 * @param yval Second random seed number (eg. y coordinate)
 * @param sittiming A mixed array containing the information 
 *    about when the situations added by add_situation are 
 *    automatically started and ended:
 *
 *    ({ info1, info2, info3, ... })
 *
 *  Each info is a mixed array as follows:
 *
 *    ({ number, duration, when, chance })
 *               -- or --
 *    ({ ({ num0, num1, num2, ..., numN }) , when, duration, percent })
 *
 * number     (int) label of the situation to start 
 *            up if number is an array then a set of
 *            situations are started one at a time.
 *            The total duration is divided evenly 
 *            between the situations labelled by the 
 *            numbers in the array which are started 
 *            and ended in the order given.  The 
 *            entire set always gets used.
 *
 * duration   (int) time (minutes) situation should last for
 *
 * when       (int) a nighttime/daytime mask
 *            This determines when during the day in
 *            Ankh-Morpork time the situation may occur.  
 *            The masks are defined in evolvingtime.h
 *            It can be a function pointer in which case it is 
 *            expected to return a mask value.
 *
 * chance     (int) 1/1000 chance per period of duration minutes 
 *            of getting into the situation
 * @see add_situation
 * @see start_situation
 * @see end_situation
 * @see evolvingroom.h
 * @example
 *  automate_situations( room, ({ 50,70, ({""}) }), 
 *     170,  220,
 *     ({ ({           0, 5, WHEN_ANY_TIME, 200 }),
 *        ({ ({ 1,2,3 }), 6, (WHEN_NIGHT|WHEN_EVENING), 500 }) }) );
 */
void automate_situations(mixed *chatargs, int xval, int yval, mixed *edata,
  object room) {
  mixed sit;
  mixed *sittiming;
  int offset;

  offset=xval*38547+yval*1232444311;

  if ( pointerp(chatargs[2]) && sizeof(chatargs[2])) {
    room->room_chat( chatargs );
  }

  if (sizeof(edata)) {
    foreach(sit in edata) {
      if (sit[1]<MIN_DURATION) {
        write("Invalid situation duration: "+sit[1]+"\n");
        return;
      }
      sit[1]*=60; /* minutes to seconds */
      if (!sittiming) sittiming=({ sit });
      else sittiming=sittiming+({ sit });
      /* may add extra info to sittiming later */
    }
  }
  roominfo[file_name(room)]=({  0,0, offset, sittiming, 
       chatargs[0], chatargs[1], 0 });
  manage_situations(room);
} /* automate_situations() */

/**
 * Shuts down all current and pending situations.  It also turns off the
 * awaked situation manager so no more are added.  It does not
 * destruct this object so all the add_situations are still loaded
 * and make be recommenced with automate_situations.  dest_me is
 * the appropriate call to permanently remove all situations.
 * @param room room to shutdown situations on
 * @see automate_situations
 */
void shutdown_situations(object room) {
  int num, *currentsits;

  currentsits=(roominfo[file_name(room)])[0];
  if (currentsits && sizeof(currentsits)) {
    foreach(num in currentsits) end_situation(num, room);
  }
} /* shutdown_situations() */

string extra_look(object room) {
  string *els;

  if (!roominfo[file_name(room)]) return "";
  els=(roominfo[file_name(room)])[1];
  if (!els || !sizeof(els)) {
    return "";
  }
  return implode(els,"  ")+"\n";
} /* add_extra_look() */

void create() {
   rooms = allocate( MAXROOMS );
   manage_rooms_situations();
   roominfo= ([ ]);
} /* create() */

object *query_rooms() { return rooms; }









