#include <am_time.h>
#include <situations.h>

#define RANDOM(a,b) ("/obj/handlers/random_num.c"->random(a,b))
/**
 * situation changer object associated with a room object.
 */

class situation_timing { 
  mixed label;
  mixed duration;
  mixed when;
  int chance;
  int *endat;
  mixed background;
  mixed category;
  int it;
  mapping it_data;  
}

nosave int offset;
nosave string *els;
nosave mixed *currentsits,*autosits;
nosave mapping sitdata;
nosave mapping sitwords;
nosave class situation_timing *sittiming;
/* By default chat fairly frequently (1-2 minutes) because
 * these situations will not necessarily be around that long */
nosave int chatmin=60,chatmax=120;
nosave object ownerroom;
nosave int automate=0; /* 0=no, 1=yes, 2=sleeping */
nosave mixed cco; /* cco[0] = last handle, cco[i] = call_outs for handle i>0 */

varargs mixed change_situation( mixed label, mixed duration,  
  mixed words, mixed handle );

object set_room(object room_o) { 
  //  tell_creator("shaggy","Room set: %O %O %O\n",room_o,this_object(),previous_object()); 
  ownerroom=room_o;
  return this_object();
}

object query_room() { return ownerroom; }
/** 
 * This function selects word replacements for #n in the text.
 * @param label Situation label to choose for
 * @param choice A random seed (integer) or
 *               a set of pairs exactly the same as the second
 *               argument to replace.
 * @see replace
 * @example
 *      choose_words( "frog", ({ "#1", "tadpole", "#2", "pond" }));
 */
void choose_words( mixed label, mixed choice )
{
  int i;
  class situation sit;
  string *wc;

  sit = sitdata[ label ];
  if (sizeof(sit->random_words)) {
    if (!sitwords) sitwords= ([ ]);

    if (intp(choice)) {
      wc=({ });
      for (i=0;i<sizeof(sit->random_words);i++) {
        wc+=({ "#"+(i+1),
          ((sit->random_words)[i])[
          RANDOM(sizeof((sit->random_words)[i]),choice+i*3347483647)] });
      }
      sitwords[label] = wc;
    }
    else 
      sitwords[label] = choice;
  }
}

/** 
 * Replaces #1 in text with the one of the first array of words in wordlist
 * and #2 with one of the second array of words and so on...
 * Each string in the string array is changed.
 */
string *insert_words_chats(class situation sit, string *words) {
  string *outarray=({ });
  string s1;

  if (!sizeof(sit->chats)) return ({ });
  foreach (s1 in sit->chats) outarray+= ({ replace(s1,words) });
  return outarray;
}

/** 
 * Replaces #1 in text with the one of the first array of words in wordlist
 * and #2 with one of the second array of words and so on...
 * For items only the item text is changed, not the key words.
 */
mixed *insert_words_items(class situation sit, string *words) {
  mixed *outarray=({ });
  mixed ai,e0,e1;

  if (!sizeof(sit->add_items)) return ({ });
  foreach (ai in sit->add_items) {
    if (sizeof(ai)>=2) {
      e0=ai[0];
      if (arrayp(e0)) e0=explode(lower_case(replace(implode(e0,"|"),words)),"|");
      else e0=lower_case(replace(e0,words));
      e1=ai[1];
      if (arrayp(e1)) e1=explode(replace(implode(e1,"|"),words),"|");
      else e1=replace(e1,words);
      ai = ({ e0,e1 })+ai[2..];
    }
    outarray+= ({ ai });
  }
  return outarray;
}

/** 
 * returns status of situation manager.  
 * If it is sleeping it will turn on again if a
 * player enters the room.
 * @return status 0 off 1 on 2 sleeping
 */
int query_status() { return automate; }

/** 
 * returns situations currently turned on.
 * @return int array of situation labels
 */
int *query_current_situations() { return ({ currentsits, autosits }); }

/** 
 * returns sittiming class with info about automated situations 
 * @return class sittiming
 */
mixed *query_sittiming() { return sittiming; }

/** 
 * returns mapping of situations. 
 * @return mapping of situations
 */
mapping query_situations() { return sitdata; }

void dest_me() {
  destruct(this_object());
} /* dest_me() */

