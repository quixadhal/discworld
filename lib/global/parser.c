/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: parser.c,v 1.4 2003/02/11 04:53:03 presto Exp $
 * $Log: parser.c,v $
 * Revision 1.4  2003/02/11 04:53:03  presto
 * detabified
 *
 * Revision 1.3  2000/05/12 01:36:46  pinkfish
 * Make it de-expand arrays if it can.
 *
 * Revision 1.2  2000/05/12 00:00:04  pinkfish
 * Fix up a few errors in the parser.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
*/
/* Ok, simple (hopefully) recursive descent parser. */

#include <creator.h>
#include <parser.h>

#define LMASTER "/d/liaison/master"

int pos, force_string;
mixed func;

protected void create() {
  seteuid("Root");
}

mixed *parse_args(string str, string close) {
  mixed *args, *m, *m2;
  object *obs;
  string s1, s2, s3, s4, s5, s6, s7;
  int i;
  mapping map;

  args = ({ });
  while (strlen(str)) {
    while (strlen(str) && str[0] == ' ') str = str[1..<1];
    if (!strlen(str) || str[0..0] == close) return ({ args, str[1..<1] });
    switch (str[0]) {
      case '\'' :
        if (sscanf(str, "'%s'%s", s1, s2) != 2) {
          printf("Unterminated string.\n");
          return 0;
        }
        args += ({ replace_string(s1, "\\n", "\n") });
        str = s2;
        break;
      case '`' :
        if (sscanf(str, "`%s`%s", s1, s2) != 2) {
          printf("Unterminated string.\n");
          return 0;
        }
        args += ({ replace_string(s1, "\\n", "\n") });
        str = s2;
        break;
      case '"' :
        if (sscanf(str, "\"%s\"%s", s1, s2) != 2) {
          printf("Unterminated string.\n");
          return 0;
        }
        args += ({ replace_string(s1, "\\n", "\n") });
        str = s2;
        break;
      case '{' :
        m = parse_args(str[1..<1], "}");
        if (!m) {
          return 0;
        }
        args += ({ m[0] });
        str = m[1];
        break;
      case '[' :
       /* put here to catch a mudfreezing bug */
       if (sscanf(str[1..<1], "%s]%s", s1, s2) != 2) {
          printf("Unmatched [.\n");
          return 0;
        }
        str = str[1..<1];
        map = ([ ]);
        while (1) {
          m = parse_args(str, ":");
          /* Ok...  if we cannot find another : maybe we are at the end? */
          if (!m) {
            while (strlen(str) && str[0] == ' ') {
               str = str[1..<1];
            }
            if (str[0] == ']') {
              break;
            }
          }
          if (!(m2 = parse_args(str, ","))) {
            if (!(m2 = parse_args(str, "]"))) {
              return 0;
            }
            if (sizeof(m[0])) {
              map[m[0][0]] = (sizeof(m2[0])?m2[0][0]:0);
            }
            break;
          }
          if (sizeof(m[0])) {
            map[m[0][0]] = (sizeof(m2[0])?m2[0][0]:0);
          }
        }
        args += ({ map });
        break;
      case '|' :
        if (sscanf(str, "|%s|%s", s1, s2) != 2) {
          printf("Unmatched |\n");
          return 0;
        }
        obs = WIZ_PRESENT->wiz_present(str, this_player());
        if (!sizeof(obs)) {
          args += ({ this_player() });
        } else if (sizeof(obs) == 1) {
          args += ({ obs[0] });
        } else {
          args += ({ obs });
        }
        str = s2;
        break;
      case '0'..'9' :
      case '-' :
        if (sscanf(str, "%d%s", i, str) != 2) {
          printf("Number expected.\n");
          return 0;
        }
        args += ({ i });
        break;
      default :
        s2 = s3 = 0;
        sscanf(str, "%s,%s", s4, s2);
        sscanf(str, "%s"+close+"%s", s5, s3);
        if (sscanf(str, "%s->%s", s6, s7) == 2 && 
            (!s3 || strlen(s5) > strlen(s6)) &&
            (!s2 || strlen(s4) > strlen(s6))) {
          /* Now we do something truely revolting.... */
          while (s7[0] == ' ') s7 = s7[1..<1];
          if (sscanf(s7, "%s(%s", s1, s7) != 2) {
            printf("'(' expected.\nLine left unprocessed %s\n", s7);
            return 0;
          }
          obs = WIZ_PRESENT->wiz_present(s6, this_player());
          if (!sizeof(obs)) {
            printf("The object %s needs to exist.\n", s6);
            return 0;
          }
          m = parse_args(s7, ")");
          if (!m) {
             return 0;
          }
          if (sizeof(m[0]) < 6) {
             m[0] += allocate(6-sizeof(m[0]));
          }
          obs = map_array(obs, "mapped_call", this_object(), s1, m[0]);
          if (sizeof(obs) == 1) {
            args += obs;
          } else {
            args += ({ obs });
          }
          str = m[1];
          break;
        } else if (s2 && s3)
          if (strlen(s4) < strlen(s5)) {
            s1 = ",";
            str = s4;
          } else {
            s1 = close;
            s2 = s3;
            str = s5;
          } else if (s2) {
            s1 = ",";
            str = s4;
          } else if (s3) {
            s1 = close;
            s2 = s3;
            str = s5;
          } else {
            s1 = "";
            s2 = "";
          }
          obs = WIZ_PRESENT->wiz_present(str, this_player());
          if (!sizeof(obs)) {
            if (str[0] >= '0' && str[0] <= '9' || str[0] == '-') {
              sscanf(str, "%d%s", i, str);
              args += ({ i });
            } else 
              args += ({ replace_string(str, "\\n", "\n") });
          } else if (sizeof(obs) == 1)
            args += ({ obs[0] });
          else
            args += ({ obs });
          str = s1+s2;
          break;
    }
    /* Skip rubbish and if we dont have a comma we have finished. */
    while (strlen(str) && str[0] == ' ') {
       str = str[1..<1];
    }
    if (!strlen(str)) {
      return ({ args, str });
    }
    if (str[0..0] == close) {
      return ({ args, str[1..<1] });
    }
    if (str[0] != ',') {
      printf("Parse error reading arguments, ',' or '%s' expected.\n", close);
      printf("Line left unprocessed %s\n", str);
      return 0;
    }
    str = str[1..<1];
  }
  return ({ args, str });
} /* parse_args() */

