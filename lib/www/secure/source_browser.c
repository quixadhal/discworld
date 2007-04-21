//Source browser by Wodan
#include <http.h>
#include <nroff.h>
#include <assert.h>
#include <player_handler.h>

string this_file;
string *simuls;
string *defs;
mixed *funcs;
string last_file;
object file_ob;
int use_defs = 0;

string function_tag(string file, string exp, string match, object ob);

void create(){
  simuls = functions((find_object("/secure/simul_efun")));
  this_file = __FILE__[strsrch(__FILE__, "/", - 1) + 1..];
}

string get_inc_path(string file){
  string ret;
  while(file[0] == ' ')
    file = file[1..];
  
  if(file[0] == '<'){
    ret = "/include/";
    file = file[1..];
  } else if(file[0] == '"'){
    if(file[1] == '/')
      ret = "";
    else 
      ret = "/" + implode(explode(last_file, "/")[0..<2], "/") + "/";
    file = file[1..];
  } else error(file);

  while(file[<1] == ' ')
    file = file[0..<2];
  file = file[0..<2];
  if(file_size(ret + file) < 0)
    return get_inc_path(sprintf("<%s>", file));
  return ret + file;
}

string expand_macros(string ins){
  int i, changed;
  int *allowed = ({' ','\t','+','-',',','(','\"','['});
  do{
    changed = 0;
    for(i=0; i<sizeof(defs); i+=2){
      int off;
      if((off = strsrch(ins, defs[i])) != -1){
        if(!off || member_array(ins[off-1], allowed) >= 0){
          ins = replace_string(ins, defs[i], defs[i+1]);
          changed = 1;
        }
      }
    }
  } while(changed);
  return ins;
}

string strip_junk(string stuff){
  stuff = replace(stuff, ({" ", "", "+", ""}));
  return replace_string(stuff, "\"\"", "");
}

void defines(){
  string *lines = ({"#include <all_inc.h>"}) + regexp(explode(read_file(last_file), "\n"),
                         "^#[ ]*(include|define)");
  string line;
  string infile = last_file;

  if(defs)
    lines = lines[1..];

  foreach(line in lines){
    int off;
    string tmp, tmp2;
    if(off = strsrch(line, "include") != -1){
      string file;
      tmp = line[off+7..];
      file = get_inc_path(tmp);
      last_file = file;
      defines();
      last_file = infile;
    } else {
      int i = 0, j, nest = 0, macro = 0;
      off = strsrch(line, "define");
      tmp = line[off+6..];
      while(tmp[0] == ' ')
        tmp = tmp[1..];
      while((tmp[++i] != ' ' || nest) && i <= (sizeof(tmp) - 1)){
        if(tmp[i] == '('){
          nest++;
          macro = 1;
        } else if(tmp[i] == ')')
          nest--;
      }
      j = i;
      if(j != (sizeof(tmp))){
        while(tmp[++j] == ' ' && j <= (sizeof(tmp) - 1));
      }

      tmp2 = tmp[j..];
      tmp = tmp[0..i-1];

      if(!macro)
        tmp2 = strip_junk(expand_macros(tmp2));
      
      defs += ({tmp, tmp2});
    }
  }
}

string get_function(string func){
  mixed tmp;
  int i=0, j=0;

  tmp = reg_assoc(read_file(last_file),
                  ({"\n[a-z_ \t]*[\\* ][ \t]*" + func +
                     "[ \t]*\\([a-zA-Z0-9, _\\*\n\t]*(...)?[ \t]*\\)[ \t\n]*{"
                      , "\n}"})
                  , ({1, 2}));

  j = member_array(1, tmp[1]) + 1;

  assert(j);

  while(tmp[0][j][i] != '\n'){
    if(tmp[0][j][i] == '}')
      return sprintf("%s%s", tmp[0][j - 1], tmp[0][j][0..i]);
    i++;
  }
  return sprintf("%s%s%s", tmp[0][j - 1], tmp[0][j], tmp[0][j + 1]);
}

