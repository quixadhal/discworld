/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: potion_shop.c,v 1.6 2000/06/23 11:44:25 terano Exp $
 *
 *
 */

/**
 * @main 
 * This room is a craft shop which handles potions and salves.
 * Do not forget important things like set_save_dir() and
 * set_theft_handler().
 * @index potion_shop
 * @started Thu Oct 29 1998
 * @author Gruper
 * @see help::craft_shop
 */


#define CONVERT "/global/events"->convert_message
#define MIN_VOLUME 1000

inherit "/std/shops/craft_shop_category";

/* Function prototypes */
void shopkeeper_say( string message );
int is_potion_or_salve( object ob );
void set_message_function( function wossname );


/* This is the function that handles messages to the player. */
private nosave function _message_function;

/* If shopkeeper is set, the messages from message_function
 * will come from the shopkeeper. */
private nosave string _shopkeeper;


/**
 * @ignore
 */
void create() {
   set_message_function( (: shopkeeper_say :) );
   ::create();
   set_allowed_to_sell( (: is_potion_or_salve :) );
}

/**
 * This method sets the message function for the shop.
 * The message function can be useful to handle messages to
 * the player via the shopkeeper. The function should be of
 * type string -> void.  If message_function is not set, we
 * default to a rather nice function.  If message_function
 * is explicitly set to 0, no message will be given apart from
 * the standard messages from craft_shop.  It is called from
 * is_potion_or_salve() and can also be used in do_buy() etc.
 * @param wossname A pointer to the function
 * @example set_message_function( (: nudity_say :) ),
 * where nudity_say looks like void nudity_say( string message ).
 * When using
 */
void set_message_function( function wossname ) {
   _message_function = wossname;
}

/**
 * @return Pointer to the message function
 */
function query_message_function() {
   return _message_function;
}


/**
 * This method sets the <b> name </b> of the shopkeeper that
 * the shop will look for when giving messages to the player.
 * @param name A name that identifies the shopkeeper to find_match
 */
void set_shopkeeper( string name ) {
   _shopkeeper = name;
}

/**
 * @return The name of the shopkeeper
 */
string query_shopkeeper() {
   return _shopkeeper;
}


/**
 * This is the default message function.
 * If the shopkeeper is in the shop, they will try to say message.
 */
void shopkeeper_say( string message ) {
   object *frog;  /* the shopkeeper */

   if( !message || message == "" )
      message = "Tuppence a bucket, well stamped down!";

   if( _shopkeeper ) {
      frog = match_objects_for_existence( _shopkeeper, this_object() );
      if( sizeof( frog ) == 1 ) {
         frog[0]->init_command( "' "+ message );
         /* Ok, we managed to give the message through the shopkeeper,
          * so we'll just bugger off. */
         return;
      }
   }

   /* We couldn't find a shopkeeper, so we just write the message.
    * Boring, if you ask me. */
   write( message );
   return;
}


/**
 * This is the default function for checking if something can
 * be sold in a potion shop.  If you override this, there is no
 * reason to inherit this file, so you can't.  It will accept any
 * object that contains something which has an eat effect or an
 * apply effect.
 * @param ob The object that someone is trying to sell to the shop
 * @return success 1 if the object can be sold, 0 if not
 */
int is_potion_or_salve( object ob ) {
   object thing, *contents;
   int found_something; // Impure stuff!
   string contents_string;

   if( !ob->query_volume() ) {
      tell_creator( this_player(), "ob has 0 volume.\n" );
      if( _message_function )
         evaluate( _message_function,
               CONVERT( "There's nothing in "+ ob->the_short() +" that "
               "can be described as either potion or salve, "+
               this_player()->the_short() +".\n" ) );
      return 0;
   } else if( ob->query_volume() < MIN_VOLUME ||
              ob->query_volume() < ( 7 * ob->query_max_volume() / 10 ) ) {
      tell_creator( this_player(), "ob does not contain enough stuff.\n" );
      if( _message_function )
         evaluate( _message_function,
               CONVERT( "We don't deal in quantities smaller than a cup, "+
               this_player()->the_short() +".\n" ) );
      return 0;
   }

   contents = all_inventory( ob );
   if( !sizeof( contents ) ) {
      tell_creator( this_player(), "No contents.\n" );
      if( _message_function )
         evaluate( _message_function,
               CONVERT( ob->the_short() +" is empty, "+
               this_player()->the_short() +"!\n" ) );
      return 0;
   }

   foreach( thing in contents )
      if( !( sizeof( thing->query_apply_effects() ) +
             sizeof( thing->query_eat_effects() ) ) ) {
         found_something = 1; // Impure
      } else {
         tell_creator( this_player(), "thing %s, effect %O.\n",
                       thing->query_short(), thing->query_eat_effects()  );
      }
   
   contents_string = ( sizeof( contents ) > 1 ) ?
         query_multiple_short( contents, "the" ) +" are neither potions nor "
            "salves, or they are " :
         contents[0]->the_short() +" is neither potion nor salve, or it is ";

   // Found impure stuff
   if( found_something ) {
      if( _message_function )
         evaluate( _message_function,
               CONVERT( "Either "+ contents_string +"impure, or so weak as "
               "to make no difference, "+ this_player()->the_short() +"." ) );
      return 0;
   }

   return 1;
}
