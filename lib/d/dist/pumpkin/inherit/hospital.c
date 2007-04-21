/**
 * @main
 * This is the hospital inheritable for Counterweight Continent, it handles the 
 * creation of NPC's (and other nicities.)
 * <p>
 * The CWC hospital object works differently to many hospitals used in 
 * other domains, because it allows certain methods to be redefined and
 * the hospital "customized".  This allows different NPC zones and 
 * configurations, and even different logging options all based on the
 * same code.
 * <p>
 * @author Taffyd
 * @started December 3, 1998.
 * @index cwc_hospital
 */

#include <armoury.h>
#include <hospital.h>
#include "path.h"
#include HOSPITAL_INC

/* Comment this out if you want to disable hospital generation
   of NPC's through the new data compiler system. */    
#define CREATE_NPCS 1

#define REGEN_TIME 1 * 60 * 60
#define DEFAULT_LOG_DIRECTORY "/d/cwc/log"
#define DEFAULT_POPULATION 3
#define MINIMUM_DEATH_REGEN 3

inherit "/std/room";

class regenerated_npc {
    string type;
    string load_position;
}

protected void load_file();
protected void save_file();
protected void set_save_file(string new_save);
protected void restore_default_npcs();
protected void set_max_living(int new_living);
protected void set_hospital_type(string new_type);
protected object *make_monster(string type);
public object *get_monster(string type);
public void hospital_log_file(string file, string format, mixed *args ...);
public int get_item(object destination, string *items);
public string query_save_file();
public int query_max_living();
public int *query_npcs();
public mapping query_hospital_npcs();
public void add_npc_type(string type, string *data);
public void regen_after_death( object dead_npc );

private nosave string _save_file, _hospital_type;
private nosave int _max_living;
private nosave mixed *_regenerated_npcs;
private nosave int _hospital_call_id;
private nosave mapping _log_file_info;
private nosave string _hospital_log_directory;

public mapping _unique_npcs;
public mapping _hospital_npcs;

public int _last_npc_check, _zone_npcs;

private nosave mapping _npc_groups;
private nosave mapping _npc_info;

/** @ignore yes */
void create() {
    seteuid(master()->creator_file(file_name(this_object())));
    
    do_setup++;
    ::create();
    do_setup--;

    if (_save_file) {
        load_file();
    
        if (!_hospital_npcs) {
            restore_default_npcs();
        }
        
        if (!_unique_npcs) {
            _unique_npcs = ([ ]);    
        }               
        
        save_file();                
    }
    
    if (!_npc_groups)
        _npc_groups = ([ ]);
        
    if (!_npc_groups)
        _npc_info = ([ ]);

    _regenerated_npcs = ({ });
    _log_file_info = ([ ]);
    
    if (!do_setup) {
        set_short("Counterweight Continent Hospital");
        set_long("All around you NPC's wait in neat little lines, "
            "ready to be sent on a massive journey that will ultimately "
            "lead to their deaths.  Some of them stand by a small window "
            "in the far wall, looking out at freedom.\n");
        add_property("determinate", "the ");
        set_light(60);
    }
} /* create() */

/** 
 * This function provides an interface to the armoury, selecting 
 * an item at random from the 'items' array and attempting to
 * clone it.
 * <p>
 * Any items of jewellery, clothing, weapons, armour or scabbards
 * that are available in the armoury can be accessed using this 
 * function.
 * <p>
 * @param destination where to place the item that is fetched 
 * from the armoury.
 * @param items an array of items, of which one will be selected, 
 * cloned, and then moved to destination.
 * <p>
 * @see /obj/handlers/armoury
 * @return 1 if the item was successfully created, 0 if it was not.
 */
