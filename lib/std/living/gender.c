int gender;

int query_female() { return gender == 2; }
int query_gender() { return gender; }
int query_male() { return gender == 1; }
int query_neuter() { return !gender; }

string query_gender_string() {
    if (!gender) return "neuter";
    else if (gender == 1) return "male";
    else return "female";
}

string query_gender_title() {
    if (!gender) return "Creature";
    else if (gender == 1) return "Sir";
    else return "Madam";
}
 
string query_objective() {
    if (!gender)
        return "it";
    else if (gender == 1)
        return "him";
    else return "her";
}
 
string query_possessive() {
    if (!gender)
        return "its";
    else if (gender == 1)
        return "his";
    else return "her";
}
 
string query_pronoun() {
    if (!gender)
        return "it";
    else if (gender == 1)
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
}