#ifdef NOPE
void inform_of_call(object ob, mixed *argv) {
  string str;
  int i;

  str = this_object()->query_cap_name() + " calls " + argv[0] + "(";
  for (i=1; i<sizeof(argv); ) {
    str += replace(sprintf("%O", argv[i]), "\n", " ");
    if (++i < sizeof(argv)) str += ",";
  }
/* Arggghhh!  This is annoying me.
 * Same comment here.
  ob->event_inform(this_object(), str + ") on you", "call");
 */
} /* inform_of_call() */
#endif

protected mixed mapped_call(object ob, string func, mixed *argv) {
  /* inform_of_call(ob, argv); */
  return call_other(ob, func, argv ...);
} /* mapped_call() */

/* Free form parse_args code */
protected int parse_frogs(string str) {
  mixed junk;

  /* We are not as such looking for an end thingy of any sort... */
  junk = parse_args(str, ";");
  /* It has already printed an error, so we return 1... */
  if (!junk)
    return 1;
  write("The line "+str+" returns: \n");
  printf("%O\n", junk[0]);
  return 1;
} /* parse_frogs() */

/* Ok, simple (hopefully) recursive descent parser. */

mixed expr();

protected mixed bit4() {
  mixed val1, val2, val3;

  if (pos < sizeof(func)) {
    if (pointerp(func[pos])) {
      return func[pos++][0];
    }
    if (stringp(func[pos])) {
      if (func[pos][0] == '$') {
        /* We want a variable... */
        val1 = (object)this_player()->get_obvar(func[pos][1..<1]);
        pos++;
        return val1;
      }
      if (force_string) {
        force_string = 0;
        return func[pos++];
      }
      val1 = (object *)WIZ_PRESENT->wiz_present(func[pos], this_player());
      if (!sizeof(val1)) {
        return func[pos++];
      }
      pos++;
      if (sizeof(val1) == 1) {
        return val1[0];
      }
      return val1;
    }
    switch (func[pos]) {
      case TOK_OBRAC :
        pos++;
        val1 = expr();
        if (func[pos] != TOK_CBRAC) {
          printf("Mismatched brackets.\n");
        } else {
          pos++;
        }
        break;
      case TOK_SARRAY :
        pos++;
        val1 = ({ });
        while (pos < sizeof(func) && 
            func[pos] != TOK_EARRAY) {
          if (func[pos] == TOK_COMMA) {
             pos++;
          }
          val2 = expr();
          if (func[pos] != TOK_COMMA && 
              func[pos] != TOK_EARRAY) {
            printf("Error processing array.\n");
            return 0;
          }
          val1 += ({ val2 });
        }
        pos++;
        break;
      case TOK_SMAPPING :
        pos++;
        val1 = ([ ]);
        while (pos < sizeof(func) && func[pos] != TOK_EMAPPING) {
          if (func[pos] == TOK_COMMA) pos++;
          val2 = expr();
          if (func[pos] != TOK_COLON) {
            printf("Error processing mapping, expected :.\n");
            return 0;
          }
          pos++;
          val3 = expr();
          if (func[pos] != TOK_EMAPPING && func[pos] != TOK_COMMA) {
            printf("Error processing mapping, expected , or ].\n");
            return 0;
          }
          val1[val2] = val3;
        }
        printf("End of mapping.\n");
        pos++;
        break;
      default :
        /* Anything else is a potential object. */
        printf("Broken parser....\n");
        break;
    }
  }
  return val1;
} /* bit4() */