public int get_item(object destination, string *items) {
    int i;
    object item;
    
    if (!objectp(destination)) {        
        return 0;        
    }            
    
    while (!item) {
        i = random(sizeof(items));
        item = ARMOURY->request_item(items[i], 80 + random(21));
        
        if (!objectp(item)) {
            items = items[0..i-1] + items[i+1..];
            continue;
        }
        
        if (!sizeof(items)) {
            hospital_log_file("BROKEN_ITEMS", "Unable to select any items "
                "for %s in the CWC hospital.\n", previous_object()->short());
            break;            
        }
    }    

    if (objectp(item)) {
        item->move(destination);    
        return 1;
    }
    
    return 0;    
} /* get_item() */

/** @ignore yes */
protected void load_file() {
    if (file_size(_save_file + ".o") > -1) {
        unguarded((: restore_object, _save_file :));
    }
} /* load_file() */

/** @ignore yes */
protected void save_file() {
    unguarded((: save_object, _save_file :));
} /* save_file() */

/** 
 * This method is used to set the save file for the hospital.  This 
 * file is where all of the hospitals NPC data is stored while it is
 * not active and in between reboots.  
 * <p>
 * If this file does not exist when the hospital is loaded, then the 
 * variables will be initialised to their defaults and 
 * <b>restore_default_npcs()</b> called.
 * <p>
 * @param new_save the file name to save the hospital data in
 */
protected void set_save_file(string new_save) {
    _save_file = new_save;
} /* set_save_file() */

/** 
 * This returns the location where hospital data will be stored.
 */
public string query_save_file() {
    return _save_file;
} /* query_save_file() */

/** 
 * This method returns whether or not it is time to regenerate a
 * unique NPC. The regeneration time is controlled by the REGEN_TIME
 * #define.
 * <p>
 * @param who this is the name or object reference of the NPC which is trying 
 * to be cloned. Now that I think about it, having an object reference
 * is pretty useless but I'm sure there was a very good reason for it.
 * <p>
 * @return 1 if it is time to make the unique NPC, or 0 if it is not.
 */
public int make_unique(mixed who) {
    if (!_unique_npcs)
        return 0;
        
    if (objectp(who))    
        who = who->query_name();
        
    if ( _unique_npcs[who] > time() )
        return 0;        
        
    _unique_npcs[who] = time() + REGEN_TIME;
    save_file();
} /* make_unique() */

/** 
 * This method is used to query how long until it is time for the
 * NPC to regenerate.
 * <p>
 * @param who the name or object reference of the NPC to test.
 * @return the time at which this NPC will regenerate.
 */
public int query_regen_time(mixed who) {
    if (objectp(who))    
        who = who->query_name();
        
    if (!_unique_npcs)        
        return 0;
    
    return _unique_npcs[who];    
} /* query_regen_time() */

/** 
 * This sets the maximum number of NPC's to be cloned by the 
 * hospital.  If this is not set then no NPC's will ever be 
 * made by the hospital, and if it is too high then the streets
 * will be overflowing with NPC's. 
 * <p>
 * @param new_living the new maximum number of NPC's to clone
 * @see ok_to_clone()
 */
protected void set_max_living(int new_living) {
    _max_living = new_living;    
} /* set_max_living() */

/** 
 * @return the maximum number of NPC's that can be cloned by 
 * the hospital.
 * @see set_max_living()
 */
public int query_max_living() {
    return _max_living;
} /* query_max_living() */

/** 
 * This method is used to restore the hospital NPC's mapping if the 
 * save file cannot be restored.  This should be masked in localised
 * hospitals so that their unique region settings can be automatically
 * setup.
 * <p>
 * If this function is overloaded, then ::restore_default_npcs() should
 * not be called, otherwise they will be cleared.
 */
protected void restore_default_npcs() {
    _hospital_npcs = ([ ]);    
} /* restore_default_npcs() */

/** 
 * This method is used to determine whether or not it is ok to create 
 * a new NPC.  It calculates the number of NPC's that exist in the
 * area controlled by the hospital, and checks this against the maximum
 * number of living objects allowed in this area.
 * <p>
 * @see set_max_living()
 * @see /d/cwc/city_inherit
 */
