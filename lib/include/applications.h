/**
 *  Applications 
 *  @author Drakkos
 *  @started 31/01/2003
 *
 */
 
#ifndef __APPLICATIONS_H
#define __APPLICATIONS_H

#define APPLICATIONS_HANDLER "/obj/handlers/applications_handler"
#define APPLY_COMMAND "/cmds/player/apply"
#define APPLY_SAVE_FILE "/save/applications"


#define MINUTE 60
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

class application {
  string type;
  string name;
  string text;
  mapping vouches;
  int time;
  string domain;


}

class application_type {
  string type; 
  string board;
  string *questions;
  int vouches_required;
  int gl_requirement;
  int qp_requirement;
  int age_requirement;
  int timeout;
  string intro_text;
  string responsibility;
} 

#endif
