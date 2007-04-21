/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: lord.c,v 1.15 2002/03/03 23:18:01 ceres Exp $
 */
/**
 * This is the lord player object and contains any needed bits to
 * handle lord specific commands and things.
 * @author Pinkfish
 */
/* this is the lord player object */

/* support added for automatically mailing docs to new cre's upon */
/* being employd - Funtime, 24/5/95 */
#define CREATOR_DOC "/doc/creator/concepts/creator_doc.txt"
//#define PROJECT_STYLE "/doc/creator/concepts/project_style.txt"

#include <command.h>
#include <mail.h>

inherit "/global/wiz_file_comm";

protected int do_dismiss(string str, string reason);
protected int employ(string str);
protected int new_domain(string dom, string director);
private int do_heart_beat();
protected int do_qsnoop(object *obs);
protected int bulk_delete( string word );
protected int clean_up_files( string word );
protected int do_hexec(string);

void create() {
  ::create();
} /* create() */

/** @ignore yes */
void move_player_to_start(string bong, int bing, string c_name, string ident,
                          int go_invis) {
  ::move_player_to_start(bong, bing, c_name, ident, go_invis);
  cat("doc/CREATORNEWS");
  cat("doc/DIRECTORNEWS");
#ifndef NEW_DRIVER
  enable_wizard();
#endif
   switch ( query_invis() ) {
      case 3 :
         tell_object( this_object(),
               "===> You are currently Trustee invisible! <===\n" );
         break;
      case 2 :
         tell_object( this_object(),
               "===> You are currently Director invisible! <===\n" );
         break;
      case 1:
         tell_object( this_object(),
               "===> You are currently invisible! <===\n" );
         break;
   }
  add_command("qsnoop", this_object(), "<indirect:player>", (: do_qsnoop($1)
                                                             :));
  add_command("qsnoop", this_object(), "", (: do_qsnoop :));
  add_command("employ", this_object(), "<word>", (: employ($4[0]) :));
  add_command("dismiss", this_object(), "<word> <string>", (: do_dismiss($4[0], $4[1]) :));
  add_command("new_domain", this_object(), "<word> <word>",
              (: new_domain($4[0], $4[1]) :) );
  add_command("heart_beat", this_object(), "", (: do_heart_beat() :) );
  add_command("bulk_delete", this_object(),
              "{a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z}",
              (: bulk_delete($4[0]) :) );
  add_command("clean_up_files",  this_object(), "<word>",
              (: clean_up_files($4[0]) :) );
  AddSearchPath(({ DIR_LORD_CMDS, DIR_SECURE_LORD_CMDS }));
} /* move_player_to_start() */

/**
 * No priorities etc anymore, so directors get there own very small
 * process_input parser(tm)
 */

protected mixed process_input(string input){
  if(input == "end_it_all")
    shutdown(0); 
  if(input[0..4] == "hexec")
    return do_hexec(input[5..]);
  return ::process_input(input);
}

/**
 * Very fast shutdown.
 * @return always returns 1
 */
protected int end_it_all() {
  shutdown(0);
  return 1;
} /* end_it_all() */

/**
 * This method runs the heart beat once on command.
 */
private int do_heart_beat() {
  heart_beat();
  return 1;
} /* do_heart_beat() */

/** @ignore yes */
int query_creator() {
  return 1;
} /* query_creator() */

nomask int query_director() {
  return 1;
}

nomask int query_lord() {
  return 1;
} /* query_lord() */

/**
 * This method is called when a director  attempts to employ
 * someone in their domain.
 * @param str the player to employ
 * @return 1 on success, 0 on failure
 */
protected int employ(string str) {
  string doc;

  if (GetForced()) {
    return 0;
  }
/*  set up mail to send to employeee */
/* added by FUntime, 24/5/1995 */
  /*
   * Change by pinkfish so it actually checks the return value of the
   * promotion code first.
   */
  switch (master()->employ_creator( lower_case(str) )) {
    case 2 :
      if(file_size(CREATOR_DOC) > 0) {
        /* load up file into 'doc' */
        doc = read_file(CREATOR_DOC);
        /* and post it on */
        AUTO_MAILER->auto_mail(lower_case(str), this_player()->query_name(),
          "Documents for new creators", "", doc, 0, 0);
      }
    
     /* if(file_size(PROJECT_STYLE)>0) {
        doc = read_file(PROJECT_STYLE);
        AUTO_MAILER->auto_mail(lower_case(str), this_player()->query_name(),
          "Documents for new creators", "", doc, 0, 0);
      } */
      return 1;
    case 1 :
      return 1;
    default :
      return 0;
  }
} /* employ() */