/** @ignore yes */
int clean_up( int parent ) {
/* Clones of this object are pointless without a room to manage. */
  if (parent) {
    return 0;
  }
  return 1;
} /* clean_up() */

/**
 * Adds a situation to the room.  These situations can be
 * invoked manually with start_situation or automatically via
 * automate_situation.
 * @param label string or number labelling the situation
 * @param sit a structure (class) containing all the bits
 * of the situation you want to add.
 * eg. 
 * start_func function to be called at start of situation 
 *             that might be used to load NPC's or anything
 *             beyond a message.
 * 
 *             The start function is passed the label, 
 *             a do_start_mess flag and the room object.  
 *             If the flag is 1 the situation is starting 
 *             rather than being reloaded.  Thus if 
 *             do_start_mess is 0 then you should avoid
 *             any obvious start messages and make it look
 *             like the situation is already underway.
 *
 * end_func function to be called an the end of a situation.  
 *             The end function is only
 *             passed the label and the room object.
 *
 * start_mess message told to the room at start of situation
 *
 * end_mess message told to the room at end of situation
 *
 * extra_look extra look string appended to rooms long 
 *             during the situation
 *
 * chats an array of chat strings to be active 
 *             during the situation 
 *
 * add_items a mixed array of ({ item, item description }) 
 *             pairs to be active during the situation
 *
 * random_words sets of words of the form ({ ({ "option1","option2" }),
 *             ({ "adjective1","adjective2" }), ... }).  One of the
 *             the first set replaces #1 in any text above and one of
 *             the second set replaces #2 in any text above and so on.
 *             The random choice is fixed for the duration of any one
 *             situation.
 *
 * @see start_situation
 * @see automate_situation
 * @see add_item
 * @see room_chat
 * @see add_extra_look
 * @example
 * #include <situations.h>
 *
 * class situation frogs;
 * frogs = new(class situation, 
 *     start_mess: "Water seeps out of the ground to form puddles.",
 *     extra_look: "There are large puddles on the ground here.",
 *     chats: ({"A hidden frog croaks quietly.",
 *              "There is a blooping sound." }),
 *     add_items:({ ({"puddle", "The puddles are dark and murky.  " 
 *                    "They will probably dry up given time." }) }) );
 * add_situation( "frogs", frogs );
 * @example
 * add_situation( "frogs", new(class situation, 
 *      start_mess: "Water seeps out of the ground to form puddles.",
 *      extra_look: "There are large puddles on the ground here.",
 *      chats: ({"A hidden frog croaks quietly.",
 *               "There is a blooping sound." }),
 *      add_items: ({ ({"puddle", "The puddles are dark and murky.  " 
 *                      "They will probably dry up given time." }) }) ));
 */
void add_situation( mixed label, class situation sit ) {
  if (!sizeof(sitdata)) {
    sitdata= ([ label : sit ]);
    sitwords= ([ ]);
  }
  else {
    sitdata+= ([ label : sit ]);
  }
}

/**
 * Starts a situation previously added to the room
 * that is managed by this object.
 * These situations can be invoked manually with 
 * start_situation or automatically via
 * automate_situation. 
 * @param label string or number labelling the situation
 * @param do_start_mess 0 to supress the start_mess string
 *        This is to fake it that a situation has been 
 *        going for a while when really you just loaded it.
 * @see add_situation
 * @see end_situation
 * @see automate_situation
 */