string function_tag(string file, string exp, string match, object ob){
  string html, infile, func;

  assert(stringp(file));
  assert(stringp(exp));
  assert(stringp(match));
  assert(objectp(ob));

  func = match[0..<2];

  if(member_array(func, simuls) >= 0){
    file = "/secure/simul_efun";
    ob = find_object(file);
    exp = "";
  }
  
  if((infile = (function_exists(func, ob, 1) + ".c")) == file){
    html = sprintf("<B><A HREF=\"%s?file=%s&funcs=%s/%s#%s\">%s</A></B>",
                   this_file, file, exp, func, func, func);
  } else {
    html = (infile == "0.c")?func:
      (sprintf("<B><A HREF=\"%s?file=%s&funcs=%s#%s\">%s</A></B>",
                   this_file, infile, func, func, func));
  }
  return sprintf("%s(", html);
}

string inherited_function_tag(string file, string match, object ob){
  string html, infile, func, filep;

  assert(stringp(file));
  assert(stringp(match));
  assert(objectp(ob));

  sscanf(match[0..<2], "%s::%s", filep, func);

  if(sizeof(filep))
    filep +=".c";
  foreach(file in inherit_list(ob)){
    string tmp = function_exists(func, (ob = load_object(file)), 1);
    if(tmp && file[<sizeof(filep)..] == filep)
      break;
  }
  
  infile = (function_exists(func, ob, 1) + ".c");

  html = (infile == "0.c")?match[0..<2]:
    (sprintf("<B><A HREF=\"%s?file=%s&funcs=%s#%s\">%s</A></B>",
             this_file, infile, func, func, match[0..<2]));
  return sprintf("%s(", html);
}

string parse_function(string funct, string *expand){
  string ret;
  int i;
  mixed *tmp; 

  if(!sizeof(defs)){
    defs = ({});
    if(use_defs)
      defines();
  }

  ret = funct;

  if(use_defs)
    while(ret != (funct = expand_macros(funct)))
      ret = funct;
          
  tmp = reg_assoc(funct, ({"[a-zA-Z_0-9][a-zA-Z_0-9]*\\(",
                           "[a-z_]*::[a-zA-Z_0-9][a-zA-Z_0-9]*\\("}),
                  ({(:function_tag($(last_file), $(implode(expand, "/")), $1,
                                   $(file_ob)):),
                    (:inherited_function_tag($(last_file), $1, $(file_ob)):)}),
                  (:HTTPD->escape_html($1):));

  ret = "";
  for(i = 0; i < sizeof(tmp[1]); i++){
    ret += evaluate(tmp[1][i], tmp[0][i]);
  }
  return ret;
}

string make_func(string *expand, int can_read, string done, mixed *func){
  string tmp;
  reset_eval_cost();
  done += "<a name=\"" + func[0] + "\"><a>";
  
  if(member_array(func[0], expand) == -1){
    string proto = func[2] + func[0];
    return done + sprintf("<B><A HREF=\"%s?file=%s&funcs=%s/%s#%s\""
                          ">%s(%s);</A></B><br>", 
                          this_file, last_file, implode(expand, "/"), func[0],
                          func[0],  proto, implode(func[3..], ","));
  }

  if(tmp = "/obj/handlers/autodoc/autodoc_handler"->query_help_on(last_file,
                                                                  func[0])){
    string help;
    if (tmp[<3..] == ".o") {
      help = NROFF_HAND->html_file(tmp);
    } else {
      help = read_file(tmp, 0, 2);
      if (help[0] == '.') {
        NROFF_HAND->create_nroff(tmp, "/tmp/00000bing");
        help = NROFF_HAND->html_file("/tmp/00000bing", tmp);
        rm("/tmp/00000bing");
      }
    }
    done += help;
  }
  if(can_read) return done + sprintf("<pre>%s</pre><br>",
           parse_function(get_function(func[0]), expand));
  return done + sprintf("<br>Can't read function: %s, no read permission to: %s<br>",
                 func[0], last_file);
}

string *inherits_list(string ob){
  mixed *tmp;

  tmp = objects((:!clonep($1) && inherits($(ob), $1) &&
                 (member_array($(ob), inherit_list($1)) != -1):));
  tmp = map(tmp, (:file_name($1) + ".c":));
  tmp = filter(tmp, (: $1[0..1] != "/w" :));

  if(sizeof(tmp) > 10){
    tmp = sort_array(tmp, (: reset_eval_cost(), $1[0..1] == "/d"? 1 : -1 :));
    return tmp[0..9];
  }
  return tmp;
}

