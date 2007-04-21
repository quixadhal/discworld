/**
 * This inheritable will handle all the generic stuff for commands that
 * need a response.  Like 'teach' and 'offer'.
 * @author Pinkfish
 * @started Mon Aug 17 04:13:10 EDT 1998
 */
private nosave mapping _respond_cmd;

class respond_data {
   /**
    * The person offering us the thingy.
    */
   object ob;
   mixed data;
}

void remove_respond_command(string type, object ob);

/** @ignore yes */
mapping query_respond_cmd_data() { return _respond_cmd; }

void create() {
   _respond_cmd = ([ ]);
} /* create() */

/**
 * This method adds a responable command set of values to the
 * living object.
 * @param type the type of thing to set as a response
 * @param ob the object which is giving us the thing
 * @param data the data associated with the command
 */
void add_respond_command(string type, object ob, mixed data) {
   class respond_data frog;

   // Make sure we only have one of them.
   remove_respond_command(type, ob);
   frog = new (class respond_data);
   frog->ob = ob;
   frog->data = data;
   if (!_respond_cmd[type]) {
      _respond_cmd[type] = ({ });
   }
   _respond_cmd[type] += ({ frog });
} /* add_respond_commnad() */

/**
 * This method returns the data associated with specified type
 * and object for a respondable command.
 * @param type the type of the respondable
 * @param ob the object we are checking against
 * @return the data associated with the response, 0 if not found
 */
mixed query_respond_command(string type, object ob) {
   class respond_data frog;

   if (_respond_cmd[type]) {
      foreach (frog in _respond_cmd[type]) {
         if (frog->ob == ob) {
            return frog->data;
         }
      }
   }
   return 0;
} /* query_respond_command() */

/**
 * This method removes the response for this command.
 * @param type the type of the responable
 * @param ob the object that we are removing a responable for
 */
void remove_respond_command(string type, object ob) {
   class respond_data frog;
   int i;

   if (_respond_cmd[type]) {
      for (i = 0; i < sizeof(_respond_cmd[type]); i++) {
         frog = _respond_cmd[type][i];
         if (frog->ob == ob) {
            _respond_cmd[type] = _respond_cmd[type][0..i - 1] +
                                 _respond_cmd[type][i+1..];
            i--;
         }
      }
   }
} /* remove_respond_command() */

/** @ignore yes */
/* Added to allow manual clearing of this stuff when it screws up */
void clear_respond_commands() {
   _respond_cmd = ([ ]);
}