void start_situation(mixed label, int do_start_mess) {
  class situation sdata;
  string *chats;
  mixed item,*items;
  object chatter;
  string *words;
  
  sdata=sitdata[ label ];
  words=sitwords[ label ];
  //  tell_creator("shaggy","label starting: %O\n",label);
  
  if (!currentsits) currentsits=({ label });
  else currentsits=currentsits+({ label });

  //  tell_creator("shaggy","Start seed: %O\n",seed);

  if (sizeof(sdata->extra_look)) {
    if (els && sizeof(els)) 
      els+=({ replace(sdata->extra_look,words) });
    else { 
      els=({ replace(sdata->extra_look,words) });   
      ownerroom->add_extra_look(this_object()); 
    }
  }
  if (do_start_mess && sizeof(sdata->start_mess)) 
    tell_room(ownerroom, replace(sdata->start_mess,words)+"\n");
  chats=insert_words_chats(sdata,words);
  if (chats && sizeof(chats)) {
    //    tell_creator("shaggy","Adding: %O %O %O\n",sdata->chat_rate,chats,ownerroom->query_chatter());
/* Note we use the old chatter because it is more efficient unless
   we know for certain that we need chats with different rates 
   in which case we use the multichatter 
*/
    if (!(chatter=ownerroom->query_chatter())) {
      if (sdata->chat_rate) {
        ownerroom->room_chat( ({ chatmin,chatmax, ({ }) }),
                      clone_object("/std/room/basic/multichatter")  );  
        chatter=ownerroom->query_chatter();
        chatter->setup_chatter( ownerroom,
        ({ (sdata->chat_rate)[0],(sdata->chat_rate)[1], chats }) );
      } else {
        ownerroom->room_chat( ({ chatmin,chatmax, chats }) );
        chatter=ownerroom->query_chatter();
      }
      chatter->check_chat();
    } else {
      if (sdata->chat_rate) {
        if (!(chatter->query_multichatter())) {
          mixed *args;
          args=(chatter->query_room_chats())[0..2];
          chatter->dest_me();
          ownerroom->room_chat( args,
                        clone_object("/std/room/basic/multichatter")  );  
          chatter = ownerroom->query_chatter();
        }
        chatter->setup_chatter( ownerroom,
          ({ (sdata->chat_rate)[0],(sdata->chat_rate)[1], chats }) );
        chatter->check_chat();
      }
      else chatter->add_room_chats(chats); 
    } 
  }
  items=insert_words_items(sdata,words);
  if (sizeof(items)) {
    foreach(item in items) { 
      ownerroom->add_item(item[0],item[1]);
      //      tell_creator("shaggy","Adding item: %O:%O\n",item[0],item[1]);
    }
  }
  if (sdata->start_func) { /* functionp */
    (*(sdata->start_func))(label,do_start_mess,ownerroom);
  }
} /* start_situation() */

/**
 * Ends a situation previously added and started on the room
 * that is managed by this object.
 * These situations can be invoked manually with start_situation 
 * or automatically via automate_situation. 
 * @param label string or number labelling the situation
 * @see add_situation
 * @see start_situation
 * @see automate_situation
 */
void end_situation(mixed label) {
  class situation sdata;
  string *chats;
  mixed item,*items;
  object chatter;
  string *words;

  sdata=sitdata[ label ];
  words=sitwords[ label ];
//  tell_creator("shaggy","label ending: %O\n",label);

  if (currentsits) currentsits=currentsits-({ label });
  if (els && sizeof(sdata->extra_look)) { 
    els-=({ replace(sdata->extra_look,words) });
    if (!sizeof(els)) ownerroom->remove_extra_look(this_object());
  }
  
  chats=insert_words_chats(sdata,words);
  if (chats && sizeof(chats) && (chatter=ownerroom->query_chatter())) {
    //tell_creator("shaggy","Removing: %O\n",chats);
    chatter->remove_room_chats(chats);
  }
  items=insert_words_items(sdata,words);
  if (sizeof(items)) {
    foreach(item in items) {
      if (arrayp(item[0])) 
        ownerroom->remove_item((item[0])[0]);
      else 
        ownerroom->remove_item(item[0]);
      //        tell_creator("shaggy","Removing item: %O:%O\n",item[0],item[1]);
    }
  }
  if (sizeof(sdata->end_mess)) 
    tell_room(ownerroom,replace(sdata->end_mess,words)+"\n");
  if (sdata->end_func) { /* functionp */
    (*(sdata->end_func))(label,ownerroom);
  }
} /* end_situation() */
  
