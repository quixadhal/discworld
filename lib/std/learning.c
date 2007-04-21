/* Higher level inheritable for learning a skill
   This could be inherited by a room or an NPC
   Belle April 2002
*/

#include <money.h>
#include <tasks.h>

class step {
  string *blurbs;
  string skill;
  int difficulty;
  string *responses;
  string *params;
  string award_mess;
  string succeed_mess;
  string fail_mess;
}

class lesson {
  mapping minimum_skills;
  mapping maximum_skills;
  string *materials;
  int cost;
  string currency;
  string *lesson_ending;
  object teacher;
  class step *plan;
}

private nosave mapping _lessons;
private nosave int current_step;
private nosave int attempt;

int lesson_response(string, object, int, string);

/**
 * Register a lesson with the learning system.
 *
 * @param name the name of the lesson
 * @param minimum_skills a mapping of skill names & bonuses that are
 *  required in order to take this lesson
 * @param maximum_skills a mapping of skill names & bonuses that are the
 *  maximum allowed to take this lesson
 * @param materials a list items required in order to take the lesson
 * @param cost the price of the lesson
 * @param currency the name of the currency required for the lesson
 * @param lesson_ending an array of strings that happen at the end the lesson to end it
 * @param teacher an optional teacher object
 * 
 */
int register_lesson(string name, mapping minimum_skills,
                    mapping maximum_skills, string *materials, int cost, 
                    string currency, string *lesson_ending, object teacher ) {
  if(!_lessons)
    _lessons = ([ ]);
  
  if(_lessons[name])
    return 0;
  
  _lessons[name] = new(class lesson,
                       minimum_skills : minimum_skills,
                       maximum_skills : maximum_skills,
                       materials : materials,
                       cost : cost,
                       currency : currency,
                       lesson_ending : lesson_ending,
                       teacher : teacher
                       );
  return 1;
}


/** 
 * Add a step to a lesson plan.  This is the interaction between student and 
 * teacher. One step should be defined for each thing you want the student
 * to learn.
 *
 * @param lesson_name the name of the lesson this lesson plan is 
 * associated with. 
 * @param blurbs An array of strings in order that they should be given 
 * during the lesson.  This is what the teacher says to the student. ', :, #, etc. 
 * can be used for say, emote and execute a command respectively.
 * @param skill The skill being learned.
 * @param difficulty The bonus level of the skill being learned.
 * @param responses An array of strings.  These are the expected responses 
 * to the teacher's blurbs.
 * @param params an array of strings given as arguments to the responses in 
 * order.
 * @param award_mess The string a player sees if they get a TM award during this step. It
 * should not end in a newline \n.
 * @param succeed_mess The string a player gets if they are successful in doing what the 
 * step asks.  It should not end in a newline \n.
 * @param fail_mess A string telling the player that they've failed.  It should not 
 * end in a newline \n.
 */
int add_step( string lesson_name, string *blurbs, string skill, int diff,
        string *responses, string *params, string award_mess, string succeed_mess, 
        string fail_mess) {

  if(!_lessons)
    _lessons = ([ ]);
  
  if( !_lessons[lesson_name] ){
    return 0;
  }
  if( !_lessons[lesson_name]->plan ){
    _lessons[lesson_name]->plan = ({ });
  }

  _lessons[lesson_name]->plan += ({ new(class step,
          blurbs : blurbs,
          skill : skill,
          difficulty : diff,
          responses : responses,
          params : params,
          award_mess : award_mess,
          succeed_mess : succeed_mess,
          fail_mess : fail_mess
          ) });
  
  return 1;
}


/** has_required_skills( string name, object player )
 * Skills: Skills and skill levels which may be necessary to learn something
 * For instance, one should learn to walk before one learns to run.  If you
 * don't have sufficient skill levels in walking, you won't be able to learn
 * how to run.
 * @param name name of the lesson they're trying to learn
 * @param player player object of the player trying to learn
 */

 
int has_required_skills( string name, object player ) {
  string skill;
  int p_level;

 // make sure the player meets the minimum skill requirements 
  foreach( skill in (keys(_lessons[name]->minimum_skills)) ){
    p_level = player->query_skill(skill);
    // debug_printf( "Skill check: Player has %d in %s.  Must be a min of %d", 
    //      p_level, skill, _lessons[name]->minimum_skills[skill]);
    if( p_level < _lessons[name]->minimum_skills[skill] ){
      // player doesn't have enough skill to learn this
      return 0;
    } 
  }
  
 // make sure the player is under the maximum skill requirements 
  foreach( skill in (keys(_lessons[name]->maximum_skills)) ){
    p_level = player->query_skill(skill);
    //debug_printf( "Skill check: Player must have a max of %d",
    //      _lessons[name]->maximum_skills[skill] );
    if( p_level > _lessons[name]->maximum_skills[skill] ){
      // player has too much skill to learn this
      return 0;
    }
  }

  return 1;
}

/** has_required_materials( string name, object player )
 *  Materials: Materials are objects which are necessary for a student to learn
 * something.  For instance, to learn to speak a language, you might need
 * a textbook. To learn to ride a horse, you might want to require a helmet.
 * @param name name of the lesson they're trying to learn
 * @param player player object of the player trying to learn
 */
