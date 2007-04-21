#define TRICK_HANDLER     load_object( "/obj/spells/trick_handler" )
#define VIRTUAL_SPELL_OB  "/obj/spells/virtual_spell"
#define SET_STONE         "/obj/magic/lg_hag_stone_set"
#define UNSET_STONE       "/obj/magic/lg_hag_stone_unset"
#define VIRTUAL_SPELL_EFF "/std/effects/magic/virtual_spell"

#define SPELL_DIRECT_NON_LIVING 1
#define SPELL_DIRECT_LIVING     2
#define SPELL_UNDIRECTED        3

#define SPELL _spell->name
#define STAGE _spell->stages[ stage_num - 1 ]

#define STAGE_COST  20
#define DIFF_POWER  25
#define STAGE_TIME  10

class spell_stage {
  string skill;      // the skill checked
  int difficulty;    // the difficulty of the check
  string tell_msg;   // the msg the witch sees 
  string say_msg;    // the msg the audience sees
  string *consumed;  // spell component(s) consumed during stage
} 

class spell_info {
  string name;                // the name of the spell
  int target_type;            // what it's directed at
  class spell_stage *stages;  // data for each stage
  string *needed;             // spell component(s) needed (not consumed)
  string tell_final;          // msg to witch when successful
  string say_final;           // msg to room when successful
}

class eff_args {
  object obj;
  class spell_info spell;
  int id;
}

/* --- MySQL Stuff --- */
/* 
The columns of the TRICKS table are as follows:
create table Tricks (
  TrickId smallint unsigned not null auto_increment,
  TrickName char(255) default '',
  Author char(255) default '',
  TimeSet int(12) unsigned default '0',
  Guild char(10) default '',
  TimesCast smallint(5) unsigned default '0',
  Witnesses double default '0.0',
  Canonised tinyint(3) default '0',
  Shared tinyint(1) default '0',
  MostRecentTime int(12) unsigned default '0',
  MostRecentCaster char(255) default '',
  TargetType tinyint unsigned default '0',
  Needed blob,
  FinalTell char(255) default '',
  FinalSay char(255) default '',
  Ritual blob,
  primary key( TrickId )
);
*/

#include <db.h>
#include <config.h>

#define DB            "discworld"
#define TRICKS        "Tricks"
#define USER_ID       CONFIG_DB_USER
#define PASSWORD      ""
#define MAXID         "max( TrickId )"
#define ID_EXISTS     1
#define DATA_EXISTS   2