/**
 * Starts one or more situations that will end after a
 * specified duration.  You can use an array and make
 * further situations commence when others end.
 * @param label (mixed) label of the situation to start 
 *  up.  If you pass an array such as ({ "frog1", "frog2" }) for the 
 * label then that set of situations are started one at
 * a time and the total duration is split evenly between them.
 * Label is usually an integer or a string or an array of
 * integers and/or strings.
 * If the string is a list of labels
 * separated by , then multiple situations
 * are started using those labels.
 * @param duration (int) total time (seconds) the overall situation 
 * should last.  You can put an array of durations -- one for each
 * situation if the label lists more than one situation and then
 * the overall time is the sum of the numbers.
 * -1 means indefinite so having any situations after
 * something with -1 duration is pointless.
 * @param handle is an internal thing that should only be called with 0
 * unless you really know what you are doing.
 * @param words is a list of replacements for #n in the text OR
 *        a random number seed, it is
 *        passed to choose_words.
 *        eg. ({ "#1", "frog", "#2", "honey" }) or 22
 * @return call_out that is propogating the changes
 * This is useful if you want to be able to kill the whole
 * set without disturbing other situations.
 * @see start_situation
 * @see end_situation
 * @see add_situation
 * @see choose_words
 */
varargs mixed change_situation( mixed label, mixed duration, 
     mixed words, mixed handle  ) {
  mixed frog,bing;
  mixed ending;

//  tell_creator("shaggy","CHANGE: %O %O\n",label,duration);

  if (!cco) cco=({ 1, 0 });
  if (!handle) {
    handle=member_array(0,cco[1..<1])+1;
    if (!handle) { 
      cco+=({ 0 });
      handle=sizeof(cco)-1;
    }
  }
  cco[0]=handle;

  if (!duration) {
    if (arrayp(label)) frog=label[0];
    else frog=label;
    if (stringp(frog)) frog=explode(frog,",");
    else frog=({ frog });
    foreach(bing in frog) end_situation(bing);
    cco[handle]=0;
    return 0;
  }
  if (!arrayp(duration)) {
    if (arrayp(label)) {
      bing=duration/sizeof(label);
      duration=({ });
      foreach (frog in label) duration+=({ bing });
    }
    else duration=({ duration });  
  }  

  if (arrayp(label) && sizeof(label)>1 && sizeof(label)>sizeof(duration)) {
    frog=label[0];
    if (stringp(frog)) ending=explode(frog,",");
    else ending=({ frog });
    label=label[1..<1];
  } 

  if (sizeof(duration)==1) {
    if (arrayp(label)) frog=label[0];
    else frog=label;
    if (stringp(frog)) frog=explode(frog,",");
    else frog=({ frog });
    if (ending) {  
      ending-=frog;
      foreach(bing in ending) end_situation(bing);
    }
    if (currentsits) frog=frog-currentsits;
    foreach(bing in frog) {
      if (!sizeof(words)) choose_words( bing, time()*335423611 );
      else choose_words( bing, words );
      start_situation(bing,1);
    }
    if (duration[0]!=-1) 
      cco[handle]=call_out("change_situation",duration[0],
                           label,0,words,handle);

    return handle;
  }
  if (arrayp(label)) frog=label[0];
  else frog=label;
  if (stringp(frog)) frog=explode(frog,",");
  else frog=({ frog });
  if (ending) {  
    ending-=frog;
    foreach(bing in ending) end_situation(bing);
  }
  if (currentsits) frog=frog-currentsits;
  foreach(bing in frog) start_situation(bing,1);
  if (duration[0]!=-1) 
    cco[handle]=
    call_out("change_situation",duration[0],
             label,duration[1..<1],words,handle);

  return handle;
} /* change_situation */

