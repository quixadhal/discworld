/* this is to be inherited by effect shadows */

object player;
int id;

attach_to_player(object p, int i)
{
   player = p;
   id = i;
   return shadow(p, 1);
}

void remove_effect_shadow(int i)
{
   if (i == id) { destruct(this_object()); return; }
   player->remove_effect_shadow(i);
}
