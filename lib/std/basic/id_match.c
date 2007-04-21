/**
 * This inheritable contains all the code to deal with matching objects in
 * the parser.
 * @author Pinkfish.
 */

#include <obj_parser.h>

private int _no_all;

/**
 * This sets the no all flag.  If this flag is set to true then the object
 * will not respond to 'all' requests and must be accessed with it's real
 * name.
 * @param no_all the no all flag value
 */
protected void set_no_all(int no_all) {
   _no_all = no_all;
} /* set_no_all() */

/**
 * This method returns the no all flag's current value.
 * If this flag is set to true then the object
 * will not respond to 'all' requests and must be accessed with it's real
 * name.
 * @return the current value of the flag
 */
int query_no_all() {
   return _no_all;
} /* query_no_all() */

/**
 * This method is used by upper objects that only wish to match the
 * ids and not mess with all the context stuff.  This method will
 * return a combination of OBJ_PARSER_MATCH_SINGULAR and
 * OBJ_PARSER_MATCH_PLURAL.
 * @param input the input string
 * @param viewer the person viewing the object
 * @param context the contex this is to be taken in
 * @return the singular/plural match flags
 */
protected int is_matching_object(string* input,
                          object viewer,
                          class obj_match_context context )
{
   string *adj;
   string *padj;
   string *names;
   string *pnames;
   int n;
   int ret;

   /* context */
   if( this_object() == context->him &&
       input[<1] == "him" ) {
      ret |= OBJ_PARSER_MATCH_SINGULAR;
   } else if( this_object() == context->her &&
              input[<1] == "her" ) {
      ret |= OBJ_PARSER_MATCH_SINGULAR;
   } else if( this_object() == context->it &&
              input[<1] == "it" ) {
      ret |= OBJ_PARSER_MATCH_SINGULAR;
   } else if( context->plural &&
              input[<1] == "them" &&
              member_array( this_object(), context->plural ) != -1 ) {
      ret |= OBJ_PARSER_MATCH_PLURAL;
   } else if( this_object() == viewer &&
              input[<1] == "me" ) {
      ret |= OBJ_PARSER_MATCH_SINGULAR;
   } else if (!_no_all && input[<1] == "all") {
      ret |= OBJ_PARSER_MATCH_PLURAL;
   } else if (!_no_all && (input[<1] == "things" || input[<1] == "ones")) {
      ret |= OBJ_PARSER_MATCH_PLURAL;
   } else if (!_no_all && (input[<1] == "thing" || input[<1] == "one")) {
      ret |= OBJ_PARSER_MATCH_SINGULAR;
   }

   if (!ret) {
      //
      // Strip off the here if it is the right place for it.
      //
      if (input[<1] == "here" &&
          viewer != environment() &&
          sizeof(input) > 1) {
         input = input[0..<2];
      }
   
      //
      // CHeck to see if we match the noun first.
      //
      names = this_object()->parse_command_id_list();
      pnames = this_object()->parse_command_plural_id_list();

      if( member_array( input[<1], pnames ) != -1 ) {
         ret |= OBJ_PARSER_MATCH_PLURAL;
      }
      if( member_array( input[<1], names ) != -1 ) {
         ret |= OBJ_PARSER_MATCH_SINGULAR;
      }

      if (!ret) {
         return 0;
      }
   }

   //
   // Check and make sure we match all the adjectives.
   //
   adj = this_object()->parse_command_adjectiv_id_list();
   padj = this_object()->parse_command_plural_adjectiv_id_list();
   if( environment() == context->him ) {
      adj += ({ "his" });
   } else if( environment() == context->her ) {
      adj += ({ "her" });
   } else if( environment() == context->it ) {
      adj += ({ "its" });
   } else if( environment() == viewer ) {
      adj += ({ "my" });
   } else if( context->plural &&
                   member_array( environment(), context->plural ) != -1 ) {
      adj += ({ "their" });
   }
   
   for( n = 0; n < sizeof( input ) - 1; n++ )
   {
      if( member_array( input[n], adj ) == -1 ) {
         if (!padj || member_array(input[n], padj) == -1) {
            return 0;
         } else {
            ret &= ~OBJ_PARSER_MATCH_SINGULAR;
            ret |= OBJ_PARSER_MATCH_PLURAL;
         }
      }
   }

   return ret;
} /* is_matching_object() */

/**
 * This method updates the context.  It returns the number of the objects
 * we match, and you pass in the number of objects that we are.
 * @param context the context to update
 * @param num the number of objects
 * @param singular return from is_matching_object
 * @return the number of matched objects
 */
protected int update_parse_match_context(class obj_match_context context,
                                         int num,
                                         int singular) {
   if (context->ordinal) {
      if (context->ordinal == -1) {
         return 1;
      }
      if (context->ordinal > num) {
         context->ordinal -= num;
         return 0;
      }
      context->ignore_rest = 1;
      return 1;
   } else if (context->number_included) {
      if (context->number_included <= num) {
         context->ignore_rest = 1;
         num = context->number_included;
         context->number_included = 0;
         return num;
      }
      context->number_included -= num;
      return num;
   } else {
      //
      // Just a singular reference.
      //
      if (num > 0 && (singular & OBJ_PARSER_MATCH_SINGULAR)) {
         return 1;
      }

      //
      // All state.
      //
      return num;
   }
} /* update_parse_match_context() */

/**
 * This code is for the new parser...  It returns an array which contains
 * two elements.  The first is the type of the match, plural or singular,
 * and the second is the object it matched.  Usualy this_object() but
 * it can be used to swap new objects.
 * @param input the input string
 * @param viewer the person viewing the object
 * @param context the contex this is to be taken in
 * @return ({ type, ob }), 0 on failure
 */
mixed parse_match_object( string* input,
                          object viewer,
                          class obj_match_context context )
{
   int ret;

   ret = is_matching_object(input, viewer, context);
   if (!ret) {
      return 0;
   }

   //
   // We matched, so see if we should have.
   //
   if (!update_parse_match_context(context, 1, ret)) {
      return 0;
   }

   return ({ ret, ({ this_object() }) });
} /* parse_match_object() */