int query_possible(class situation_timing sit, int it, int tod, int cnt) 
{
  int possible;
  class situation_timing sit2;
  int cnt2,it2,tod2,possible2;

  if (functionp(sit->when)) possible=(*(sit->when))(tod);
  else possible=sit->when;

  possible=
    (possible&(1<<((AM_TIME_HANDLER)->query_am_hour(tod))))
    &&
    (RANDOM(1000,it*1147483647+cnt*2047483243) < sit->chance);
  //  tell_creator("shaggy","trying %O (%O %O %O %O %O %O)\n",sit->label,possible,sit->when,(1<<((AM_TIME_HANDLER)->query_am_hour(tod))),RANDOM(1000,it*1147483647+cnt*2047483243),tod,it);
  //    tell_creator("shaggy","trying %O %O %O %O %O %O\n",time(),t,offset,ttmp,tod,it);
  if (possible && sit->category) {
    /* If it is a member of a category we must check that there
       are no others of the category going first */
    cnt2=0;
    foreach (sit2 in sittiming) {
      cnt2++;
      if (cnt2!=cnt && sit2->category == sit->category) {
	it2=(tod+offset+(cnt2*234231))/sit2->duration;
	tod2=it2*sit2->duration-offset-(cnt2*234231);
	if (functionp(sit2->when)) possible2=(*(sit2->when))(tod2);
	else possible2=sit2->when;
	possible2=
	  (possible2&(1<<((AM_TIME_HANDLER)->query_am_hour(tod2))))
	  &&
	  (RANDOM(1000,it2*1147483647+cnt2*2047483243) < sit2->chance);
	//	tell_creator("shaggy","Comparing %O (%O %O %O %O %O %O)\n",sit2->label,possible2,sit2->when,(1<<((AM_TIME_HANDLER)->query_am_hour(tod2))),RANDOM(1000,it2*1147483647+cnt2*2047483243),tod2,it2);
	if (possible2 && (tod2<tod || (tod==tod2 && cnt2<cnt))) {
	  possible=0;
	  break;
	}
      }
    }
  }

  return possible;

} /* query_possible() */

/**
 * Starts and ends situations according to the information
 * in the sittiming array.  It is called continuously 
 * automatically while there are interactives in the room.
 */
void manage_situations() {
  class situation_timing sit;
  int t,t0,it,tod,cnt,possible,dt,tc,i;
  mixed label,lb;
  mixed *newsits;
  mixed *changes;
  mixed *background_on,*background_off; /* background situations */
  int ttmp,tstep;

//  tell_creator("shaggy","Managing: %s %O\n",ownerroom->query_short(),sittiming);

  if (!automate) return;
  if (!sizeof(filter_array( all_inventory( ownerroom ),(: interactive($1) :)))) {
    automate=2;
    return;
  }
  //  tell_creator("shaggy","Time %O\n",time());
  t0=time()+offset;
  newsits=({});
  tstep=2147483648;
  background_on=({ });
  background_off=({ });

  cnt=0;
  foreach (sit in sittiming) {
    cnt++;
    t=t0+(cnt*234231);
    ttmp=sit->duration;
    it=t/ttmp;
    /* time of day it started */
    tod=it*ttmp-offset-(cnt*234231);

    /* possible values cached in it_data mapping */
    possible=sit->it_data[ it ];

    /* If it's not in cache -- work it out */
    if (undefinedp(possible)) {
      possible=query_possible(sit, it, tod, cnt);
      sit->it_data[ it ]=possible;
      if (!undefinedp(sit->it_data[ it-1 ])) 
	map_delete(sit->it_data,it-1);
    } else {
      //      tell_creator("shaggy","Test for %O cached %O\n",sit->label,possible);
    }

    if (possible) {
      if (sit->background) background_off+=explode(sit->background,",");
      if (arrayp(sit->label)) {
	//      label=(sit->label) [((t % ttmp)*sizeof(sit->label))/tmp];
	dt=(t % ttmp); 
	i=0;
	foreach (tc in sit->endat) {
	  if (tc>dt) break;
	  i++; 
	} 
	label=(sit->label)[i];
	ttmp=tc-dt;
      }
      else label=sit->label;
      
      if (stringp(label)) {
	label=explode(label,",");
	newsits=newsits+label;
	foreach(lb in label) choose_words(lb,it*1147483647+cnt*2047555777);
      }
      else { 
	newsits=newsits+({ label });
	choose_words(label,it*1147483647+cnt*2047555777);
      }
    }
    else {
      if (sit->background) background_on+=explode(sit->background,",");
      /* pretest next possible time to avoid retesting every situation,
	 every time one of them might start */
      /* possible values cached in it_data mapping */
      possible=sit->it_data[ it+1 ];
      /* If it's not in cache -- work it out */
      if (undefinedp(possible)) {
	possible=query_possible(sit, it+1, tod+sit->duration, cnt);
	sit->it_data[ it+1 ]=possible;
	if (!undefinedp(sit->it_data[ it-1 ]))
	  map_delete(sit->it_data,it-1);
      }
      if (possible) 
	ttmp=ttmp-(t % ttmp); 
      else 
	ttmp=2*ttmp-(t % ttmp);
    }

    if (ttmp<tstep) tstep=ttmp;
  }
   
  //  tell_creator("shaggy","newsits: %O tstep: %O t: %O/%O \n",newsits,tstep,dt,tc);

  call_out("manage_situations",tstep+1);

  newsits = newsits+(background_on-background_off);

  if (autosits && sizeof(autosits)) {
    changes=autosits-newsits;
    if (sizeof(changes)) { 
      foreach(label in changes) end_situation(label);
      autosits=autosits-changes;
    }
    changes=newsits-autosits;
  }
  else changes=newsits;
  if (sizeof(changes)) {
    foreach(label in changes) start_situation(label,2-automate);
    if (autosits) autosits=autosits+changes; 
    else autosits=changes;
  }

  automate=1;
} /* manage_situations() */