public int ok_to_clone(string where) {   
    if (time() > _last_npc_check + 300) {
        _last_npc_check = time();
                
        _zone_npcs = sizeof(filter(named_livings(), (: environment($1) && 
            base_name(environment($1))[0..(sizeof(CWC) + sizeof($(where)) - 1)] == 
                (CWC + $(where)) :)));            
    }        

    return _zone_npcs < _max_living;
} /* ok_to_clone() */

/** 
 * This method is used to return statistical information about the hospital.
 * @return an array containing the number of NPC's in the zone, and the
 * time at which this number was last calculated.
 */
public int *query_npcs() {
    return ({ _zone_npcs, _last_npc_check });
} /* query_npcs() */

/**
 * This method can be used to add a new type of NPC to the hospital, or
 * to extend the NPC's in an existing zone.
 * <p>
 * @param type the zone in which the NPC's will be created
 * @param data an array of NPC's to create in the zone.
 */
public void add_npc_type(string type, string *data) {
    if (undefinedp(_hospital_npcs[type])) {
        _hospital_npcs[type] = copy(data);
    }
    else { 
        if (arrayp(_hospital_npcs[type])) {
            _hospital_npcs[type] += data;        
        }
    }
                    
    save_file();
} /* add_npc_type() */

/** 
 * This method returns all of the hospital zones and NPC data. It 
 * is fairly useless.
 * <p>
 * @return a mapping containing hospital zone and NPC data information.
 */ 
public mapping query_hospital_npcs() {
    return copy(_hospital_npcs);
} /* query_hospital_npcs() */

/** 
 * This method is used to create an NPC of a specific name or of a 
 * hospital type.
 * <p>
 * @param type the type of NPC to create. This can either be a specific
 * name (such as 'sailor') or a zone (such as 'docks'). Valid zones
 * must be defined in the hospital for this to work correctly.
 * @return the NPC's made by the hospital
 */
public object *get_monster(string type) {
    object *monsters, monster;

    if (_hospital_npcs[type]) {
        tell_creator("taffyd", "Attempting to create a %s NPC.\n", type);
        monsters = make_monster(_hospital_npcs[type][random(sizeof(_hospital_npcs[type]))]);
        tell_creator("taffyd", "Monsters are now %O.\n", monsters);
    }
    else {
        monsters = make_monster(type);
    }
    
    // Now do the stuff that is common for all npcs
    
    foreach (monster in monsters) {
        monster->add_property("monster_type", type);
        monster->add_effect("/std/effects/npc/i_died", 
            (: regen_after_death :) );
    }
    
    return monsters;
} /* get_monster() */

/**
 * This method is used to clone a monster. It is called by get_monster() and 
 * should be redefined in your localised hospital zone to add support for 
 * new monsters.
 * <p>
 * Note that if you to redefine this function, you do not need to call the
 * precursor. Otherwise your hospital will start producing 'hospital 
 * accidents'. (The default NPC for an unconfigured NPC).
 * <p>
 * @param type the type of NPC to generate.
 * @return the object reference of the NPC or NPC's made by the hospital.
 */
protected object *make_monster(string type) {
    object monster;
    
    monster = clone_object("/obj/monster");
    monster->set_name("failure");
    monster->set_short("hospital accident");
    monster->set_race("human");
    monster->set_guild("warrior");
    monster->set_level(1);           
                       
    return ({ monster });
} /* make_monster() */

/**
 * This method is called by the npc.death effect after an NPC that is handled
 * by the hospital has died. It tells the room where it was generated to 
 * make a new NPC and to all sorts of funky things.
 * <p>
 * This is not fully implemented yet.
 */
