/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: controlled_monster.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: controlled_monster.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

int turn_chance;
object my_owner;
object my_enemy;
string leave_msg;
string enter_msg;   
int summoned;

int set_enemy( object enemy );

object query_owner() { return my_owner; }
object query_enemy() { return my_enemy; }
void set_leave_msg( string mess ) { leave_msg = mess; }
void set_enter_msg( string mess ) { enter_msg = mess; }
void set_summoned( int summ ) { summoned = summ; }

void init() {
   player->init();
   if( this_player() != my_owner )
      return;
   this_player()->add_command( "help", this_object(), "<direct:living:here>" );
   this_player()->add_command( "order", this_object(), 
                              "<direct:living:here> to {kill|attack} <indirect:living:here>" );
   this_player()->add_command( "order", this_object(), 
                              "<direct:living:here> to protect <indirect:living:here>" );
   this_player()->add_command( "order", this_object(), 
                              "<direct:living:here> to leave" );
}

int do_order( object *indir, string indir_match, string dir_match, 
             mixed *args, string pattern ) {
   if( this_player() != my_owner )
      return 0;
   if( pattern == "<direct:living:here> to leave" ) {
      my_owner->add_succeeded_mess( this_object(), 
                                   "$D smiles happily.\n", ({ }) );
      tell_object( my_owner,
                  (string)this_object()->the_short() +
		   " tells you: Thank you.\n");
      if ( summoned ) {
         call_out( "go_away", 1 );
      } else {
         my_owner = 0;
         call_out( "run_away", 1 );
      }
   } else if ( pattern == "<direct:living:here> to protect <indirect:living:here>" ) {
      if ( !indir ) {
         my_owner->add_failed_mess( this_object(), 
                                   "$D asks you: Who do you want me to protect?\n", ({ }) );
         return 0;
      }
      tell_object( indir[ 0 ], (string)this_object()->the_short() +" tells you:  "+ 
                  (string)my_owner->the_short() 
                  +" has asked me to protect you.\n" );
      tell_object( my_owner, (string)this_object()->the_short() 
                  +" tells you: Fine.\n" );
      this_player()->add_succeeded_mess( this_object(), 
                                        "$N $V $D to protect $I.\n ", ({ indir[ 0 ] }) );
      this_object()->do_command( "protect "+
				 (string)indir[ 0 ]->query_name() );
   } else {
      if ( !indir ) {
         my_owner->add_failed_mess( this_object(), 
                                   "$D asks you: Who do you want me to kill?\n", ({ }) );
         return 0;
      }
      if ( !set_enemy( indir[ 0 ] ) ) {
         my_owner->add_failed_mess( this_object(), 
                                   "$D tells you: There is no way you can force me to attack $I.\n", 
                                   ({ indir[ 0 ] }));
         return 0;
      };
      tell_object( my_enemy, (string)this_object()->the_short() +
		   " tells you:  "+ 
                  (string)my_owner->the_short() 
                  +" has asked me to kill you.\nPrepare to die!\n" );
      tell_object( my_owner, this_object()->the_short() +
		   " tells you: Fine.\n" );
   }
   return 1;
}

int do_help( object *indir, string indir_match, string dir_match, 
            mixed *args, string pattern ) {
   if ( this_player() != my_owner )
     return 0;
   this_player()->add_succeeded_mess( this_object(), 
                                     "You can \"order\" "+
				      this_object()->the_short() 
                                     +" to kill someone or to leave.\n ", ({ }) );
   return 1;
}

int set_enemy( object enemy ) {
   object *list;
   int i;
   
   my_enemy = enemy;
   
   if( my_enemy == my_owner ) {
      my_owner = 0;  /******* stop attacking owners foes ******/
      
      list = this_object()->query_attacker_list();
      for( i = 0; i < sizeof(list); i++ ) {
         this_object()->stop_fight( list[i] );
         (list[ i ])->stop_fight( this_object() );
         if ( !pk_check( list[ i ], my_enemy ) ) {
            (list[ i ])->attack_ob( my_enemy );
            my_enemy->attack_ob( list[ i ] );
            tell_object( list[ i ], (string)this_object()->the_short()
                        +" tells you: I will help you fight "+
                        (string)my_enemy->the_short() +".\n");
         }
      }
   } else {
      if ( pk_check( my_owner, my_enemy ) ) {
         if ( turn_chance )
           call_out( "set_enemy", 1, my_owner );
         else 
           call_out( "go_away", 1);
         return 0;
      }
   }   
   this_object()->attack_ob( my_enemy );
   call_out( "move_me", 1 );
   return 1;
}  /* set_enemy() */