/**
 * Tests for enabling situation managing.
 * If situation managing is already active or turned off
 * it does nothing.
 * @see automate_situation
 */
void check_situations() {
  if (automate==2 && interactive(this_player())) manage_situations();
}

/**
 * Makes a seed value for the random part of when 
 * situations turn on and off.  The two ints must be 
 * constant for a given room -- like the coordinates.
 * @param xval integer to use to make a seed (eg. x coordinate)
 * @param yval integer to use to make a seed (eg. y coordinate)
 */
void make_seed(int xval, int yval) {
  /* The >>2 makes time() + offset always positive */
  offset=(xval*38547+yval*1232444311)>>2;
} /* set_seed() */

/**
 * Automate starting and ending of a situations.
 * These situations can be invoked manually with start_situation.
 * The automated starting and ending is unaffected by the room 
 * unloading.  When the room reloads the situation will be 
 * restarted unless its duration is up.
 * You must include the file situations.h for the definitions
 * of the when masks.
 * @param label (mixed) label of the situation to start 
 *  up.  If you pass an array such as ({ "frog1", "frog2" }) for the 
 * label then that set of situations are started one at
 * a time and the total duration is split evenly between them.
 * Label is usually an integer or a string or an array of
 * integers and/or strings.
 * If the string is a list of labels
 * separated by , then multiple situations
 * are started using those labels.
 * @param duration (int) total time (seconds) the overall situation 
 * should last.  If an array is specified for duration each
 * situation gets it's own little one.  If -1 is specified as
 * a duration for one part that situation is background complimentary
 * situation to the rest that is on when the rest are off.
 * @param when (int) a time of the day mask.  This limits when
 * the situation is allowed to occur.  The mask is composed of
 * the allowed hours in AM time ( 24 hours clock, (1<<hour) and 
 * combined with | (OR) ).   You can just use these
 * predefined masks and ingore how it works:
 *  WHEN_WEE_HOURS, WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_AFTERNOON
 *  WHEN_EVENING, WHEN_LATENIGHT, WHEN_MIDDAY, WHEN_MORNING, 
 *  WHEN_EARLY_MORNING, WHEN_LATE_MORNING, WHEN_NIGHT, WHEN_DAY
 *  WHEN_ANY_TIME    
 * The masks are defined in /include/situations.h.
 * @param chance  (int) chance in 1000 of starting the situation
 *  This is tested every duration seconds.
 * @see add_situation
 * @see start_situation
 * @see end_situation
 * @see evolvingroom.h
 * @example
 * #include <situations.h>
 *
 *  automate_situation( "frog", 300, WHEN_ANY_TIME, 200 );
 *
 * This will automatically start the situation labelled "frog" 
 * at a random time that is any time of the day with a 200/1000
 * chance of it starting per 300 seconds.  It will last for
 * 300 seconds (5 minutes).  
 * @example
 *  automate_situation( ({"frog1","frog2"}), 240, WHEN_EVENING|WHEN_NIGHT,
 * , 300 );
 *
 * This will automatically start a situation that is a combination 
 * of "frog1" followed by "frog2" at a random time but only in the
 * evening or at night.  There will be a 300/1000 chance of it 
 * starting per 240 seconds.  Both the "frog1" and "frog2" 
 * situations will get half the total time (as there are two),  
 * 120 seconds each, for a total duration of 240 seconds (4 minutes).
 */
