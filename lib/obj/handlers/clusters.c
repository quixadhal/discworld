/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: clusters.c,v 1.17 2003/02/12 19:13:36 wodan Exp $
 *
 */

#if !efun_defined(db_exec)
#define OLD
#else
#include <db.h>
#endif
#include <map.h>
#define SIZE 7 //don't make this too high
#define LEVEL(x) load(x)->level

#include <config.h>

nosave int maxnum = 0;
nosave int lasttime = 0;
nosave int count = 0;
nosave mapping used = ([ ]);
nosave mixed *todo= ({ });
nosave mapping cache = ([ ]);
nosave int setup_done = 1;
nosave int db;
nosave int setup_start_time;

class cluster{
  mixed *members;
  int level; //0 is cluster of rooms, 1 is cluster of cluster of rooms etc
  int cluster_number; //in which cluster is this cluster
  int this_cluster;
  mixed *contacts;
}

#ifndef OLD
private int get_cluster(mixed);

private mixed cdb_fetch(int fd, int row){
  mixed stuff = db_fetch(fd, row);
  if(arrayp(stuff)){
    mixed tmp, ret = ({});
    foreach(tmp in stuff){
      if(stringp(tmp)){
        int i;
        sscanf(tmp, "%d", i);
        if(i || tmp == "0")
          tmp = i;
      }
      ret += ({tmp});
    }
    stuff = ret;
  }
  return stuff;
}

#endif

private class cluster load(int x){
  mixed rows;
  class cluster tmp = new(class cluster);
  
  if(cache[x])
    return cache[x];
#ifdef OLD
  cache[x] = restore_variable(unguarded((: read_file(sprintf("/save/clusters/%d",
                                                           (int)$(x))) :)));
#else
  rows = db_exec(db, "select member from cluster_members where cluster_number = %d", x);
  if(stringp(rows) || !rows)
    return 0;
  tmp->members = ({});
  rows++;
  while(--rows)
    tmp->members += cdb_fetch(db, rows);

  rows = db_exec(db, "select level, in_cluster from cluster_list where cluster_number = %d", x);
  if(stringp(rows)){
    //maybe the database handle died
    db_close(db);
    db = db_connect("localhost","discworld",CONFIG_DB_USER);
    return 0;
  }

  rows = cdb_fetch(db, 1);
  
  tmp->level = rows[0];
  tmp->cluster_number = rows[1];
  tmp->this_cluster = x;
  
  rows = db_exec(db, "select contact from cluster_contacts where cluster_number = %d", x);
  if(stringp(rows))
    return 0;
  tmp->contacts = ({});
  rows++;
  while(--rows)
    tmp->contacts += cdb_fetch(db, rows);
  cache[x] = tmp;
#endif
  return cache[x];
}

private void save(class cluster x){
  
#ifdef OLD
  unguarded((:write_file(sprintf("/save/clusters/%d", $(x->this_cluster)),
                           save_variable($(x)), 1):));
#else
  db_exec(db, "insert into cluster_list values (%d, %d, %d)", x->this_cluster,
          x->level, x->cluster_number);

  map(x->members, (:db_exec(db, "insert into cluster_members values (%d, '%s')",
                            $(x->this_cluster), db_escape(""+$1)):));

  map(x->contacts, (:db_exec(db, "insert into cluster_contacts values (%d, '%s')",
                            $(x->this_cluster), db_escape(""+$1)):));
#endif
  cache[x->this_cluster] = x;
}

private void make_clusters(mixed);

nosave function fix_contacts;
nosave function fix_room_contacts;