public void regen_after_death( object dead_npc ) {
    object *monsters, destination;
    class regenerated_npc new_npc;
    int i, max;
  
    if ( !dead_npc )
        return;
                
    _regenerated_npcs += ({ new( class regenerated_npc,
        type : dead_npc->query_property("monster_type"), 
        load_position : previous_object()->query_property("start location") ) });

    if ( !ok_to_clone(_hospital_type) || sizeof(_regenerated_npcs) < MINIMUM_DEATH_REGEN )
        return;

    new_npc = _regenerated_npcs[0];
    
    if ( !classp( new_npc ) )
        return;
        
    _regenerated_npcs = _regenerated_npcs[1..];

    if (!new_npc->load_position || !new_npc->type );
        return;
    
    max = random(5);

    for ( i = 0; i < max; i++ ){
        destination = find_object( new_npc->load_position );
        
        if ( !objectp( destination ) )
            return;

        monsters = get_monster(new_npc->type);
        destination->replace_monster(previous_object(), monsters);

        call_out((: $1->announce_entry($2) :), 8 + random(6), destination, 
            monsters);
    }
} /* regen_after_death() */

/**
 * This method is used to set the hospital type.  This is actually used to
 * set the geographical region that the hospital controls.
 * <p>
 * @example
 * // Only NPC's in /d/cwc/Bes_Pelargic will be managed by this hospital.
 * set_hospital_type("Bes_Pelargic");
 * @example
 * // Only NPC's in the Aarrgh forest will be managed by this hospital.
 * set_hospital_type("Forests/Aarrgh");
 * @param new_type the type and geographical region managed by this hospital
 */
protected void set_hospital_type(string new_type) {
    _hospital_type = new_type;
} /* set_hospital_type() */

/** 
 * This method provides log_file() functionality but for /d/cwc/log 
 * instead.  You should use this method to record errors and whatever 
 * that are domain specific instead of using log_file().
 * @example
 * hospital_log_file("BAD_HOSPITAL_NPC", "%s: Bad NPC hospital data, %O.\n", 
 * ctime(time()), _regenerated_npcs);
 * // This would log to /d/cwc/log/BAD_HOSPITAL_NPC.
 * @param file the file name to log to. This will have /d/cwc/log/ inserted 
 * before it.
 * @param format the text to log, can contain sprintf() format specifiers.
 * @param args the arguments for any sprintf format thingies
 */
public void hospital_log_file(string file, string format, mixed *args ...) {
    string filename;
    
    filename = _hospital_log_directory;
    
    /* We are restricted to logging in our domain, let's be neat about it. */
    
    if (file[0] == '/') {
        file = file[ strsrch( file, "/", -1 ) + 1 .. ];
    }
    
    /* Link to sefun */
    
    log_file( _hospital_log_directory + "/" + file, format, args ...);
} /* hospital_log_file() */

/** 
 * This method sets the directory in to which hospital_log_file will
 * log by default. If the directory does not exist, then it is set to
 * DEFAULT_LOG_DIRECTORY.
 * @param new_dir the directory to log to.
 * @example
 * set_log_directory( "/d/cwc/Bes_Pelargic/log" );
 * // log directory is now /d/cwc/Bes_Pelargic/log
 * @example
 * // (cre) MacChirton: ithinkmykeyboardisbroken
 * set_log_directory( "/d/cWc/BeS_pelarGic/lrg" );
 * // Displays: "/d/cwc/hospital/bes_pelargic: Invalid log 
 * // directory, defaulting to /d/cwc/log"
 */
protected void set_log_directory(string new_dir) {
    
    if (file_size(new_dir) != -2) {
        tell_creator( previous_object( 1 ), "%s: Invalid "
            "log directory -- defaulting to %s.\n",
            file_name( this_object() ), DEFAULT_LOG_DIRECTORY );
            
        new_dir = DEFAULT_LOG_DIRECTORY;
    }

    _hospital_log_directory = new_dir;
} /* set_log_directory() */

protected void set_npc_groups(mapping x) {
    _npc_groups = x;
} /* set_npc_groups() */

protected void set_npc_info(mapping x) {
    _npc_info = x;
} /* set_npc_info() */

public object data_get_npc(string npc_name) {
    class npc_info info;
    object npc;
    function func;
    
    if ( undefinedp(_npc_info[npc_name]) )
        return 0;
    
