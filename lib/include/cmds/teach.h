/**
 * This file contains all the stuff to do with the teaching respondable
 * command.
 * @author Pinkfish
 * @started Sun Aug 16 23:39:28 PDT 1998
 */
#ifndef __CMDS_TEACH_H__
/** @ignore yes */
#define __CMDS_TEACH_H__

/**
 * The class to keep track of everything to do with teaching a skill.
 */
class teaching_skill {
   /**
    * The skill to teach.
    */
   string skill;
   /**
    * The level the skill was at when we started teaching.
    */
   int lvl;
   /**
    * The number of levels of the skill to teach.
    */
   int num;
   /**
    * The amount of xp to spend on teaching the skill.
    */
   int xp;
   /**
    * This is used in the stop command for teaching the skill.
    */
   object teacher;
   /**
    * This is used iun the stop command for teaching the skill.
    */
   object taught;
   /**
    * This is the location we started the teaching in.  Fail if we moved.
    */
   object room;
}

/**
 * This is the type used for teaching skills.
 */
#define TEACH_SKILL_TYPE "teach skill"

/**
 * This is the type used for teaching commands.
 */
#define TEACH_COMMAND_TYPE "teach command"

/**
 * This is the property to use to test to see if the player has
 * automatic teaching turned on or not.
 */
#define TEACH_COMMAND_AUTO_PROPERTY "auto teach"

#endif
