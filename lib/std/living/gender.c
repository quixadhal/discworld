/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gender.c,v 1.4 2002/03/20 16:59:58 gruper Exp $
 * $Log: gender.c,v $
 * Revision 1.4  2002/03/20 16:59:58  gruper
 * Added query_real_gender() to take care of witches' guild
 * joining issues on women's day.
 *
 * Revision 1.3  2001/03/08 13:36:33  taffyd
 * Added query_womens_day()... where did it go?
 *
 * Revision 1.2  1999/01/13 00:41:41  arby
 * Added query_gender_name to return man, woman, or neuter.
 *
 * Revision 1.1  1998/01/06 04:29:08  ceres
 * Initial revision
 * 
*/
int gender;
nosave int womens_day;

int query_female() { return (gender == 2) || womens_day; }
int query_gender() { return (womens_day?2:gender); }
int query_male() { return gender == 1 && !womens_day; }
int query_neuter() { return !gender && !womens_day; }

string query_gender_string() {
    if (!gender && !womens_day) return "neuter";
    else if (gender == 1 && !womens_day) return "male";
    else return "female";
}

string query_gender_title() {
    if (!gender && !womens_day) return "creature";
    else if (gender == 1 && !womens_day) return "sir";
    else return "madam";
}
 
string query_gender_name() {
    if (!gender && !womens_day) return "neuter";
    else if (gender == 1 && !womens_day) return "man";
    else return "woman";
}

string query_objective() {
    if (!gender && !womens_day)
        return "it";
    else if (gender == 1 && !womens_day)
        return "him";
    else return "her";
}
 
string query_possessive() {
    if (!gender && !womens_day)
        return "its";
    else if (gender == 1 && !womens_day)
        return "his";
    else return "her";
}
 
string query_pronoun() {
    if (!gender && !womens_day)
        return "it";
    else if (gender == 1 && !womens_day)
        return "he";
    else return "she";
}

void set_gender(mixed arg) {
  if(stringp(arg)) {
    if(arg == "male")
      gender = 1;
    else if(arg == "female")
      gender = 2;
    else
      gender = 0;
  } else {
    gender = arg;
  }
} /* set_gender() */

/* Can only set ourselves on or off on womens day */
/* Bollocks. */
void set_womens_day(int bing) {
/*
  if (previous_object() != this_object())
    return ;
*/
  womens_day = bing;
} /* set_womens_day() */

int query_womens_day() { 
    return womens_day;
} /* query_womens_day() */

/* This function always returns the real gender, regardless of,
 * ah, pink bow day. */
nomask int query_real_gender() {
   return gender;
} /* query_real_gender() */