protected mixed do_function_call(object ob, string name, mixed stuff) {
  string found;
  object shad;

  if (objectp(ob)) {
    shad = ob;
    while ((shad = shadow(shad, 0))) {
      if ((found = function_exists(name, ob)))
        break;
    }
    if (!found && !(found = function_exists(name, ob))) {
      printf("*** function %s not found in %s ***\n",
          name, file_name(ob));
      return 0;
    } else {
      printf("*** function %s on %s found in %s ***\n", name, file_name(ob),
          found);
    }
    if ((ob != this_player()) && ob->query_property("player") &&
        !ob->query_property("no score")) {
        unguarded((: write_file,
                   (LMASTER->query_member(this_player()->query_name()) ?
                    "/d/admin/log/CALL_LIAISONS.log" :
                    "/d/admin/log/CALL_CREATORS.log" ),
                   sprintf("%s: %s (%O) called %s(%s) on %s (%O)\n",
                           ctime(time()),
                           this_player()->query_name(), this_player(),
                           name,
                           implode(stuff - ({0}), ", "), ob->query_name(),
                           ob) :));
        user_event( this_object(), "inform",
              sprintf("%s called %s(%s) on %s",
            this_player()->query_name(), name, implode(stuff - ({0}), ", "),
            ob->query_name()), "calls");
    }
    return call_other(ob, name, stuff[0], stuff[1], stuff[2],
        stuff[3], stuff[4], stuff[5]);
  }
  return 0;
} /* do_function_call() */

