/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ensmurfed.c,v 1.1 1998/01/06 04:35:35 ceres Exp $
 * $Log: ensmurfed.c,v $
 * Revision 1.1  1998/01/06 04:35:35  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

varargs string short(int dark) {
   if (player)
     return player->short(dark)+"smurf"; 
   return "Smurf curse shadow";
}

mixed query_short() { return player->query_short()+"smurf"; }
mixed query_long() { return "This is a small blue humanoid.\n"; }

/* childrens cartoon characters don't have sex */
int query_female() { return 0; }
int query_gender() { return 0; }
int query_male() { return 0; }
int query_neuter() { return 1; }
string query_gender_string() { return "smurf"; }
string query_gender_title() { return "smurf"; }
string query_objective() { return "it"; }
string query_possessive() { return "its"; }
string query_pronoun() { return "it"; }

string long(string str, int dark) {
   string s, guild_ob;
   
   s = "You see "+ query_short();
   guild_ob = player->query_guild_ob();
   if (guild_ob)
      s += " "+ (string)guild_ob->query_title( this_object() ) +".\n";
   else
      s += ".\n";
   s += "It is a small blue humanoid wearing white trousers.\n";
   s += capitalize(query_pronoun())+" "+player->health_string()+".\n";
   s += player->calc_extra_look();
   s += player->weather_extra_look();
   s += player->query_living_contents(0);
   return s;
}

string smurf( string str ) {
   string str1;
   
   if ( sscanf( str, "%s,", str ) ) str1 = ",";
   else if ( sscanf( str, "%s.", str ) ) str1 = ".";
   else if ( sscanf( str, "%s!", str ) ) str1 = "!";
   else if ( sscanf( str, "%s;", str ) ) str1 = ";";
   else str1 = "";
   if ( sscanf( str, "%sing", str ) ) str = "smurfing";
   else if ( sscanf( str, "%sd", str ) ) str = "smurfed";
   else if ( sscanf( str, "%ss", str ) ) str = "smurfs";
   else str = "smurf";
   return str + str1;
}

string mangle_speech( string str ) {
   string tmp, *bits;
   int i;

   tmp = player->mangle_speech( str );
   if( !tmp ) tmp = str;
   bits = explode(tmp, " ");
   for (i=1;i<sizeof(bits);i+=3) {
      bits[i] = smurf( bits[i] );
   }
   tmp = implode(bits, " ");
   return tmp;
} /* mangle_speech() */