private void create(){
#ifndef OLD
  db = db_connect("localhost","discworld",CONFIG_DB_USER);
  map(({"cluster_members", "cluster_list", "cluster_contacts"}),
      (:db_exec(db, "delete from %s;", $1):));
#endif
  fix_room_contacts = function(int i){
    class cluster fixing = load(i);
    if((sizeof(fixing->contacts)) && intp(fixing->contacts[0])){
      error(sprintf("%d in two or more clusters", i));
      return;
    }
    fixing->contacts = uniq_array(map(fixing->contacts,
#ifdef OLD
                                      (:MAP->query_cluster($1):)
#else
                                      (:cache[$1]:)
#endif
                                      ));
  save(fixing);
  };


  fix_contacts = function (int i){
    class cluster fixing = load(i);

    reset_eval_cost();

    if(fixing->level){

      if((sizeof(fixing->contacts)) && (fixing->level ==
                                        LEVEL(fixing->contacts[0]))){
        error(sprintf("%d in two or more clusters", i));
        return;
      }
      
      fixing->contacts = uniq_array(map(fixing->contacts,
                                        (:load($1)->cluster_number:)));
      
      save(fixing);
      
      if(fixing->level > 1){
        map(fixing->members, (:efun::call_out((fix_contacts), 0, $1):));
        return;
      }
      
      map(fixing->members, fix_room_contacts);
    }
  };

#ifdef OLD
  unguarded(function(){
    string file, *files=get_dir("/save/clusters/");
    foreach(file in files)
      rm(sprintf("/save/clusters/%s", file));
  });
#endif
}

private void make_clusters(mixed start){
  //read at your own risk...

  int count = 0, depth = -1, oldmax = maxnum, level = (stringp(start)? 0 :
                                                       load(start)->level + 1);
  class cluster current;
  mixed *list;
  function qdd, real;

  if(stringp(start)){ //normal rooms
    qdd = function(string room){

      //f leaves just the rooms with an exit back
      function f = function(string there, string room){
        mixed tmp;
        return member_array(room, 
                            (tmp = MAP->qdd(there), tmp? tmp : ({}))) + 1;
      };

      string *qdd = MAP->qdd(room);
      return filter(allocate(sizeof(qdd) / 2, (:$(qdd)[$1 * 2 + 1]:)), f, room);
    };

    real = (:$1:);

  } else { 
    qdd = (:load($1)->contacts:);

    if(stringp(load(start)->contacts[0]))
      real = (:cache[$1]:); //(:MAP->query_cluster($1):);
    else
      real = (:load($1)->cluster_number:);
  }
  
  current = new (class cluster, members:({start}),
                 level:level,
                 this_cluster:++maxnum,
                 contacts:({}));

  list = ({start});
  if(used[start])
    error(sprintf("%O already in a cluster", start));
  used[start] = 1;

  while(++depth < sizeof(list) || sizeof(todo)){
    mixed *exits;
    mixed exit;
    if(depth < sizeof(list)) 
      exits = (*qdd)(list[depth]);
    else { 
      count = SIZE + 1;
      exits = ({});
    }
    foreach(exit in exits) {
      if(!used[exit = (*real)(exit)]) {
        current->members += ({exit});
        list += ({exit});
        used[exit] = 1;
        if(++count > SIZE)
          break;
      }
    }
    if(count > SIZE || !(((1 + depth) < sizeof(list)) || sizeof(todo))){
      mixed tmp, *contacts = ({});
      count = 0;
      foreach(tmp in current->members){
        reset_eval_cost();
        if(stringp(tmp)){
#ifdef OLD
          MAP->set_cluster(tmp, current->this_cluster)
#endif
          ;
          cache[tmp] = current->this_cluster;
        }
        else{
          class cluster clust;
          clust = load(tmp);
          clust->cluster_number = current->this_cluster;
          cache[tmp] = clust;
        }
        
        exits = (*qdd)(tmp);

        if(exits){
          function add_non_member, convert_and_add;

          // and now we need to get the rooms this cluster connects to from the
          // exits info. 

          add_non_member = (: (member_array($2, $(current->members)) == -1)?
                            ($1 | ({ $2 })) : //don't want doubles
                            $1 //room was a member
                           :);

          convert_and_add = (:evaluate($(add_non_member), $1, 
                                      evaluate($(real), $2)):);

          contacts = implode(exits, convert_and_add, contacts);
        }
      }

      //and now we generate exit names for them
      current->contacts = contacts;

      if(sizeof(contacts) || current->level > 1)
        cache[current->this_cluster] = current; 
      else 
        maxnum--;

      todo = filter(todo + list + contacts, (:!used[$1]:)); //rooms not yet 
                                                            //in a cluster 

      depth = -1;
      if(sizeof(todo)){
        list = ({todo[0]});
        todo = todo[1..];
        if(used[list[0]])
          error("corrupt todo list");
        used[list[0]] = 1;
        
        current = new (class cluster, members:({list[0]}),
                       level:level,
                       this_cluster:++maxnum,
                       contacts:({}));
      } else 
        list = ({ });   
    }
  }
  
  if((maxnum - oldmax) > SIZE)
    make_clusters(current->this_cluster - 1);
  else { //fix contacts arrays
    int i;
    for(i = oldmax + 1; i <= maxnum; i++)
      if(LEVEL(i))
        efun::call_out(fix_contacts, 0, i);
      else
        efun::call_out(fix_room_contacts, 0, i);
  }
  used = ([]);
  efun::call_out((:cache = ([]), setup_done = 1:), 2);
}

