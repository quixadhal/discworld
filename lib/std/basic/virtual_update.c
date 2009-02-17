/**
 * This is the inherit used to update virtual objects from their base files
 * if something changes.  This will call the method init_static_arg with
 * any updated data.
 * @author Dragonkin
 * @changes Pinkfish Fri Apr  6 13:53:28 PDT 2001
 * Turned into an inherit
 */
#include <virtual.h>

mixed query_property(string name);
void add_property(string name, mixed prop);
void init_static_arg(mapping data);

/**
 * This method turns the current object into a missing item
 * receipt.
 */
void replace_me()
{
   object receipt;
   mixed als;

   receipt = clone_object("/obj/misc/al_receipt");
   receipt->set_name("receipt");
   receipt->set_object(file_name(this_object()));
   receipt->set_virtobname(query_property(VIRTUAL_NAME_PROP));
   receipt->set_value(this_object()->query_value());
   als = "/global/auto_load"->fragile_auto_str_ob(this_object());
   als[0] = 1;
   receipt->set_static_save(als);
   receipt->set_short("destructed item receipt");
   receipt->add_adjective(({ "destructed", "item" }));
   receipt->set_long("This seems to be a small piece of paper.\n");
   receipt->set_read_mess("According to our sources, your " +
                          this_object()->query_short() +
                          " was not " "allowed to exist.  Have a nice day.");
   receipt->move(environment());
   receipt->set_weight(1);
   destruct(this_object());
}

/**
 * THis method should be called in the inheritable to deal with virtual
 * objects.  It will check to see if the data has changed and update the
 * object to reflect any of the changes.
 */
void init_virtual_object()
{
   mapping stat_temp;
   string virt_name;
   string new_name;

   virt_name = query_property(VIRTUAL_NAME_PROP);
   if (virt_name) {
      if (file_size(virt_name) == -1) {
         new_name = (CLONER)->other_file(virt_name);
         if (stringp(new_name) && (new_name != virt_name)) {
            add_property(VIRTUAL_NAME_PROP, new_name);
            virt_name = new_name;
         } else {
            if (VIRTUAL_HANDLER->query_forbidden(virt_name)) {
               call_out("replace_me", 1);
            } else {
               VIRTUAL_HANDLER->add_missing(virt_name);
            }
         }
      }
      if (file_size(virt_name) != -1 &&
          query_property("virtual time") < stat(virt_name)[1]) {
         stat_temp = (VIRTUAL_HANDLER)->new_data(virt_name);
         if (mapp(stat_temp)) {
            init_static_arg(stat_temp);
            add_property("virtual time", time());
         }
      }
   }

}                               /* init_virtual_object() */
