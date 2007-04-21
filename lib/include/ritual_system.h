/**
 * Rituals.h
 * A header file for rituals.
 * @author Terano.
 */
#define EFFECTS "/std/effects/faith/"
#define ITEMS "/d/guilds/priests/items/"
#define RITUALS "/obj/rituals"
#define ROOMS "/d/guilds/priests/rooms"
#define INHERITS RITUALS "/inherits"

#define SHADOWS "/std/shadows/faith/"
//Target types!
#define RITUAL_DIRECT_NONE       0
#define RITUAL_DIRECT_ROOM       1 
#define RITUAL_DIRECT_NON_LIVING 2
#define RITUAL_DIRECT_LIVING     4
#define RITUAL_DIRECT_SELF       8
#define RITUAL_DIRECT_MULTIPLE   16
#define RITUAL_DIRECT_EXTERN     32

//Something to show something!
#define RITUAL_CALCULATE_DYNAMIC  0

//Some imbue stuff
#define IMBUE_PERFORMER -1
#define IMBUE_PRIEST     1

//Special Stages
#define SUCCESS -1
#define FAILURE -2
#define STOPPED -3

//Some files!
#define RITUAL EFFECTS"/basic_ritual"
#define SHADOW SHADOWS"/basic_ritual"

//Impediments and benefits
#define RELIGIOUS   "religious"
#define MENTAL      "mental"
#define SPEECH      "speech"
#define VISION      "vision"
#define MOVEMENT    "movement"
#define ENVIRONMENT "environment"
#define MISC        "misc"

//A skill define
#define FAMILY "faith.rituals" + info->family

//Some defines to make filing a breeze!
#define CURING_INHERIT INHERITS "/basic_healing"
#define SHIELD_INHERIT INHERITS "/basic_shield"
#define PASSAGE_INHERIT INHERITS "/basic_passage"
#define ALCOHOL_INHERIT INHERITS "/basic_alcohol"
#define WARD_BASE SHADOWS +"/base_ward"

//Some common data structures.
/**
 * Each ritual has one or more stages.
 * Each stage must have messages for both the player and external parties.
 * Each stage can optionally have a skill check.
 * Each stage can optionally have items which can be consumed.
 * Each stage can be optionally lengthened, this will then be modified by 
 * other factors. [alignment and skill]. This defaults to 1 if not entered.
 */

class stage {
  int key;               //The identifier for this stage. 
  string *messages;      //Messages [internal and external]
  string skill;          //Skill required for this stage. [Optional]
  int level;             //Power level for this stage.
  string *materials;     //Materials required for this stage. [Optional]
  string *consumables;   //Materials consumed for this stage. [Optional]
  int pause;             //Mandatory Pause - Required for complex rituals. [Opt]
  int time_modifier;     //A base modifier used to determine stage length. 
  function success_func; //A function that will be evaluated if successful.
  string prayer;         //The name of a prayer required for this stage.
  int imbue_source;       //The stage is either set on imbuing or set on casting.
                         //Options are IMBUE_PERFORMER or IMBUE_PRIEST
}

/**
 * Data Storage for Rituals
 */
class ritual {
   //Standard variables: Will be the same for every casting.
   string name;                     //Name of the ritual.
   string family;                   //Type of ritual [Defensive, Curing, etc].
   int base_gp_cost;                //Base cost to cast ritual
   int *align;                      //Alignment restriction
   int targets;                     //An int for the purposes of targeting!
   class stage *stages;             //See info on stages below.
   int pk_check;                    //Is this ritual PK checked?
   int teach_base;                  //Base level for teaching
   int learn_base;                  //Base level for learning
   string training_skill;           //Skill used for teaching
   int movement;                    //Is movement required to cast this ritual.
   int speech;                      //Is speech required to cast this ritual.
   int vision;                      //Is vision required to cast this ritual.
   string *materials, *consumables; //Same as for each stage.
   function target_check;           //Can be set, returns 1 if can be targetted
   string resist_skill;             //Skill that resists the effects.
   string help;                     //Path of the help file.
   int offensive;                   //Is this a ritual that initialises combat

   //Dynamic variables: Change for each instance of the ritual.
   object caster;        //The caster
   object imbue;         //The imbue being performed from [optional]
   object *rit_targets;  //Targets of our ritual
   object *resistors;    //Those who successfully made a resist check.
   int current_stage;    //Stage the ritual is currently at.
   int start_time;       //Time ritual commenced.
   int target_count;     //The count of number of targets!
   function valid;       //Function used to validate casting. 
   int degree;           //The degree of success.
   string deity;         //Deity of the caster.
   mapping difficulties; //Things that make the ritual harder. 
   mapping benefits;     //Things that make the ritual easier.
}