/**
 * This method is called when the director attempts to dismiss
 * a creator who is currently working (or not working
 * if they are being dimissed I guess).
 * @param str the creator to dismiss
 * @return 1 on success, 0 on failure
 */
protected int do_dismiss(string str, string reason) {
  if (GetForced()) {
    return 0;
  }
   return (int)master()->dismiss_creator( str + " " + reason);
} /* do_dismiss() */

/**
 * This method is used to create a new domain.
 * @param str the input parameters
 * @return 1 on success, 0 on failure
 */
protected int new_domain(string dom, string director) {

  if (GetForced()) {
    return 0;
  }
  return (int)master()->create_domain(dom, director);
} /* new_domain() */

/** @ignore yes */
string query_object_type(object) {
  if (master()->query_trustee(geteuid(this_object())))
    return "T";
  return "D";
} /* query_object_type() */

/** @ignore yes */
void event_enter(object me, string s1, object from) {
  if ((!s1 || s1 == "") && interactive(me)) {
    if (environment() == environment(me))
      event_inform(me, me->query_cap_name()+" invisibly enters the room",
                   "enter");
    else
      event_inform(me, me->query_cap_name()+" invisibly enters your inventory",
                   "enter");
  }
  ::event_enter(me, s1, from);
} /* event_enter() */

/** @ignore yes */
void event_exit(object me, string s1, object from) {
  if ((!s1 || s1 == "") && interactive(me)) {
    if (environment() == environment(me))
      event_inform(me, me->query_cap_name()+" invisibly exits the room",
                   "enter");
    else
      event_inform(me, me->query_cap_name()+" invisibly exits your inventory",
                   "enter");
  }
  ::event_exit(me, s1, from);
} /* event_exit() */

/** @ignore yes */
protected int do_hexec(string junk) {
  if (GetForced()) {
    return 0;
  }
  write_file("/w/"+query_name()+"/exec_thing.c", "void create() { "+junk+"; }");
  catch(("/w/"+query_name()+"/exec_thing")->bingle());
  destruct(find_object("/w/"+query_name()+"/exec_thing"));
  rm("/w/"+query_name()+"/exec_thing.c");
  return 1;
} /* do_hexec() */

/** @ignore yes */
varargs int adjust_xp(int number, int shared) {
  if (objectp(previous_object())) {
    event_inform(previous_object(),
                 (string)previous_object()->query_name() + " (" +
                 file_name(previous_object()) +") gives you " +
                 number + " xp", "xp");
  }
  return ::adjust_xp(number, shared);
} /* adjust_xp() */


/**
 * This method forces a bulk delete on a certain letter.
 * This letter will then be checked to see if any of
 * those players have timed out and should be deleted.
 * @param word the letter to bulk delete
 * @return 0 on falure and 1 on success
 */
protected int bulk_delete( string word ) {
  if (GetForced()) {
    return 0;
  }
  word = lower_case(word);
  return (int)"/secure/bulk_delete"->delete_files(word);
} /* bulk_delete() */

/**
 * This method forces the clean up of certain file son the
 * bulk deleter.  This cleans up all the files associated with
 * the given player.
 * @param word the name of the player to clean up
 * @return 1 on success, 0 on failure
 */
protected int clean_up_files( string word ) {
  if (GetForced()) {
    return 0;
  }
  return (int)"/secure/bulk_delete"->clean_up_files(word);
} /* clean_up_files() */

/**
 * This method is used to do a quiet snoop on a player.
 * @param str the player to snoop
 * @return 1 on success, 0 on failure
 */
protected int do_qsnoop(object *obs) {
  object targ;

  if (GetForced()) {
    return 0;
  }
  if (!obs || !sizeof(obs)) {
    snoop(this_object(), 0);
    write("Ok, qsnoop cancelled.\n");
    return 1;
  }
  targ = obs[0];
  if(!snoop(this_object(), targ)) {
    notify_fail("You fail to qsnoop " + targ->query_cap_name() + ".\n");
    return 0;
  }
  write("Ok, qsnooping "+targ->query_cap_name() + ".\n");
  return 1;
} /* do_qsnoop() */