int has_required_materials( string name, object player ) {
  // if it's in their inventory they have it
  string mat;
  
  foreach( mat in _lessons[name]->materials ) {
    if( sizeof( filter_array(deep_inventory(player),
           (: ($1)->query_short() :) ) ) ){
      return 1;
    }
  }
  // these are not the materials we're looking for
  return 0;
}
/** has_required_cash( string name, object player )
 * Give some, get some. Money makes a teacher more likely to teach you.
 * @param name name of the lesson they're trying to learn
 * @param player player object of the player trying to learn
 */

int has_required_cash( string name, object player ) {
  //debug_printf( "Cash check: Player has %d in %s needs to have %d", 
  //              player->query_value_in(_lessons[name]->currency), 
  //              _lessons[name]->currency, _lessons[name]->cost );
  if(player->query_value_in(_lessons[name]->currency) < _lessons[name]->cost) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * This method starts a lesson for a given player.
 * @param the lesson name
 * @param the player
 */
int start_lesson(string name, object player) {
  string *needed;
    
  if(!_lessons[name])
    return notify_fail(player, "Sorry, it's not clear what you want to learn "
                "here.\n");

  needed = ({ });
  if(!has_required_skills(name, player))
    needed += ({ "skills" });
  if(!has_required_materials( name, player ))
    needed += ({ "materials" });
  if(!has_required_cash( name, player ))
    needed += ({ "cash" });

  
  if(sizeof(needed)) {
    debug_printf( "Player doesn't have the correct %s\n", 
                  query_multiple_short(needed) );
 
    return add_failed_mess("Sorry, You don't have the correct " +
                  query_multiple_short(needed) + " to learn today.\n");
  }
  
  
  // Call the lesson step function for the first step.
  call_out("lesson_step", 1, name, player, 0);
  return 1;
}

/**
 * This method returns an array of the registered lessons.
 * @return a list of lesson names.
 */
string *query_lessons() { return keys(_lessons); }

protected void lesson_step(string name, object student, int lessonstep) {
  string str;
  class step current;
  object teacher = _lessons[name]->teacher;
  int i;
  
  // The current step in the lesson plan.
  current = _lessons[name]->plan[lessonstep];
  //tell_creator( "belle", "Current lessonstep is %O\n", current );

  // register response commmand
  str = "";
  //tell_creator( "belle", "command %s\n params %s\n", current->responses[0], current->params[0] );
  //tell_creator( "belle", "Sizeof current responses is %d", sizeof(current->responses) );

  for(i=0; i<sizeof(current->responses); i++) {
    if(current->params && sizeof(current->params) > i) {
      str = " <string:'" + current->params[i] + "'>";
      //tell_creator( "belle", "Str is %s\n", str );
      student->add_command(current->responses[i], this_object(), str,
                           (: lesson_response($(name), $(student),
                                              $(lessonstep), $4) :));
      //tell_creator( "belle", "Added command %s to %O with extra string %s\n",
      //              current->responses[i], this_object(), str );
    } else {
      str = "";
      student->add_command(current->responses[i], this_object(), str,
                           (: lesson_response($(name), $(student),
                                              $(lessonstep), "") :));
      //tell_creator( "belle", "Added command %s to %O with extra string %s\n", 
      //              current->responses[i], this_object(), str );
    }
  }

  // output blurbs (via teacher if appropriate)
  foreach( str in (current->blurbs) ) {
   if( teacher )
      teacher->queue_command(str);
    else
      tell_object(student, str);
  }

}

int lesson_response(string name, object student, int lessonstep, string args) {
  class step current;
  
  // validate response
  if(this_player() != student)
    return 0;

  current = _lessons[name]->plan[lessonstep];
  //tell_creator( "belle", "Args is %O\nSizeof Current-params is %d\n", args[0], sizeof(current->params) );
  
  // If this step has response paramters we need to validate them.
  if(sizeof(current->params) && 
     member_array(args[0], current->params) == -1)
    return student->add_failed_mess(this_object(), "Invalid response.\n",
            ({ }));
  
  // perform TM check
  //tell_creator( "belle", "TM Check skill: %s diff: %d\n", current->skill, current->difficulty );
  switch(TASKER->perform_task(student, current->skill, current->difficulty,
                              TM_FIXED)) {
  case AWARD:
    // display award message
    tell_object(student, "%^YELLOW%^"+current->award_mess+"%^RESET%^\n");
    
  case SUCCEED:
    //move on.
    tell_object( student, current->succeed_mess + "\n");

    // This removes the last add_command.
    student->remove_object(this_object(), 1);

    //tell_creator( "belle", "lessonstep is %d, number of lesson steps is %d\n",
    //		  lessonstep, (sizeof(_lessons[name]->plan)-1) );
    // Is this the last step?
    if( (sizeof(_lessons[name]->plan)-1) == lessonstep ) {
      tell_creator("belle", "Lesson is on its last step: %d\n", lessonstep );
      call_out( "lesson_end", 1, name, student ); 
      return 1;
    } else {
      call_out("lesson_step", 1, name, student, ++lessonstep);
    }
    break;
    
  default:
    // repeat
    tell_object(student, current->fail_mess + "\n" );
    break;
  }
  return 1;
}

int lesson_end ( string name, object student ) {
  string str;
  object teacher = _lessons[name]->teacher;
  
  // output blurbs (via teacher if appropriate)
  foreach( str in (_lessons[name]->lesson_ending) ) {
    if( teacher )
      teacher->queue_command(str);
    else
      tell_object(student, str);
  }  
  return 1;
}