int set_owner( object owner, int time, int turn) {
   if ( !owner ) return 0;
   my_owner = owner;
   turn_chance = turn;
   if ( summoned )
     call_out( "do_leave", time ) ;
   call_out( "move_me", 1 );
   return 1;
} /* set_owner() */

void event_dest_me( object ob ) {
   if ( ob == my_owner ) 
     call_out( "go_away", 0 );
   if ( ob == my_enemy ) {
      if ( !my_owner ) 
        call_out( "go_away", 0 );
      if ( my_enemy )
        tell_object( my_owner, (string)my_enemy->the_short() +" has left DiscWorld\n" );
      my_enemy = 0;
      call_out( "move_me",0);
   }
} /* event_dest_me() */
 
void event_death( object ob ) {
   if ( ob == my_enemy ) {
      if ( !my_owner ) call_out( "go_away", 0 );
      /* call_out to preven the monster from bragging 
       * before the death message */
      call_out( "do_tell", 0, my_owner, this_object()->query_short() +" tells you: "+
               my_enemy->the_short() +" has been killed!\n");
      my_enemy = 0;
      call_out( "move_me", 0 );
   }
} /* event_death() */

void do_tell( object ob, string str ) {
   tell_object( ob, str );
} /* do_tell() */

/*********************** Follow owner or enemy **********/
void move_me() {
   if ( my_enemy ) {
      if ( environment() == environment( my_enemy ) ) return;
      player->move( environment( my_enemy ), enter_msg, leave_msg );      
      tell_object( my_enemy, this_object()->the_short() +" says: You cannot escape me!\n" ); 
      return;
   }
   if ( my_owner ) {
      if ( environment() == environment( my_owner ) ) return;
      player->move( environment( my_owner ), enter_msg, leave_msg );     
      this_object()->do_command( "protect "+ (string)my_owner->query_name() );
      this_object()->do_command( "follow "+ (string)my_owner->query_name() );      
      return;
   }
   if ( summoned ) 
     call_out( "go_away", 0 );
   else
     call_out( "run_away", 1 );
} /* move_me() */

int do_leave() {
   if ( my_owner ) {
      tell_object( my_owner, this_object()->the_short()
                  +" tells you:  It is time for me to leave.\n" );
      say( this_object()->the_short() +" smiles happily.\n" );
   }
   call_out( "go_away", 2 );
   return 1;
} /* do_leave() */

/**************************** ATTACK / COMBAT ***************/
int attack_by( object ob ) {
   if ( summoned && !my_owner ) {
      say( this_object()->the_short()
	   +" says: I will not fight without an owner!\n");
      call_out("go_away",0);
      return 1; 
   }
   if ( interactive( my_owner ) ) {
      if ( (string) ob->query_guild_ob() == "/std/guilds/priest" ) {
         tell_object( my_owner, this_object()->the_short() +" tells you: "
                     "I do not wish to kill "+ ob->the_short()+".\n");
         
         if ( !turn_chance ) {
            call_out( "go_away", 1 );
            return 1;
         }
         if ( random( 100 ) < turn_chance ) {
            set_enemy( my_owner );
            return 1;
         }
      }
      if ( random( 100 ) < ( turn_chance / 5 ) ) {
         set_enemy( my_owner );
         return 1;
      }
   }
   return player->attack_by( ob );
} /* attack_by() */

void event_exit( object ob ) {
   if( ob != my_owner && ob != my_enemy ) return;
   call_out( "move_me", 1 );
} 

void go_away() {
  write( leave_msg );
  player->move( "/room/rubbish" );
} /* dest_me() */

