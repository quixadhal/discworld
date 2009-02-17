/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: spells.c,v 1.7 2002/07/10 00:07:17 taffyd Exp $
 * $Log: spells.c,v $
 * Revision 1.7  2002/07/10 00:07:17  taffyd
 * Made it possible to delete spells which have no name.
 *
 * Revision 1.6  2001/11/12 22:48:34  pinkfish
 * Fix up the reset_spells() thingy.
 *
 * Revision 1.5  2001/11/12 22:46:32  pinkfish
 * Add in a protected method to reset spell data,.
 *
 * Revision 1.4  2001/11/12 22:21:24  pinkfish
 * Add in some docs.
 *
 * Revision 1.3  2001/08/15 20:29:49  tannah
 * Fix0rd a bug in query_spells_nocase()
 *
 * Revision 1.2  2001/08/15 04:11:32  presto
 * Added code to handle spell names of any upper/lowercase
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 *
*/
#include <spells.h>

class spell_process {
   mapping spells;
   mapping process;
}

private mixed spells;

void create() {
   spells = ([ ]);
}

/**
 * This method returns the string help for the spell.  It is used by the
 * help command to find the help for the spell.
 * @param str the spell to find help on
 * @return the help string of the spell
 */
string help_spell(string str) {
   if (!mapp(spells)) {
     spells = ([ ]);
   }
   if (!m_sizeof(spells)) {
      return 0;
   }
   if (!spells[str]) {
      return 0;
   }
   return (string)spells[str][S_OBJECT]->help(str);
}

/**
 * This method returns the current bunch of spells for the object.
 * @return the spells
 */
mapping query_spells() {
   if (!mapp(spells)) {
      spells = ([ ]);
   }
   return spells;
}

/**
 * This method returns the list of spells without any case issues.
 * @return the list of spells without any case
 */
mapping query_spells_nocase() {
  mapping lc_spells = ([]);
  filter( this_object()->query_spells(),
                 (: $3 += ([ lower_case( $1 ) : $2 ]) :), lc_spells );
  return lc_spells;
}

/**
 * This method adds a spell into the current spell list.
 * @param name the name of the spell to add
 * @param ob the object the spell is on
 * @param func the function to call for the spell
 * @return 1 on success, 0 on failure
 */
int add_spell(string name, mixed ob, mixed func) {
   int i;
   mapping tmp;
 
   if (pointerp(spells)) {
      tmp = ([ ]);
      for (i=0;i<sizeof(spells);i+=2)
         tmp[spells[i]] = spells[i+1];
      spells = tmp;
   }
   if (!mapp(spells)) {
      spells = ([ ]);
   }
   spells[name] = ({ ob, func });
   return 1;
}

/**
 * This method removes the spell of the given name.
 * @param name the name of the spell to remove
 * @return 1 on succes, 0 on failure
 */
int remove_spell(string name) {
  string spell, name_lc;
  
  if ( !name ) {
      map_delete( spells, 0 );
      return 1;
  }

  name_lc = lower_case(name);
  foreach (spell in keys(spells)) {
     if (lower_case(spell) == name_lc) {
        map_delete(spells, spell);
     }
  }
  return 1;
}

/**
 * This method finds the data for the spell of the specific name
 * @param word the name of the spell to find data on
 * @return the data for the spell
 */
mixed query_spell( string word ) {
   mapping tmp_spells;
   string test_spell;

   if ( !mapp( spells ) ) {
      return 0;
   }

   tmp_spells = query_spells_nocase();
   test_spell = lower_case(this_object()->expand_nickname(word));
   return tmp_spells[test_spell];
} /* query_spell() */

/** 
 * This method resets all the spell data.
 */
protected void reset_spells() {
   spells = ([ ]);
} /* reset_spells() */