void automate_situation( mixed label, mixed duration, mixed when, mixed chance,
     mixed category ) {
  mixed chatargs,endat,background,labels;
  int i,d;
  string ook;

//  tell_creator("shaggy","Automating %O\n",label);

  if (!offset) {
    //    tell_creator("shaggy","automate_sit: %O %O %O\n",ownerroom,this_object(),previous_object()); 
    ook=file_name(ownerroom);
    if (!ook || ook=="") {
      write( "Bad file name" );
      return;
    }
    /* generate a seed value that is all the characters in the
       filename multiplied together:  not wonderful but you
       can specify your own with situation_changer_seed */
    offset=1;
    foreach (i in ook) offset*=i;
    /* The >>2 makes time() + offset always positive */
    offset=(offset>>2)+1;
    
  }    
  if (ownerroom->query_chatter()) {
    chatargs=(ownerroom->query_chatter())->query_room_chats();
    if (sizeof(chatargs)==3) {
      chatmin=chatargs[0];
      chatmax=chatargs[1];
    }
  }

  endat=({ });
  if (arrayp(duration)) {
    if (!arrayp(label) || sizeof(duration)!=sizeof(label)) {
      write("Duration must be an array of the same length as label");
      return;
    }
    d=0;
    labels=({ });
    for (i=0;i<sizeof(duration);i++) {
      if (duration[i]!=-1) { 
	d+=duration[i]; /* -1 means infinite duration background situation */
        endat+=({ d });
	labels=labels+({ label[i] });
      }
      else {
	if (background) background=implode(explode(background,",")+({ label[i] }),",");
        else background=label[i];
      }
    }
  } else {
    labels=label;
    d=duration;
    if (arrayp(label)) {
      for (i=0;i<sizeof(label);i++) {
        endat+=({ ((i+1)*duration)/sizeof(label) });
      }
    } else {
      endat=({ duration });
    }
  }
  //  tell_creator("shaggy","%O endat %O\n",label,endat);

  if (!sittiming) 
    sittiming=
      ({ new(class situation_timing, label: labels, duration: d,
             when: when, chance: chance, endat: endat, background: background,
             category: category, it: 0, it_data: ([ ]) ) });
  else 
    sittiming=sittiming+
      ({ new(class situation_timing, label: labels, duration: d,
             when: when, chance: chance, endat: endat, background: background,
             category: category, it: 0, it_data: ([ ]) ) });
  automate=2; /* start sleeping */
} /* automate_situation() */

/**
 * Shuts down all current and pending situations.  It also turns off the
 * automated situation manager so no more are added.  It does not
 * destruct this object so all the add_situations are still loaded
 * and may be recommenced with automate_situation or change_situation.  
 * dest_me is the appropriate call to permanently remove all situations.
 * @see automate_situation
 * @see change_situation
 */
void shutdown_all_situations() {
  int label;
  mixed h;

  if (sizeof(cco)>1) {
    foreach (h in cco[1..<1]) {
       if (h) remove_call_out(h);
    }
  }
  cco=({ 0 });
  if (currentsits && sizeof(currentsits)) {
    foreach(label in currentsits) end_situation(label);
  }
  autosits=0;
  automate=0;
} /* shutdown_all_situations() */

/**
 * Shuts down a change_situation based on the call_out handle
 * returned by the call to change_situation.
 * @param handle call_out handle.  If 0 then the last
 * known handle is used.
 * @param label label or array of labels of situations to clean 
 * up with end_situation
 * @param the_room the room
 * @see automate_situation
 * @see change_situation
 */
void shutdown_situation(int handle, mixed label) {
  mixed frog,bing;

  if (sizeof(cco)) {
    if (!handle) handle=cco[0];
    if (handle && cco[handle]) remove_call_out(cco[handle]);
    cco[handle]=0;
  }

  if (arrayp(label)) { 
    frog=({ });
    foreach(bing in label) {
      if (stringp(bing)) frog+=explode(bing,",");
      else frog+=({ bing });
    }
  }
  else if (stringp(label)) label=explode(label,",");
  else label=({ label });

} /* shutdown_sit uation() */

string extra_look() {
  if (!els || !sizeof(els)) {
    return "";
  }
  return implode(els,"  ")+"\n";
} /* add_extra_look() */