#ifndef OLD
private int get_cluster(mixed thing){
  if(stringp(thing)){
    mixed exret = db_exec(db, "select cluster_number from cluster_members where member = '%s'",
                          db_escape(thing));
    if(!stringp(exret) && exret){
      return cdb_fetch(db, 1)[0];
    }
  } else if(intp(thing)){
    mixed exret = db_exec(db, "select in_cluster from cluster_list where cluster_number = %d",
                          thing);
    if(!stringp(exret) && exret){
      return cdb_fetch(db, 1)[0];
    }
  }
  return 0;
}
#endif

private int query_top_cluster(string room){
  int ret;
#ifdef OLD
  ret = MAP->query_cluster(room);

  if(ret && unguarded((:file_size(sprintf("/save/clusters/%d", $(ret))):)) > 0)
    while(load(ret)->cluster_number){
      int last = ret;
      ret = load(ret)->cluster_number;
      if(last == ret)
        return ret;
    }
#else
  ret = get_cluster(room);
  if(ret){
    int last = ret;
    ret = get_cluster(ret);
    while(ret){
      last = ret;
      ret = get_cluster(ret);
    }
    return last;
  }
#endif
  else{ //new cluster space
    setup_start_time = time();
    setup_done = 0;
    make_clusters(room);
  }

  return ret;
}

#ifndef OLD
private mixed *get_members(int clust){
  mixed rows;
  rows = db_exec(db, "select member from cluster_members where cluster_number = %d", clust);
  if(stringp(rows))
    return ({ });
  return allocate(rows, (:cdb_fetch(db, $1 + 1)[0]:));
}

private mixed *get_contacts(int clust){
  mixed rows;
  rows = db_exec(db, "select contact from cluster_contacts where cluster_number = %d", clust);
  if(stringp(rows))
    return ({ });
  return allocate(rows, (:cdb_fetch(db, $1 + 1)[0]:));
}
#endif

private mixed *find_places(int start, int end, int *places){
  int found = 0, depth = 0;
  int *key;
  int *exits, bing, *result, ob, exit;
  mapping search = ([]);

  if(start == end){
#ifdef OLD
    return load(start)->members;
#else
    return get_members(start);
#endif
  }

  key = ({ start });
  search[start] = "binglewop";
  while (!found) {
    reset_eval_cost();
    ob = key[depth];
    if (ob) {
      /* Get the destinations */
#ifdef OLD
      exits = load(ob)->contacts;
#else
      exits = get_contacts(ob);
#endif
      foreach(exit in exits) {
        if ((!search[exit]) && (member_array(exit, places) != -1 ||
                               sizeof(places) == 0)) {
          search[exit] = ob;
          key += ({ exit });
          if (exit == end)
            found = 1;
        }
      }
    }
    if (!found && ++depth >= sizeof(key))
      return 0;
  }

  exits = ({ });
  bing = end;
  result = ({ });

  while (bing != start) {
    reset_eval_cost();
#ifdef OLD
    result += load(bing)->members;
#else
    result += get_members(bing);
#endif
    bing = search[bing];
  }
#ifdef OLD
  result += load(bing)->members;
#else
  result += get_members(bing);
#endif
  return result;
} /* find_places() */
  
private mixed find_route(mixed start, mixed end, string *places){
  int found = 0, i, depth = 0;
  string *key;
  string *exits, bing, *result, ob;
  mapping route = ([]), search = ([]);
  int t = real_time();
  
  if(start == end)
    return ({});

  key = ({ start });
  route[start] = "binglewop";
  while (!found) {
    reset_eval_cost();
    ob = key[depth];
    if (ob) {
      /* Get the destinations */
      exits = MAP->qdd(ob);
      for (i = 1; i < sizeof(exits); i += 2) {
        mixed exit = exits[i];
        if ((!route[exit]) && (member_array(exit, places) != -1 ||
                               sizeof(places) == 0)) {
          route[exit] = exits[i - 1];
          search[exit] = ob;
          key += ({ exit });
          if (exit == end)
            found = 1;
        }
      }
    }
    if (!found && (++depth >= sizeof(key) || (real_time() - t) > 4))
      return 0;
  }
  
  exits = ({ });
  bing = end;
  result = ({ });

  while (bing != start) {
    reset_eval_cost();
    /* Create it backwards, we go from the destination to the source */
    result = ({route[bing]}) + result; 
    bing = search[bing];
  }
  return result;
} /* find_route() */

