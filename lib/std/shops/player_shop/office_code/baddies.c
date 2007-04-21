/******************************************************************************
 * This file contains banning-related functions
 *****************************************************************************/

/**
 * @ignore yes 
 * Ban this person from the shop.
 * This person will be banned for a specific period, and will not be allowed
 * back in the shop within this period.  People can be banned by managers, or
 * automatically for attacking employees.
 * @param word person to ban
 * @param reason reason for the ban
 * @param banner person implementing the ban
 */
private void add_baddie(string word, string reason, string banner)
{
   object ob;

   word = lower_case(word);
   if (query_baddie(word))
   {
      /*
       * Added this bit after discovering someone had been banned 75
       * times for killing Gretta @ Tarnach's- all at the same time.
       */
      if (_baddies[word][BAD_TIME] == time() ||
         reason == _baddies[word][BAD_REASON]) return;
      remove_baddie( word );
   }
   if (!sizeof(_baddies)) _baddies = ([word:BAD_MAP]);
   else _baddies += ([word:BAD_MAP]);
   _baddies[word][BAD_REASON] = reason;
   _baddies[word][BAD_BANNER] = banner;
   _baddies[word][BAD_TIME] = time();
   save_me();
   shop_log(GENERAL, banner, "banned "+ cap_name(word)+
      " for "+ reason, PAID);
   employee_log(word, sprintf("Banned by %s for %s.", cap_name(banner),
      reason));
   fire_them(_proprietor, word, reason);
   remove_applicant(word);
   if (!(ob = find_player(word))) return;

   /* If this person is in the shop front, move them outside. */
   if (environment(ob) == find_object(_shop_front))
   {
      tell_room(_shop_front, ob->query_short()+ " drifts out of the door, "
        "seemingly against "+ ob->query_possessive()+ " will.\n", ({ob}));
      tell_object(ob , "You feel yourself pushed out of the shop "
        "by a mysterious force.\n");
      tell_room(_shop_front->query_outside(), ob->query_short()+
        " drifts through the door of "+ _shop_name +
        ", seemingly against "+ ob->query_possessive()+ " will.\n", ({ob}));
      ob->move(_shop_front->query_outside());
   }
}
/* add_baddie() */

/**
 * @ignore yes 
 * Managers' office.
 * Ban a person from the shop.
 */
int do_ban(mixed *args)
{
   if (!test_player(lower_case(args[0])))
   {
      tell_object(this_player(), args[0]+ " is not a player.\n");
      return 1;
   }
   if (_employees[args[0]][EMP_POINTS] & MANAGER &&
      !this_player()->query_creator())
   {
      tell_object(this_player(), "You cannot ban "+ cap_name(args[0])+
         ".\n");
      return 1;
   }
   add_baddie(args[0], args[1], this_player()->query_name());
   tell_object(this_player(), "You ban "+ cap_name(args[0])+
     " for "+ args[1]+ ".\n");
   return 1;
}
/* do_ban() */

/**
 * @ignore yes 
 * Managers' office.
 * Remove the ban on a person.
 */
int do_unban(string person)
{
   if (!query_baddie(person))
   {
      tell_object(this_player(), person+ " is not currently banned.\n");
      return 1;
   }
   remove_baddie(person);
   shop_log(GENERAL, this_player()->query_name(),
     "removed the ban on "+ cap_name(person), PAID);
   tell_object(this_player(), "You remove the ban on "+ 
     cap_name(person)+ ".\n" );
   return 1;
}
/* do_unban() */

/**
 * @ignore yes 
 * Remove this person from the list of people banned from the shop.
 * @param word the person to unban
 */
private void remove_baddie(string word)
{
   if (!query_baddie(word)) return;
   map_delete(_baddies, word);
   save_me();
}
/* remove_baddie() */
