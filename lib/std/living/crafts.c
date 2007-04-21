/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: crafts.c,v 1.2 2002/03/16 01:05:36 pinkfish Exp $
 * $Log: crafts.c,v $
 * Revision 1.2  2002/03/16 01:05:36  pinkfish
 * Fix up some things with undieing.
 *
 * Revision 1.1  2001/11/12 22:44:49  pinkfish
 * Initial revision
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
private mixed _processes;

void create() {
   _processes = ([ ]);
}

/**
 * This method returns the current bunch of processes for the object.
 * @return the processes
 */
mapping query_processes() {
   return _processes;
}

/**
 * This method returns the list of processes without any case issues.
 * @return the list of processes without any case
 */
mapping query_processes_nocase() {
  mapping lc_processes = ([]);
  filter( this_object()->query_processes(),
                 (: $3 += ([ lower_case( $1 ) : $2 ]) :), lc_processes );
  return lc_processes;
}

/**
 * This method adds a process into the current process list.
 * @param name the name of the process to add
 * @param ob the object the process is on
 * @return 1 on success, 0 on failure
 */
int add_process(string name, string ob) {
   _processes[name] = ob;
   return 1;
}

/**
 * This method removes the spell of the given name.
 * @param name the name of the spell to remove
 * @return 1 on succes, 0 on failure
 */
int remove_process(string name) {
  string process, name_lc;

  name_lc = lower_case(name);
  foreach (process in keys(_processes)) {
     if (lower_case(process) == name_lc) {
        map_delete(_processes, process);
     }
  }
  return 1;
}

/**
 * This method finds the data for the spell of the specific name
 * @param word the name of the spell to find data on
 * @return the data for the spell
 */
string query_process( string word ) {
   return _processes[word];
} /* query_spell() */

/**
 * This method resets all of the process data.
 */
protected void reset_processes() {
   _processes = ([ ]);
} /* reset_processes() */
