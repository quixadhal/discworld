/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tokeniser.c,v 1.3 2000/05/11 23:54:51 pinkfish Exp $
 * $Log: tokeniser.c,v $
 * Revision 1.3  2000/05/11 23:54:51  pinkfish
 * Make sure it detects floats correctly.
 *
 * Revision 1.2  1998/04/03 19:46:40  pinkfish
 * Add floating point support.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Does tokenising the the parser used by the mudlib when someone uses
 * the ';' command.
 * @author Pinkfish
 */
/* Ok, simple (hopefully) recursive descent parser. */
#include "tokenise.h"

mapping token;

void create() {
  token = ([
           '+' : TOK_PLUS,
           '-' : TOK_MINUS,
           '*' : TOK_MULT,
           '/' : TOK_DIV,
           '(' : TOK_OBRAC,
           ')' : TOK_CBRAC,
           '{' : TOK_SARRAY,
           '}' : TOK_EARRAY,
           '[' : TOK_SMAPPING,
           ']' : TOK_EMAPPING,
           ':' : TOK_COLON,
           ',' : TOK_COMMA,
           '.' : TOK_DOT,
           '\t' : TOK_TAB,
           '=' : TOK_ASSIGN,
#ifdef BOOLEAN
           '>' : TOK_GREATOR,
           '<' : TOK_LESS,
           "==" : TOK_EQUAL,
           ">=" : TOK_GREATOREQ,
           "<=" : TOK_LESSEQUAL,
#endif
           "->" : TOK_CALL,
           ".." : TOK_DOTDOT,
           ]);
} /* create() */

/**
 * Tokenise, we rip out strings and make them as seperate enties
 * Otherwise things inside strings will get processed elsewhere
 * which could be bad...
 * @param inp the input string
 * @return the tokenised string
 */
mixed tokenise(string inp) {
  string *bits, *cur;
  int pos, num;
  int start_pos;

  cur = ({ });
  while (pos < strlen(inp))
    switch (inp[pos++]) {
      case ' ' :
      case '\t' :
        break;
      case '\'' :
      case '"' :
      case '`' :
        bits = explode("#"+inp[pos-1..]+"#", inp[pos-1..pos-1]);
        if (!bits || sizeof(bits) < 2) {
          write("Error processing the string.\n");
          return 0;
        }
        pos += strlen(bits[1])+1;
        cur += ({ ({ bits[1] }) });
        break;
      case '.' :
      case '0' :
      case '1' :
      case '2' :
      case '3' :
      case '4' :
      case '5' :
      case '6' :
      case '7' :
      case '8' :
      case '9' :
        start_pos = pos;
        while ((inp[pos] >= '0' && inp[pos] <= '9') || (inp[pos] == '.')) {
          pos++;
        }
        if (strsrch(inp[start_pos..pos], ".") != -1) {
          sscanf(inp[start_pos-1..pos], "%f", num);
        } else {
          sscanf(inp[start_pos-1..pos], "%d", num);
        }
        cur += ({ ({ num }) });
        break;
      case '+' :
      case '*' :
      case ':' :
      case ',' :
      case '{' :
      case '}' :
      case '(' :
      case ')' :
      case '[' :
      case ']' :
        cur += ({ token[inp[pos-1]] });
        break;
      case '>' :
      case '<' :
      case '=' :
      case '-' :
      case '/' :
      case '.' :
        if (inp[pos-1] != '/') {
          if ((num = token[inp[pos-1..pos]])) {
            cur += ({ num });
            pos++;
            break;
          }
          num = token[inp[pos-1]];
          if (num) {
            cur += ({ num });
            break;
          }
        } else {
          if (sizeof(cur) && !stringp(cur[sizeof(cur)-1])) {
            cur += ({ token[inp[pos-1]] });
            break;
          }
        }
      default :
        num = pos-1;
        while (pos < strlen(inp) && (!token[inp[pos++]] || inp[pos-1] == '/'));
        if (pos == num+1)
          break;
        if (token[inp[pos-1]])
          pos--;
        cur += ({ inp[num..pos-1] });
        break;
    }
  return cur;
} /* tokenise() */
