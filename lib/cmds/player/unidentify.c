/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unidentify.c,v 1.1 2001/09/26 01:45:09 presto Exp $
 *
 *
 */

#define EFFECT "/std/effects/object/identity_hold"

inherit "/cmds/base";

mixed cmd(object *indirect_obs)
{
   object *things;

   things = filter(indirect_obs, (: $1->query_identifier() :));
   if (!sizeof(things))  {
      if (sizeof(indirect_obs) > 1)
         write("You do not have any of those things identified.\n");
      else
         write("You do not have " + indirect_obs[0]->the_short() +
               " identified.\n");
   }
   else  {
      things->set_identifier(0);
      write("You stop identifying " + query_multiple_short(things, "one") +
            ".\n");
   }

   return 1;
}


mixed *query_patterns() {
   return ({ "<indirect:object:me'object(s)'>", (: cmd($1) :) });
} /* query_patterns() */
