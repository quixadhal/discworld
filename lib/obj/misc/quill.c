/*
 * Ink/dipping can be added once vessels/liquids is finished.
 * Writing size can be added once that's worked out.
 *
 * Added 'copy' command - Jeremy, 10-Dec-95
 */

#include <language.h>

inherit "/obj/weapon";

int do_scribble( object *things, string mess);

object writing_on;
string type;

void setup() {
   set_name( "quill" );
   set_long( "This is a nice, long, feathery quill.  You could \"write\" "
         "on things with it.\n");
   set_weight( 3 );
   set_value( 1000 );
   new_weapon( 240 );
   add_help_file("quill");
   set_damage_chance( 100 );
   add_attack( "stab", 90, ({ 10, 2, 10 }), "pierce", "sharp", 0 );
   add_property( "paper writer", 1 );
   type = "ink";
} /* setup() */

void init() {
   add_command( "write", "on <indirect:object> with <direct:object>" );
   add_command( "write", "with <direct:object> on <indirect:object>" );
   add_command( "sign", "<indirect:object> with <direct:object>");
   add_command( "scribble",
                "<string'message'> on <indirect:object> with <direct:object>",
                (: do_scribble($1, $4[0]) :));
/*
   this_player()->add_command( "copy", this_object(),
                              ({"<indirect:object> 'to' <indirect:object> 'with' <direct:object>",
                                "<indirect:object> 'to' <indirect:object> 'with' <direct:object> 'in' <word>"}));
 */
} /* init() */

int do_scribble( object *things, string mess) {
   string language;

   if ( query_wielded() != this_player() ) {
      this_player()->add_failed_mess( this_object(), "You need to be "+
            "holding $D to $V with it.\n", ({ }) );
      return 0;
   }
   things = filter(things, (: is_in_me_or_environment($1, this_player()) :));
   if (!sizeof(things)) {
      add_failed_mess("You can only $V on objects you are holding or are "
                      "in the room with $D.\n");
      return 0;
   }

   if ( sizeof( things ) > 1 ) {
      add_failed_mess("You can only $V on one object at once with $D.\n");
      return 0;
   }
   if ( !things[ 0 ]->query_property( "writeable" ) ) {
      add_failed_mess("You cannot $V on $I with $D.\n", things );
      return 0;
   }
   language = (string)this_player()->query_current_language();
   if ( !LANGUAGE_HAND->query_language_written( language ) ) {
      add_failed_mess("You cannot $V on "+
            "$I with $D when you're not using a language that can be "+
            "written.\n", things );
      return 0;
   }

   if (things[0]->is_current_page_torn_out()) {
      add_failed_mess( "The page of " + things[0]->the_short() +
            " you were writing on appears to have been torn out.\n" );
      return 0;
   }

   things[0]->add_read_mess( mess, type, language, 0 );
   add_succeeded_mess("$N $V something on $I.\n", things);
   return 1;
} /* do_scribble() */

int do_write( object *things ) {
   string language;

   if ( query_wielded() != this_player() ) {
      this_player()->add_failed_mess( this_object(), "You need to be "+
            "holding $D to $V with it.\n", ({ }) );
      return 0;
   }
   things = filter(things, (: is_in_me_or_environment($1, this_player()) :));
   if (!sizeof(things)) {
      add_failed_mess("You can only $V on objects you are holding or are "
                      "in the room with $D.\n");
      return 0;
   }

   if ( sizeof( things ) > 1 ) {
      add_failed_mess("You can only $V on one object at once with $D.\n");
      return 0;
   }
   if ( !things[ 0 ]->query_property( "writeable" ) ) {
      add_failed_mess("You cannot $V on $I with $D.\n", things );
      return 0;
   }
   language = (string)this_player()->query_current_language();
   if ( !LANGUAGE_HAND->query_language_written( language ) ) {
      add_failed_mess("You cannot $V on "+
            "$I with $D when you're not using a language that can be "+
            "written.\n", things );
      return 0;
   }
   if ( writing_on ) {
      add_failed_mess("You are already writing on $I with $D.\n", ({ writing_on }) );
      return 0;
   }
   add_succeeded_mess("$N start$s writing on $I with $D.\n", things );
   writing_on = things[ 0 ];
   call_out( "begin_writing", 0, this_player() );
   return 1;
} /* do_write() */

void begin_writing( object writer ) {
   if ( !writer ) {
      writing_on = 0;
      return;
   }
   writer->do_edit( 0, "end_writing" );
} /* begin_writing() */

void end_writing( string words ) {
   string language;

   if ( !words || ( words == "" ) ) {
      write( "You don't write anything on "+
            (string)writing_on->the_short() +".\n" );
      say( (string)this_player()->the_short() +" doesn't "+
            "write anything after all.\n" );
      writing_on = 0;
      return;
   }
   language = (string)this_player()->query_current_language();
   if ( !LANGUAGE_HAND->query_language_written( language ) ) {
      write( "You seem to have changed to using a non-written language.\n" );
      say( (string)this_player()->the_short() +" seems "+
            "linguistically-challenged.\n" );
      writing_on = 0;
      return;
   }
   if ( !writing_on ) {
      write( "You seem to have lost what you were writing on.\n" );
      say( (string)this_player()->the_short() +" looks around "+
            "in puzzlement.\n" );
      return;
   }
   if ( member_array( environment( writing_on ), ({ environment(),
         environment( this_player() ) }) ) == -1 ) {
      write( (string)writing_on->the_short() +
            " seems to have run away from you.\n" );
      say( (string)this_player()->the_short() +" looks like "+
            (string)this_player()->query_pronoun() +"'s lost something.\n" );
      writing_on = 0;
      return;
   }
   if (writing_on->is_current_page_torn_out()) {
      write( "The page of " + (string)writing_on->the_short() +
            " you were writing on appears to have been torn out.\n" );
      say( (string)this_player()->the_short() +" looks like "+
            (string)this_player()->query_pronoun() +"'s lost something.\n" );
      writing_on = 0;
      return;
   }

   writing_on->add_read_mess( words, type, language, 0 );
   write( "You finish writing on "+
         (string)writing_on->the_short() +".\n" );
   say( (string)this_player()->the_short() +" finishes writing "+
         "on "+ (string)writing_on->a_short() +".\n" );
   writing_on = 0;
} /* end_writing() */

