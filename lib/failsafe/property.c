inherit "/failsafe/simul_efun";
/* /basic/property */
 
static string *property;

void create() {
  property = ({ });
}
 
void set_property(string *array) {
    if (!pointerp(array)) return;   /* use add_property */
    property = array;
    }
 
void add_property(mixed var) {
    if (!pointerp(var))
        property += ({ var });
    else  /* is there anything I have missed here ? */
        property += var;
    }

int remove_property(string str) {
int i;
    if (!stringp(str) || !property || !str) return 0;
    i = member_array(str, property);
    if (i<0) return 0;
    property = delete(property, i, 1);
    return 1;
    }
 
mixed query_property(string str) {
int i;
    if (!property || !str)
        return property;
    if (member_array(str, property) > -1) return 1;
    return 0;
    }
 
