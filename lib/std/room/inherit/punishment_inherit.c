/**
 * This is the basic inheritable punishment class.
 * @author Pinkfish
 * @started Tue Sep 19 14:37:28 PDT 2000
 */
#define NOMIC_SYSTEM_CASE_CLASSES
#include <nomic_system.h>
#include <nroff.h>

private nosave string _short;
private nosave string _mail_information;
private nosave string _desc_file;

void create() {
   _mail_information = "";
   this_object()->setup();
} /* create() */

/**
 * This method sets the base short description of the punishment.
 * @param name the base short description
 */
void set_short(string short) {
   _short = short;
} /* set_short() */

/**
 * This method sets the description file for this type of punishment.  What it
 * does and so on.
 * @param str the description
 */
void set_description_file(string str) {
   _desc_file = str;
} /* set_description() */

/** 
 * This method returns the description file for the punishment.
 * @return the description file
 */
string query_description_file() {
   return _desc_file;
} /* query_description_file() */

/**
 * This method sets the mail information to use when sending out a mail for
 * this punishment.
 * @param info the mail information
 */
void set_mail_information(string mail) {
   _mail_information = mail;
} /* set_mail_information() */

/**
 * The short desscription of the punishement.
 * @param area the area the case is in
 * @param the_case the case
 * @param type the type name of the punishment
 * @param data the data associated with the type
 * @return the short description
 */
string query_short(string area, class nomic_case the_case, string type, mixed data) {
   return _short;
} /* query_short() */

/**
 * The desscription of the punishement.
 * @param area the area the case is in
 * @param type the type name of the punishment
 * @return the description
 */
string query_description(string area, string type) {
   string str;

   if (_desc_file) {
      str = unguarded( (: NROFF_HAND->cat_file(_desc_file+"_nroff", 1) :) );
      if (!str) {
         unguarded( (: NROFF_HAND->create_nroff(_desc_file, _desc_file+"_nroff") :) );
         str = unguarded( (: NROFF_HAND->cat_file(_desc_file+"_nroff", 1) :) );
      }
      if (str) {
         return str;
      }
   }
   return "Nothing particularly special.\n";
} /* query_description() */

/**
 * The start of the punishment stuff, figure out any extra data.  The
 * default result calls the function back with a 0 data arguement.
 * @param area the area the case is in
 * @param the_case the case
 * @param type the type name of the punishment
 * @param finish_func the function to call when we are finished
 */
void start_punishment(string area, class nomic_case the_case, string type, function finish) {
   evaluate(finish, the_case, type, 0);
} /* start_punishment() */

/**
 * Complete the punishment, this actually does anythign that really needs to
 * be done to do the real work.
 * @param area the area the case is in
 * @param the_case the case to work on
 * @param type the type of the punishement
 * @param data any extra data to use
 */
void complete_punishement(string area, class nomic_case the_case, string type, mixed data) {
   return ;
} /* complete_punishment() */

/**
 * This method suspends the punishment.
 * @param area the area the case is in
 * @param the_case the case to work on
 * @param type the type of the punishment
 * @param data the data associated with the punishment
 */
void suspend_punishment(string area, class nomic_case the_case, string type, mixed data) {
   return ;
} /* suspend_punishment() */

/**
 * This method is used to print extra data in the mail message which
 * gets sent.  This should tell the person how to go about dealing with
 * whatever has happened to them.
 * @param area the area the case is in
 * @param the_case the case to work on
 * @param type the type of the punishment
 * @param data the data associated with the punishment
 */
string query_mail_information(string area, class nomic_case the_case,
                              string type, mixed data) {
   return _mail_information;
} /* query_mail_information() */