int do_sign(object* things) {
   object ob;
   string language;

   if ( query_wielded() != this_player() ) {
      add_failed_mess( "You need to be "+
            "holding $D to $V with it.\n", ({ }) );
      return 0;
   }
   things = filter(things, (: is_in_me_or_environment($1, this_player()) :));
   if (!sizeof(things)) {
      add_failed_mess("You can only $V on objects you are holding or are "
                      "in the room with $D.\n");
      return 0;
   }

   if ( sizeof( things ) > 1 ) {
      add_failed_mess("You can only $V on one object at once with $D.\n");
      return 0;
   }
   if ( !things[ 0 ]->query_property( "writeable" ) ) {
      add_failed_mess("You cannot $V on $I with $D.\n", things );
      return 0;
   }
   
   language = (string)this_player()->query_current_language();
   if ( !LANGUAGE_HAND->query_language_written( language ) ) {
      add_failed_mess("You cannot $V $I with $D when you're not "
            "using a language that can be written.\n", things );
      return 0;
   }

   things[ 0 ]->add_read_mess(this_player()->short(0, 1), "cursive, "
        "signed by", language, 0);
   add_succeeded_mess("$N $V $I with $D.\n", things);
   return 1;
} /* do_sign() */

int do_copy( mixed *in_dir, string direct, string indirect, mixed *args ) {
    // This will need a lot of checks.  For now it is just a "proof of
    // concept".
    mixed src_mess, mess;
    string lang, cur_lang;
    int i, c, siz, perr;

    //printf("-------- in_dir\n%O\n------- args\n%O\n", in_dir, args);
    // This is to keep the parser from matching "quill in <word>" on
    // a second attempt when the language checks fail.  Maybe this won't
    // be necessary some day.
    if ( !present(direct, this_player()) ) return 0;
    if ( query_wielded() != this_player() ) {
        this_player()->add_failed_mess( this_object(), "You need to be "
                                       "holding $D to $V with it.\n", ({ }) );
        return 0;
    }
    if ((sizeof(in_dir) != 2) ||
        (sizeof(in_dir[0]) != 1) || (sizeof(in_dir[1]) != 1)) {
        //write("You can only copy to/from one object at a time.\n");
        this_player()->add_failed_mess(this_object(),
                                       "You can only $V to/from one object "
                                       "at a time.\n", ({ }));
        return 0;
    }
    if ( !in_dir[1][0]->query_property( "writeable" ) ) {
        this_player()->add_failed_mess( this_object(), "You can't write on "
                                       "$I.\n", in_dir[1] );
        return 0;
    }
    if ( sizeof(args) == 4 ) {
        if ( !LANGUAGE_HAND->query_language_written(args[3]) ) {
            this_player()->add_failed_mess(this_object(),
                               args[3] + " is not a written language.\n",
                               ({ }));
            return 0;
        }
        if ( LANGUAGE_HAND->query_language_magic(args[3])) {
            this_player()->add_failed_mess(this_object(),
                               "You can't write " + args[3] + " like this.\n",
                               ({ }));
            return 0;
        }
        if ( !this_player()->query_language(args[3]) ) {
            this_player()->add_failed_mess(this_object(),
                               "You don't know " + args[3] + ".\n", ({ }));
            return 0;
        }
        cur_lang = args[3];
    }
    src_mess = in_dir[0][0]->query_read_mess();
    if (!sizeof(src_mess)) {
        write("There is nothing written on " + in_dir[0][0]->the_short() +
              ".\n");
        this_player()->add_failed_mess(this_object(),
                                       "There is nothing written on $I.\n",
                                       in_dir[0]);
        return 0;
    }
    // Copy each read_mess
    for (i = 0; i < sizeof(src_mess); i++) {
        mess = src_mess[i][0];
        lang = src_mess[i][2];
        siz = src_mess[i][3];
        if (LANGUAGE_HAND->query_language_magic(lang)) {
            mess = "A series of unintelligible scribbles.";
            lang = "common";
        } else if (!this_player()->query_language(lang) && stringp(mess)) {
            // Introduce a few copying errors, based on intelligence
            // and dexterity
            perr = this_player()->query_int() + this_player()->query_dex()
              - 20;
            for (c = random(perr); c < sizeof(mess); c += random(perr)+1) {
                if ((mess[c] >= 'a') && (mess[c] <= 'z')) {
                    mess[c] = 'a' + random(26);
                } else if ((mess[c] >= 'A') && (mess[c] <= 'Z')) {
                    mess[c] = 'A' + random(26);
                }
            }
        } else if ( stringp(cur_lang) ) {
            lang = cur_lang;
        }
        //printf("%O\n", src_mess[i]);
        //printf("--------\n%O\n", in_dir);
        in_dir[1][0]->add_read_mess(mess, type, lang, siz);
    }
    this_player()->add_succeeded_mess(this_object(),
                                      "$N $V the writing on $I with $D.\n",
                                      in_dir[0]);
    return 1;
}