protected mixed bit3() {
  mixed *tmp, val1, val2, val3;
  int i;

  val1 = bit4();
  while (pos < sizeof(func)) {
    switch (func[pos]) {
      case TOK_CALL :
        pos++;
        force_string = 1;
        val2 = bit4();
        force_string = 0;
        if (func[pos] != TOK_OBRAC) {
          printf("Open bracket expected.\n");
        } else {
          tmp = ({ });
          pos++;
          while (pos < sizeof(func) && func[pos] != TOK_CBRAC) {
            if (func[pos] == TOK_COMMA) pos++;
            val3 = expr();
            if (func[pos] != TOK_COMMA && func[pos] != TOK_CBRAC) {
              printf("Error in function arguments.\n");
              return 0;
            }
            tmp += ({ val3 });
          }
          pos++;
        }
        if (objectp(val1)) {
          val1 = ({ val1 });
        }
        if (!pointerp(val1)) {
          printf("Array or object expected for function call.\n");
          break;
        }
        if (!stringp(val2)) {
          printf("String expected for the function call name.\n");
          break;
        }
        if (sizeof(tmp) < 6) {
          tmp += allocate(6-sizeof(tmp));
        }
        for (i=0;i<sizeof(val1);i++) {
          val1[i] = do_function_call(val1[i], val2, tmp);
        }
        if (sizeof(val1) == 1) {
          val1 = val1[0];
        }
        break;
      case TOK_SMAPPING :
        /* Array/mapping index... */
        pos++;
        val2 = expr();
        if (func[pos] == TOK_DOTDOT) {
          pos++;
          val3 = expr();
          if (func[pos] != TOK_EMAPPING) {
            printf("Expected closeing ].\n");
            break;
          }
          pos++;
          if (!pointerp(val1)) {
            printf("Can only use the .. syntax on arrays.\n");
            break;
          }
          if (!intp(val2) || !intp(val3)) {
            printf("Indexes must be integers.\n");
            break;
          }
          val1 = val1[val2..val3];
          break;
        }
        if (func[pos] != TOK_EMAPPING) {
          printf("Expected closeing ].\n");
          break;
        }
        pos++;
        if (mapp(val1)) {
          /* Anything is a legal index... */
          val1 = val1[val2];
        } else if (pointerp(val1)) {
          /* Only integers... */
          if (!intp(val2)) {
            printf("Can only use integers as an index on an array.\n");
          } else if (val2 < 0 || val2 >= sizeof(val1)) {
            printf("Index out of bounds.\n");
          } else {
            val1 = val1[val2];
          }
        } else {
          printf("Can only index off arrays or mappings.\n");
        }
        break;
      default :
        return val1;
    }
  }
  return val1;
} /* bit3() */

protected mixed bit2() {
  mixed val1, val2;

  val1 = bit3();
  while (pos < sizeof(func)) {
    switch (func[pos]) {
      case TOK_MULT :
        pos ++;
        val2 = bit3();
        if ((!intp(val1) && !floatp(val1)) || (!intp(val2) && !floatp(val2))) {
          printf("Incompatible types in multiply, both must be integers.\n");
          val1 = 0;
        } else {
          val1 = val1*val2;
        }
        break;
      case TOK_DIV :
        pos ++;
        val2 = bit3();
        if ((!intp(val1) && !floatp(val1)) || (!intp(val2) && !floatp(val2))) {
          printf("Incompatible types in division, both must be integers.\n");
          val1 = 0;
        } else if (!val2) {
          printf("Division by 0 error.\n");
        } else {
          val1 = val1/val2;
        }
        break;
      default :
        return val1;
    }
  }
  return val1;
} /* bit2() */

mixed expr() {
  mixed val1, val2;

  val1 = bit2();
  while (pos < sizeof(func)) {
    switch (func[pos]) {
      case TOK_PLUS :
        pos ++;
        val2 = bit2();
        if (pointerp(val1) && !pointerp(val2)) {
          printf("Incompatible types in addition, array and something "
                 "else.\n");
          val1 = 0;
        } else {
          val1 = val1 + val2;
        }
        break;
      case TOK_MINUS :
        pos ++;
        val2 = bit2();
        if (pointerp(val1) && !pointerp(val2)) {
          printf("Incompatible types in addition, array and something "
                 "else.\n");
          val1 = 0;
        } else {
          val1 = val1 + val2;
        }
        break;
      case TOK_ASSIGN :
        pos++;
        val2 = expr();
        if (pointerp(val1) || mapp(val1)) {
          printf("Cannot use an array or mapping as a variable name.\n");
        } else {
          this_player()->set_obvar(val1, val2);
        }
        val1 = val2;
        break;
      default :
        return val1;
    }
  }
  return val1;
} /* expr() */

void init_expr(string expr) {
  pos = 0;
  force_string = 0;
  func = TOKENISER->tokenise(expr);
}

void finish_expr() {
  func = 0;
}

#ifdef BOOLEAN
/*
 * Actually.  I think I am starting to get carried away here.  So, I will
 * stop...
 */
mixed eval() {
  mixed *tmp, val1, val2;
  int i, j;

  val1 = bit2();
  while (pos < sizeof(func))
    switch(func[pos]) {
      case TOK_EQUAL :
      case TOK_GREATOR :
      case TOK_LESS :
      case TOK_GREATOREQ :
      case TOK_LESSEQ :
    }
} /* eval() */
#endif
