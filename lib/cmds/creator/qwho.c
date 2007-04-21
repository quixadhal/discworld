/**
 * Playerdist
 * Shows the geographical distribution of players online
 *
 * @author Dasquian
 * @started 6/10/02
 * @changed 29/04/03 pinkfish
 * Updated to be in the main directory and using 'qwho'.
 **/

#include <top_ten_tables.h>

int cmd(string filter, string arg);

string* determine_location( object person ) {
    mixed *areas, *place;
    
    areas = ({
    
        ({ "/d/liaison/NEWBIE", ({ "Misc", "The newbie area" }) }),
        ({ "/room/air", ({ "Misc", "The air" }) }),
        ({ "/room/void", ({ "Misc", "The void" }) }),
        ({ "/room/rubbish", ({ "Misc", "The rubbish room" }) }),
        ({ "/room/departures", ({ "Misc", "The departures lounge" }) }),
        ({ "/room/jail", ({ "Misc", "Jail" }) }),
        ({ "/w/", ({ "Misc", "/w dirs" }) }),

        ({ "/d/cwc/utils/interview", ({ "CWC", "Interview room" }) }),
        ({ "/d/cwc/common", ({ "CWC", "Commonroom" }) }),
        ({ "/d/cwc/Brown_Islands", ({ "CWC", "Brown Islands" }) }),
        ({ "/d/cwc/Bes_Pelargic/Bad_Poo-Ning/roads/medina",
            ({ "CWC", "Medina" }) }),
        ({ "/d/cwc/Bes_Pelargic/Sum_Bing/roads/snail",
            ({ "CWC", "The Snail" }) }),
        ({ "/d/cwc/Bes_Pelargic", ({ "CWC", "Bes Pelargic" }) }),
        ({ "/d/cwc/Hunghung", ({ "CWC", "HungHung" }) }),
        ({ "/d/cwc/Isle_of_Tonahamen", ({ "CWC", "Isle of Tonahamen" }) }),
        ({ "/d/cwc/families/mcsweeny", ({ "CWC", "McSweeney Estate" }) }),
        ({ "/d/cwc/Maps", ({ "CWC", "Terrains" }) }),
        ({ "/d/cwc", ({ "CWC", "?" }) }),
        ({ "/d/guilds/assassins/Bes-Pelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/guilds/thieves/Bes-Pelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/guilds/warriors/BesPelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/guilds/warriors/Hunghung", ({ "CWC", "HH guilds" }) }),
        ({ "/d/guilds/priests/Bes_Pelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/guilds/wizards/Bes_Pelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/guilds/witches/bespelargic", ({ "CWC", "BP guilds" }) }),
        ({ "/d/underworld/moving_shop/rooms/cwc", ({ "CWC", "F-shop" }) }),

        ({ "/d/am/utils/interview", ({ "AM", "Interview room" }) }),
        ({ "/d/am/shades", ({ "AM", "The Shades" }) }),
        ({ "/d/am/common", ({ "AM", "Commonroom" }) }),
        ({ "/d/am", ({ "AM", "Ankh-Morpork" }) }),
        ({ "/d/special/player_shops", ({ "AM", "Tarnach's" }) }),
        ({ "/d/guilds/merchants", ({ "AM", "Ankh-Morpork" }) }),
        ({ "/d/guilds/assassins/Ankh-Morpork", ({ "AM", "AM guilds" }) }),
        ({ "/d/guilds/thieves/Ankh-Morpork", ({ "AM", "AM guilds" }) }),
        ({ "/d/guilds/warriors/Ankh-Morpork", ({ "AM", "AM guilds" }) }),
        ({ "/d/guilds/priests/Ankh-Morpork", ({ "AM", "AM guilds" }) }),
        ({ "/d/guilds/wizards/Ankh-Morpork", 
            ({ "AM", "Unseen University" }) }),
        ({ "/d/guilds/witches/Ankh-Morpork", ({ "AM", "AM guilds" }) }),
        ({ "/d/guilds/witches/am", ({ "AM", "AM guilds" }) }),

        ({ "/d/klatch/utils/interview", ({ "Klatch", "Interview room" }) }),
        ({ "/d/klatch/common", ({ "Klatch", "Commonroom" }) }),
        ({ "/d/klatch/djel/city", ({ "Klatch", "KLK" }) }),
        ({ "/d/klatch/djel/necropolis", ({ "Klatch", "Necropolis" }) }),
        ({ "/d/klatch/djel/netherworld", ({ "Klatch", "Netherworld" }) }),
        ({ "/d/klatch/tsort/desert/roads", 
            ({ "Klatch", "Tsort desert road" }) }),
        ({ "/d/klatch/tsort/desert", ({ "Klatch", "Tsort desert" }) }),
        ({ "/d/klatch/tsort", ({ "Klatch", "Tsort" }) }),
        ({ "/d/klatch/Maps", ({ "Klatch", "Terrains" }) }),
        ({ "/d/klatch", ({ "Klatch", "?" }) }),
        ({ "/d/guilds/assassins/Khot-lip-khin", 
            ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/thieves/Khot-lip-khin", ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/warriors/Khot-lip-khin", 
            ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/priests/Djelibeybi", ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/priests/Khot-lip-khin", ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/wizards/Khot-lip-khin", ({ "Klatch", "KLK guilds" }) }),
        ({ "/d/guilds/wizards/Ephebe", ({ "Klatch", "Ephebe guilds" }) }),
        ({ "/d/guilds/wizards/Tsort", ({ "Klatch", "Tsort guilds" }) }),
        ({ "/d/guilds/witches/klk", ({ "Klatch", "KLK guilds" }) }),

        ({ "/d/ram/utils/interview", ({ "Ram", "Interview room" }) }),
        ({ "/d/ram/common", ({ "Ram", "Commonroom" }) }),
        ({ "/d/ram/Gloomy_Forest", ({ "Ram", "Gloomy forest" }) }),
        ({ "/d/ram/Lancre_Kingdom", ({ "Ram", "Lancre Kingdom" }) }),
        ({ "/d/ram/Listening_Monks", ({ "Ram", "Listening Monks" }) }),
        ({ "/d/ram/Razorback", ({ "Ram", "Razorback" }) }),
        ({ "/d/ram/Slice", ({ "Ram", "Slice" }) }),
        ({ "/d/ram/Sprite_Caverns", ({ "Ram", "Sprite caverns" }) }),
        ({ "/d/ram/Temple_of_Soyin", ({ "Ram", "Temple of Soyin" }) }),
        ({ "/d/ram/Uberwald", ({ "Ram", "Uberwald" }) }),
        ({ "/d/ram/badass", ({ "Ram", "Bad Ass" }) }),
        ({ "/d/ram/madstoat", ({ "Ram", "Mad Stoat" }) }),
        ({ "/d/ram/ohulan", ({ "Ram", "Ohulan-Cutash" }) }),
        ({ "/d/ram/foothills", ({ "Ram", "Connecting paths" }) }),
        ({ "/d/ram/bridge", ({ "Ram", "Connecting paths" }) }),
        ({ "/d/ram/moorland", ({ "Ram", "Connecting paths" }) }),
        ({ "/d/ram/gorge_road", ({ "Ram", "Connecting paths" }) }),
        ({ "/d/ram/Lower_Valleys", ({ "Ram", "Connecting paths" }) }),
        ({ "/d/ram/lancre_town", ({ "Ram", "Lancre Town" }) }),
        ({ "/d/ram/bandit_camp", ({ "Ram", "Bandit camp" }) }),
        ({ "/d/ram/druid_circle", ({ "Ram", "Druid circle" }) }),
        ({ "/d/ram/mountains", ({ "Ram", "Ramtops" }) }),
        ({ "/d/ram", ({ "Ram", "?" }) }),
        ({ "/d/guilds/assassins/Ohulan-Cutash", ({ "Ram", "OC guilds" }) }),
        ({ "/d/guilds/thieves/Madstoat", ({ "Ram", "Mad Stoat guilds" }) }),
        ({ "/d/guilds/warriors/Lancre", ({ "Ram", "Lancre guilds" }) }),
        ({ "/d/guilds/priests/Monks-of-Cool", ({ "Ram", "Monks of Cool" }) }),
        ({ "/d/guilds/priests/Ohulan-Cutash", ({ "Ram", "OC guilds" }) }),
        ({ "/d/guilds/wizards/Creel-Springs",
            ({ "Ram", "Creel Springs guilds" }) }),
        ({ "/d/guilds/witches/badass", ({ "Ram", "Bad Ass guilds" }) }),
        ({ "/d/guilds/witches/lancre", ({ "Ram", "Lancre guilds" }) }),
        ({ "/d/guilds/witches/madstoat", ({ "Ram", "Mad Stoat guilds" }) }),
        ({ "/d/guilds/witches/razorback", ({ "Ram", "Razorback guilds" }) }),

        ({ "/d/sur/utils/interview", ({ "Sur", "Interview room" }) }),
        ({ "/d/sur/common", ({ "Sur", "Commonroom" }) }),
        ({ "/d/sur/Maps", ({ "Sur", "Terrains" }) }),
        ({ "/d/sur/Dinky", ({ "Sur", "Dinky" }) }),
        ({ "/d/sur/Sto_Helit", ({ "Sur", "Sto Helit" }) }),
        ({ "/d/sur/StoLat", ({ "Sur", "Sto Lat" }) }),
        ({ "/d/sur/Hillshire", ({ "Sur", "Hillshire" }) }),
        ({ "/d/sur/Holywood", ({ "Sur", "Holywood" }) }),
        ({ "/d/sur/Nowhere", ({ "Sur", "Nowhere" }) }),
        ({ "/d/sur/PekanFord", ({ "Sur", "Pekan Ford" }) }),
        ({ "/d/sur/Sheepridge", ({ "Sur", "Sheepridge" }) }),
        ({ "/d/sur/Scrogden", ({ "Sur", "Scrogden" }) }),
        ({ "/d/sur/items/carriage", ({ "Sur", "Carriage" }) }),
        ({ "/d/sur", ({ "Sur", "?" }) }),
        ({ "/d/guilds/assassins/Sto-Lat", ({ "Sur", "SL guilds" }) }),
        ({ "/d/guilds/assassins/Sto-Helit", ({ "Sur", "SH guilds" }) }),
        ({ "/d/guilds/assassins/Sheepridge", 
            ({ "Sur", "Sheepridge guilds" }) }),
        ({ "/d/guilds/thieves/Sto-Lat", ({ "Sur", "SL guilds" }) }),
        ({ "/d/guilds/thieves/Sto-Helit", ({ "Sur", "SH guilds" }) }),
        ({ "/d/guilds/thieves/Pekan_Ford", ({ "Sur", "PF guilds" }) }),
        ({ "/d/guilds/warriors/Sto-Lat", ({ "Sur", "SL guilds" }) }),
        ({ "/d/guilds/priests/Sto-Lat", ({ "Sur", "SL guilds" }) }),
        ({ "/d/guilds/wizards/Sto-Lat", ({ "Sur", "SL guilds" }) }),
        ({ "/d/guilds/witches/skund", ({ "Sur", "Skund guilds" }) }),
        ({ "/d/guilds/witches/stolat", ({ "Sur", "SL guilds" }) }),

        ({ "/d/forn/utils/interview", ({ "Forn", "Interview room" }) }),
        ({ "/d/forn/common", ({ "Forn", "Commonroom" }) }),
        ({ "/d/forn/genua", ({ "Forn", "Genua" }) }),
        ({ "/d/forn", ({ "Forn", "?" }) }),
        ({ "/d/guilds/assassins/Genua", ({ "Forn", "Genua guilds" }) }),

        ({ "/d/special/utils/interview", ({ "Special", "Interview room" }) }),
        ({ "/d/special/common", ({ "Special", "Commonroom" }) }),
        ({ "/d/special/ctf", ({ "Special", "CTF" }) }),
        ({ "/d/special", ({ "Special", "?" }) }),
        
        ({ "/d/playtesters/utils/interview", ({ "Playtesters", 
            "Interview room" }) }),
        ({ "/d/playtesters/common", ({ "Playtesters", "Commonroom" }) }),
        ({ "/d/playtesters/palace_rooms", ({ "Playtesters", "Palace" }) }),
        ({ "/d/playtesters", ({ "Playtesters", "?" }) }),

        ({ "/d/liaison/utils/interview", ({ "Liaison", "Interview room" }) }),
        ({ "/d/liaison/common", ({ "Liaison", "Commonroom" }) }),
        ({ "/d/liaison", ({ "Liaison", "?" }) }),
        
        ({ "/d/underworld/common", ({ "Underworld", "Commonroom" }) }),
        ({ "/d/underworld/death", ({ "Underworld", "Death's domain" }) }),
        ({ "/d/underworld/lspace", ({ "Underworld", "L-space" }) }),
        ({ "/d/underworld/moving_shop", ({ "Underworld", "T-shop" }) }),
        ({ "/d/underworld", ({ "Underworld", "?" }) }),
        
        ({ "/d/guilds/common", ({ "Guilds", "Commonroom" }) }),
        ({ "/d/guilds/assassins", ({ "Guilds", "Assassins" }) }),
        ({ "/d/guilds/thieves", ({ "Guilds", "Thieves" }) }),
        ({ "/d/guilds/warriors", ({ "Guilds", "Warriors" }) }),
        ({ "/d/guilds/priests/rooms", ({ "Guilds", "Passage rooms" }) }),
        ({ "/d/guilds/priests", ({ "Guilds", "Priests" }) }),
        ({ "/d/guilds/wizards", ({ "Guilds", "Wizards" }) }),
        ({ "/room/magic_circle", ({ "Guilds", "Witches' circle" }) }),
        ({ "/d/guilds/witches", ({ "Guilds", "Witches" }) }),
        ({ "/d/guilds", ({ "Guilds", "?" }) }),
        
        ({ "/d/admin/room/domain_control", ({ "Admin", "Domain control" }) }),
        ({ "/d/admin", ({ "Admin", "?" }) }),

        ({ "/d", ({ "Other", "Unknown /d room" }) }),
   
    });
    
    
    if( !environment(person) ) {
        return ({ "Other", "No environment" });
    }
    
    foreach( place in areas ) {
        if( strsrch( file_name(environment(person)), place[0] ) == 0 ) {
            return place[1];
        }
    }
    
    return ({ "Other", "Unknown environment" });
    
} /* determine_location() */


int get_guild_id(object person) {
    switch(person->query_guild_ob()) {
        case "/std/guilds/warrior":
            return 0;
        case "/std/guilds/assassin":
            return 1;
        case "/std/guilds/thief":
            return 2;
        case "/std/guilds/wizard":
            return 3;
        case "/std/guilds/priest":
            return 4;
        case "/std/guilds/witch":
            return 5;
        default:
            return 6;
    }
} /* get_guild_id() */


mapping get_dist( string filter, string arg ) {
    mapping ps;
    object *people, person;
    string *loc;
    int to_add;
    
    people = filter( users(), (: !$1->query_creator() && 
        $1->query_name() != "logon" :) );
    ps = ([ ]);
    
    foreach( person in people ) {
    
        loc = determine_location(person);
        
        if( !ps[loc[0]] ) {
            if( filter == "average" ) {
                ps[loc[0]] = ({ ([ ]), 0, ({ -1, 0, 0 }) });
            }
            else {
                ps[loc[0]] = ({ ([ ]), 0, 0 });
            }
        }
        ps[loc[0]][1]++;
        
        if( !ps[loc[0]][0][loc[1]] ) {
            if( filter == "average" ) {
                ps[loc[0]][0][loc[1]] = ({ 0, ({ -1, 0, 0 }) });
            }
            else {    
                ps[loc[0]][0][loc[1]] = ({ 0, 0 });
            }
        }
        ps[loc[0]][0][loc[1]][0]++;
        
        switch(filter) {
            case "verbosity":
                if( person->query_verbose(arg) ) {
                    ps[loc[0]][2]++;
                    ps[loc[0]][0][loc[1]][1]++;
                }
                break;
            
            case "average":
                to_add = 0;
                if( arg == "guild level" ) {
                    to_add = person->query_level();
                }
                if( arg == "rating" ) {
                    to_add = (int)TOP_TEN_HANDLER->calculate_rating(person);
                }
                if( arg == "age" ) {
                    to_add = -person->query_time_on();
                }
                
                if( to_add < ps[loc[0]][2][0] || ps[loc[0]][2][0] == -1 ) {
                    ps[loc[0]][2][0] = to_add;
                }
                ps[loc[0]][2][1] += to_add;
                if( to_add > ps[loc[0]][2][2] ) {
                    ps[loc[0]][2][2] = to_add;
                }
                
                if( to_add < ps[loc[0]][0][loc[1]][1][0] || 
                    ps[loc[0]][0][loc[1]][1][0] == -1 ) {
                    
                    ps[loc[0]][0][loc[1]][1][0] = to_add;
                }
                ps[loc[0]][0][loc[1]][1][1] += to_add;
                if( to_add > ps[loc[0]][0][loc[1]][1][2] ) {
                    ps[loc[0]][0][loc[1]][1][2] = to_add;
                }
                
                break;
            
            case "category":
                if( arg == "guild" ) {
                    if( !ps[loc[0]][2] ) {
                        ps[loc[0]][2] = ({0,0,0,0,0,0,0});
                    }
                    if( !ps[loc[0]][0][loc[1]][1] ) {
                        ps[loc[0]][0][loc[1]][1] = ({0,0,0,0,0,0,0});
                    }
                    to_add = get_guild_id(person);
                    ps[loc[0]][2][to_add]++;
                    ps[loc[0]][0][loc[1]][1][to_add]++;
                }
                break;
            
            default:
        }
    }
    
    return ps;

} /* get_dist() */


int cmd( string filter, string arg, int verbose ) {
    string place, zone, *guilds;
    mapping players, zones;
    int total, i, j;
    string tmp;
    string ret;

    ret = "";
    players = get_dist(filter,arg);
    total = sizeof(filter( users(), (: !$1->query_creator() && 
        $1->query_name() != "logon" :) ));
    
    guilds = ({ "Warriors", "Assassins", "Thieves", "Wizards", "Priests", 
        "Witches", "Adventurers" });
    
    tmp = "\n";
    tmp+= sprintf( "%-27s", "Distribution of players:" );
    
    if( filter == "category" && arg == "guild" ) {
        j = (this_player()->query_cols() - 27) / (sizeof(guilds));
        for( i=0; i<sizeof(guilds); i++ ) {
            tmp += sprintf( "%" + j + ".3s", guilds[i] );
        }
    }
    if( filter == "average" ) {
        if( arg == "age" ) {
            j = (this_player()->query_cols() - 27);
            tmp += sprintf( "%-" + j + "s", "   Average age" );
        }
        else {
            j = (this_player()->query_cols() - 27) / 3;
            tmp += sprintf( "%" + j + "s", "Lowest" );
            tmp += sprintf( "%" + j + "s", "Average" );
            tmp += sprintf( "%" + j + "s", "Highest" );
        }
    }
    
    tmp += "\n\n";
    ret += tmp;

    foreach( place in keys(players) ) {
        
        tmp = "%^YELLOW%^";
        tmp+= sprintf( "%-24.24s", place );
        tmp+= sprintf( "%3.1i", players[place][1] );
        
        switch( filter ) {
            case "verbosity": 
                tmp += players[place][2] + "/" + 
                    players[place][1] + " verbose " + arg;
                break;
            
            case "average":
                if( arg == "age" ) {
                    tmp += sprintf( "%-"+ j +"s", "   " + query_time_string(
                        players[place][2][1] / players[place][1] ) );
                }
                else {
                    tmp += sprintf( "%"+ j +".1i", players[place][2][0] );
                    tmp += sprintf( "%"+ j +".1i", players[place][2][1] /             
                        players[place][1] );
                    tmp += sprintf( "%"+ j +".1i", players[place][2][2] );
                }
                break;
            
            case "category":
                if( arg == "guild" ) {
                    for( i=0; i<sizeof(guilds); i++ ) {
                        tmp += sprintf( "%" + j + ".1i", 
                            players[place][2][i] );
                    }
                }
                break;
            
            default:
                tmp += "   (";
                tmp += sprintf( "%3s", 
                    ((100 * players[place][1]) / total + "%") );
                tmp += ")";
                break;
   
        } 
        tmp += "%^RESET%^";
        tmp += "\n";
        ret += tmp;

        if (verbose) {
           zones = players[place][0];
           foreach( zone in keys( zones ) ) {
        
               tmp = sprintf( "%-24.24s", ("  - " + zone) );
               tmp+= sprintf( "%3.1i", zones[zone][0] );
               
               switch( filter ) {
                   case "verbosity": 
                       tmp += " (" + zones[zone][1] + "/" + zones[zone][0] + 
                           " verbose " + arg + ")";
                       break;
                   
                   case "average":
                       if( arg == "age" ) {
                           tmp += sprintf( "%-"+ j +"s", "   " + 
                               query_time_string( zones[zone][1][1] /             
                               zones[zone][0] ) );
                       }
                       else {
                           tmp += sprintf( "%"+ j +".1i", zones[zone][1][0] );
                           tmp += sprintf( "%"+ j +".1i", zones[zone][1][1] /             
                               zones[zone][0] );
                           tmp += sprintf( "%"+ j +".1i", zones[zone][1][2] );
                       }
                       break;
                   
                   case "category":
                       if( arg == "guild" ) {
                           for( i=0; i<sizeof(guilds); i++ ) {
                               if( !zones[zone][1][i] ) {
                                   tmp += "%^BLACK%^";
                                   tmp += sprintf( "%" + j + ".1i",
                                        zones[zone][1][i] );
                                   tmp += "%^RESET%^";
                               }
                               else {
                                   tmp += sprintf( "%" + j + ".1i",
                                       zones[zone][1][i] );
                               }
                           }
                       }
                       break;
                   
                   default:
                       tmp += "   (";
                       tmp += sprintf( "%3s", 
                           ((100 * zones[zone][0]) / total + "%") );
                       tmp += ")";
                       break;
               }                 
               tmp += "\n";
               ret += tmp;
           }
        }
        if (verbose) {
           ret += "\n";
        }
    }
    
    ret += "Total = " + total + "\n";
    write("$P$Distribution$P$" + ret);
    
    return 1;

} /* cmd() */


mixed *query_patterns() {
    return ({ 
        "by domain", (: cmd(0, 0, 0) :),
        //"by verbose <string'type'>", (: cmd("verbosity", $4[0], 0) :),
        "by guild level",  (: cmd("average", "guild level", 0) :),
        "by rating",  (: cmd("average", "rating", 0) :),
        "by age",  (: cmd("average", "age", 0) :),
        "by guild",  (: cmd("category", "guild", 0) :),
        "verbose by domain", (: cmd(0, 0, 1) :),
        //"verbose by verbose <string'type'>", (: cmd("verbosity", $4[0], 1) :),
        "verbose by guild level",  (: cmd("average", "guild level", 1) :),
        "verbose by rating",  (: cmd("average", "rating", 1) :),
        "verbose by age",  (: cmd("average", "age", 1) :),
        "verbose by guild",  (: cmd("category", "guild", 1) :),
    });
} /* query_patterns() */
