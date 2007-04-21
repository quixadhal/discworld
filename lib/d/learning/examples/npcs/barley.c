/* Barley is a chatty NPC.  If you try to kill him, he'll say
*something else, too.  He can say things, and emote things.
*/


inherit "/obj/monster";


void setup() {

    set_name( "barley" );
    set_short( "Barley" );
    add_property("determinate", "");
    add_alias( ({"npc", "warrior", "man"}) );
    set_long("Barley is a friendly, talkative NPC.  He chats and emotes."
    "  Also, he will occasionally give out quest hints.  If you try and"
    " kill him, he will become more animated.\n"); 
    basic_setup ("human", "warrior", 50);
    set_gender( 1 );
    
        //Educate your NPCs! Give them a language, and make them use it.
        //add() as many as you wish, but set() to only one.
    add_language( "common" );
    set_language( "common" );
    
    //The numbers control the frequency of chats.  The first number is 
    //the probability in 1000 a chat will occur every 2 seconds.  eg, 
    //100 = a one in ten chance of a chat happening every 2 secs, 
    //One chat per 20 seconds.  You will need to tweak figures to make these
   // appear at the right intervals.
   // The second number controls how likely the chat is to be chosen.  The 
   //higher the number, the more common the chat.
    load_chat( 50, ({
                  3, "'I say this quite often.",
                  3, ":does this quite often.",
                  3, "Chats can appear without the NPC name in them.",
                  1, "'I only give out quest hints 10% of the time."
               }) );
               
        //load__a_chats are combat chats, triggered by attacking the NPC
        //The numbers are the same, but it's an idea to make them appear more
        //often, as combat can be quick and furious.
    load_a_chat( 150, ({
                    1, "'Oof!",
                    1, ":is happy to die so you can learn.",
                    1, "You feel Someone is waiting in the wings."}) );

} /* ends setup() */