string make_inherit_list(string done, string file){
  return sprintf("%s<B><A HREF=\"%s?file=%s\">%s</A></B><br> ", done,
                 this_file, file, file);
}

mixed *filter_funcs(mixed *ret, mixed *item){
  mixed *present = map(ret, (:$1[0]:));

  if(member_array(item[0], present) == -1)
    return ret + ({item});

  return ret;
}

string www_request(string, mapping args, class http_request req){
  string *expand;
  string ret;
  
  if(!args["funcs"])
    args["funcs"] = "";
  expand = explode(args["funcs"], "/");
  
  use_defs = args["defs"]?1:0;
  
  if(sizeof(args["file"])>2){
    if(args["file"][<2..] != ".c")
      args["file"] += ".c";
    if(file_size(args["file"]) > 0){
      catch{
        int can_read;
        
        if(last_file != args["file"] || !file_ob){
          last_file = args["file"];
          file_ob = 0;
          file_ob = load_object(args["file"]);
          funcs = 0;
          defs = 0;
        }
        
        if(!req->user) {
          ret = "Try help coffee_table";
          throw("no user");
        }
        
        if(!PLAYER_HANDLER->test_creator(req->user)){
          ret = "You are not a creator!";
          throw("not a cre");
        }
        
        can_read = unguarded((: master()->valid_read($(last_file), 
                                                     $(req->user), "cat"):));
        if(!funcs){
          funcs = filter(functions(file_ob, 1),
                         (:(function_exists($1[0], $(file_ob), 1) + ".c") ==
                          $(last_file):));
          
          funcs = implode(funcs, (:filter_funcs:), ({}));
          
          funcs = sort_array(funcs, 1);
        }

        if(args["funcs"] == "all")
          expand = map(funcs, (:$1[0]:));

        ret = sprintf(@END
<html>
<head>
<title> Browsing %s</title>
</head>
<body bgcolor="#ffffff" TEXT="#000030" LINK="#4a529c" VLINK="#b57339">
<font face="arial,helvetica">
<IMG ALIGN=LEFT SRC="/pics/dw3.gif"><h2>Source Browser</h2><h3><i>Froginess for all.</I></h3><BR clear=left><HR>
<h3>File: %s</h3>
<dl>
<dt><strong>Inherits:</strong>
<dd>%s<BR>
<dt><strong>Inherited by:</strong>
<dd>%s<BR>
<dt><strong>Functions:</strong><b><a href="%s?file=%s">Fold all</a></b>
    <b><a href="%s?file=%s&funcs=all">Expand all</a></b>
    <b><a href="%s?file=%s&funcs=%s&defs=1">Expand defines</a></b><BR><BR>

END
        , last_file, last_file
        , implode(inherit_list(file_ob), (:make_inherit_list:), "<br>")
        , implode(inherits_list(last_file), (:make_inherit_list:), "<br>")
        , this_file, last_file, this_file, last_file, this_file, last_file
        , args["funcs"]) +
          unguarded((: implode($(funcs), (:make_func, $(expand), 
                                 $(can_read):), ""):)) + "<BR>" + sprintf(@END
</dl>
<hr><form action="%s" METHOD=GET>
<b>File:</b> <input type="text" name="file" size=62><p>
<p>
<input type="submit" value="Get file">
<input type="reset" value="Reset">
</form>
<hr>
%s<br>
<i><a href="mailto:wodan.discworld@wodan.net">wodan.discworld@wodan.net</font></a></i><br>
</body>
</html>
END
        , this_file
        , "/www/footer"->www_function("footer", ([ ]), req, 0, 1));

      };
    }
  }
  if(ret)
    return ret;
  return sprintf("No such (loadable) file, or no file selected."
    "<hr><form action=\"%s\" METHOD=GET>"
    "<b>File:</b> <input type=\"text\" name=\"file\" size=62><p>"
    "<p>"
    "<input type=\"submit\" value=\"Get file\">"
    "<input type=\"reset\" value=\"Reset\">"
    "</form><br><b><a HREF=\"file_browser.c?"
    "action=/secure/%s,check_file,"
    "%s,file\">Browse</a>", allocate(3, this_file) ...);
}

int check_file(string file){
  if(sizeof(file) < 3)
    return 0;
  return file[<2..] == ".c";
}