nosave int debugging = 0;

private void debug(int *clusters){
  int cluster;
  if(debugging)
    error("read the error trace");
  debugging = 1;
  reset_eval_cost();
  while(intp(load(clusters[0])->members[0]))
    clusters = implode(clusters, (:$1 + load($2)->members:), ({}));
  foreach(cluster in clusters){
    string *rooms = load(cluster)->members;
    string room, room2;
    reset_eval_cost();
    foreach(room in rooms)
      foreach(room2 in rooms)
        if(!this_object()->get_route(room, room2))
          error("cluster error");
  }
}


/** 
 * Returns an array with move instructions from 'start' to 'end'
 * @param start the start location
 * @param end the end location
 * @param callback the function to call back with the result
 */

string* get_route(string start, string end, function callback){
  mixed *places = ({}), *oldplaces;
  int sip = 0; //succes is possible
  int from, to;
  int level;

  if(lasttime != time()){
    lasttime = time();
    count = 0;
  } else {
    if(count++ > 10) {
      return ({}); //10 routes per second is madness, probably a broken npc
    }
  }

  if(!setup_done){
    if((time()-setup_start_time) < 5)  {
      return 0;
    }
    //looks like we're broken cp runtime logs.
#ifndef OLD
    //catch(db_close(db));
#endif
    catch(unguarded(function(){
      cp("/log/runtime", "/w/wodan/clusterruntime");
      cp("/log/catch", "/w/wodan/clustercatch");
    }));
    
    //destruct(this_object());
    tell_creator("wodan", "would have dested");
    setup_done = 1;
    return 0;
  }

  if(!stringp(start)){
    start = base_name(start);
    if(!stringp(start))
      error("illegal start point");
  }  
  if(!stringp(end)){
    end = base_name(end);
    if(!stringp(end))
      error("illegal end point");
  }  
  if(catch(from = query_top_cluster(start))){
#ifndef OLD
    //may have lost the database connection
    db_close(db);
    db = db_connect("localhost","discworld", CONFIG_DB_USER);
#endif
    return 0;
  }

  if(!setup_done) {
    return 0;
  }

  if(from) {
    level = LEVEL(from);
  }

  to = query_top_cluster(end);
    
  if(!to || (level != LEVEL(to))) {
    return ({}); //a route can't exist
  }

  while (level) {
#ifdef OLD
    from = MAP->query_cluster(start);
    to = MAP->query_cluster(end);
#else
    from = get_cluster(start);
    to = get_cluster(end);
#endif
    
    while((from != to) && (LEVEL(from) < level)){
      from = load(from)->cluster_number;
      to = load(to)->cluster_number;
    }
    
    level = LEVEL(from);
  
    if(!level){ //this can only happen in the first loop
      if(!sip) {
        break;
      }
      error("wrong level");
    }
    oldplaces = places;
    places = find_places(from, to, places);
  
    //printf("level %d sizeof places %d\n", level, sizeof(places));
    if(!places){
      if(!sip) {
        return ({});
      }
      debug(oldplaces);
      error("this can't happen, honest!");
    }
    sip = 1;
    level--;
  }


  // level is now 0
#ifdef OLD
  from = MAP->query_cluster(start);
  to = MAP->query_cluster(end);
#else
  from = get_cluster(start);
  to = get_cluster(end);
#endif
  
  oldplaces = places;
  places = find_places(from, to, places);
  
  if(!places){
    if(!sip) {
      return ({});
    }
    debug(oldplaces);
    error("this can't happen, honest!!");
  }

  //now places is an array of real rooms which should contain the rooms on the
  //route from start to end

  cache = ([]); // don't need to waste memory on the cache anymore
  return find_route(start, end, places);
}
  
#ifndef OLD
void dest_me(){
  db_close(db);
  destruct(this_object());
}
#endif