    info = _npc_info[npc_name];
    
    if (stringp(info->base))
        npc = clone_object(info->base);
    else
        return 0;
    
    if (arrayp(info->functions)) {
        foreach (func in info->functions) {
            evaluate(func, npc, previous_object());
        }
    }
    
    return npc;
} /* data_get_npc() */

object *data_get_group(string group_name) {
    class npc_group group;
    function func;
    int population, quantity, i;
    object *npcs, npc;
    
    if ( undefinedp(_npc_groups[group_name]) )
        return 0;
        
    group = _npc_groups[group_name];

    /* Determine the population from the class, if this is undefined
     * then we make set it to DEFAULT_POPULATION.
     */
         
    if (functionp(group->population))
        population = evaluate(group->population);
    else {
        population = DEFAULT_POPULATION;
    }

    if (!group->storage) {
        group->storage = ({ });
    }
    else {
        group->storage = filter(group->storage, (: objectp($1) :));
        
        /* 
         * If we have more NPC's than the population, then stop right now.
         */
        if (sizeof(group->storage) >= population)
            return 0;
    }
    
    if (functionp(group->quantity)) {
        quantity = evaluate(group->quantity);        
    }
    else {
        quantity = 1;
    }
    
    if (stringp(group->npc)) {
        npcs = allocate(quantity);
        
        for (i = 0; i < quantity; i++) {
            npcs[i] = data_get_npc(group->npc);
        }
        
        npcs = filter(npcs, (: objectp($1) :));
        group->storage += npcs;
    }
    
    foreach (npc in npcs) {
        if (arrayp(group->functions)) {
            foreach (func in group->functions) {
                evaluate(func, npc, previous_object());
            }
        }
        
    }
    
    return npcs;
} /* data_get_group() */

/*
 * I stole this function straight out of Jeremy's sur hospital.
 * It does weird and wonderful things.
 * <p>
 * This is a utility function for the rooms.  It's easier on the
 * programmer to make the weights non-cumulative, but it's inefficient
 * for the code.  The room can call this in setup() (or in a callout
 * if time is critical) to make the npc array cumulative.  Note that
 * the original array isn't modified.  This allows the original array to be
 * dynamically changed and recalculated.
 * <p>
 * @param a an array of NPC's to generate, in the format 
 * ({ ({ "name", chance }), ... })
 */
mixed accumulate_npc_array( mixed a ) {
    int cum, i;
    mixed choices;

    choices = copy(a);
    cum = 0;
    for (i = 0; i < sizeof(choices); i++) {
        cum += choices[i][0];
        choices[i][0] = cum;
    }
    return choices;
} /* accumulate_npc_array() */

/**
 * I stole this one as well.
 * <p>
 * This method randomly selects an NPC or a group of NPC's from 
 * the list of defined NPC's for the room/region.
 * @param choices if this is passed, then the array is used to 
 * select NPC's from.  This array must be a cumulative array. (see example)
 * @return an 
 * Returns a group of NPCs randomly selected.  "choices" is an array
 * of 2-element arrays giving the cumulative chance of the NPC group
 * and the group name, e.g.:
 * ({ ({ 5, "wolves" }), ({ 10, "trolls" }), ({ 20, "bandits" }) })
 * This would clone wolves 25% of the time, trolls 25% of the time, and
 * bandits 50% of the time.  If this is too confusing, see
 * accumulate_npc_array().
 */
varargs mixed get_random_npcs(mixed *choices) {
    int i;
    int chance;
    
    if (!choices)
        choices = previous_object()->query_npcs();

    if (!arrayp(choices) || !sizeof(choices))
        return 0;
    
    chance = random(choices[<1][0]);
       
    for (i = 0; i < sizeof( choices ); i++) {
        if ( choices[ i ][ 0 ] > chance ) {
            return data_get_group( choices[ i ][ 1 ]);        
        }
    }
    
    return 0;
} /* get_random_npcs() */
