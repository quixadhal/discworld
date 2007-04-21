/** Inherit for one-armed bandits.
 * Refined from the original object by Jobe.
 * Wirble, June 4th 2002
 */

#include <money.h>


int do_slot();
int do_pull_lever();
int do_read_dial();

int set_coin_type( string coin );
int set_chances( int c1, int c2, int c3, int c4, int c5 );
int set_prizes( int p1, int p2, int p3, int p4, int p5, int jackpot );

int* query_prizes();
int* query_chances();
string query_coin_type();

void rollem( object player );
string my_short();

private string *animal = ({"albatross", "frog", "hippo", "turtle", "cabbage"});

private string *adject = ({"A crazy", "A dancing", "A sad", "A floopy", "A bemused",
    "A sarcastic", "A quizzical", "A startled", "A friendly", "A mindless"});

private string _coin = "Ankh-Morpork pence", _coin_short = "pence",
    _coin_pl = "Ankh-Morpork pence";
private int _busy = 0;
private int* _chances = ({1,1,1,1,1});
private int* _prizes = ({2,2,2,2,2,3});


/** @ignore */
void init(){
    this_player()->add_command( "slot", this_object(),
        _coin_short + " into <direct:object>", (: do_slot() :) );
    this_player()->add_command( "pull", this_object(),
        "lever on <direct:object>", (: do_pull_lever() :) );
    this_player()->add_command( "read", this_object(),
        "dial on <direct:object>", (: do_read_dial() :) );
} /*void init() */


/** This sets the type of coin the bandit accepts.  It has to be of valid
 * type as registed in the money handler.
 * @param coin the name of the coin
 * @return 1 on success, otherwise 0
 */
int set_coin_type( string coin ) {
    if( member_array( coin, MONEY_HAND->query_valid_types() ) == -1 )
        return 0;
    _coin = coin;
    _coin_short = MONEY_HAND->query_aliases_for( coin )[0];
    _coin_pl = MONEY_HAND->query_main_plural_for( coin );
    return 1;
} /* int set_coin_type() */


/** Sets the chances for winning.  Each value represents the chance for one
 * type of picture to show up.
 *
 * 5, 4, 3, 2, 1 would mean: 33.3% chance for pic 1, 26.6% for pic 2,
 * 20% for pic 3, 13.3% for pic 4 and 6.6% for pic 5.
 * To win, all three images must be the same pic. To win the jackpot, there has
 * to be another match of 10% per image.
 * @param c1 Chance for albatross
 * @param c2 Chance for frog
 * @param c3 Chance for hippo
 * @param c4 Chance for turtle
 * @param c5 Chance for cabbage
 * @return 1 on success, otherwise 0
 */
int set_chances( int c1, int c2, int c3, int c4, int c5 ) {
    if( !( c1 > 0 && c2 > 0 && c3 > 0 && c4 > 0 && c5 > 0 ) )
        return 0;
    _chances = ({ c1, ( c1 + c2 ), ( c1 + c2 + c3 ), ( c1 + c2 + c3 + c4 ),
        ( c1 + c2 + c3 + c4 + c5 ) });
    return 1;
} /* int set_chances() */


/** Number of coins (as per defined type) won by scoring a triple or the
 * jackpot. Minimum 2.
 * @param p1 Number of coins for 3x albatross
 * @param p2 Number of coins for 3x frog
 * @param p3 Number of coins for 3x hippo
 * @param p4 Number of coins for 3x turtle
 * @param p5 Number of coins for 3x cabbage
 * @param jackpot Number of coins for jackpot (higher than any of the others)
 * @return 1 on success, otherwise 0
 */
int set_prizes( int p1, int p2, int p3, int p4, int p5, int jackpot ) {
    if( !( p1 > 1 && p2 > 1 && p3 > 1 && p4 > 1 && p5 > 1 && jackpot > 1 ) ||
        ( jackpot <= p1 || jackpot <= p2 || jackpot <= p3 ||
        jackpot <= p4 || jackpot <= p5 ) )
        return 0;
    _prizes = ({p1, p2, p3, p4, p5, jackpot});
    return 1;
} /* int set_prizes() */


/** @ignore */
int do_slot() {
    if( member_array( _coin, this_player()->query_money_array() ) == -1 ) {
        this_player()->add_failed_mess( this_object(),
            "You need " + add_a( _coin ) + " to play.\n", ({ }) );
        return 0;
    }
    if( _busy == 1 ) {
        this_player()->add_failed_mess( this_object(),
            "There is already a coin inserted.\n", ({ }) );
        return 0;
    }
    if( _busy == 2 ) {
        this_player()->add_failed_mess( this_object(),
            "The wheels of $D are still spinning.\n", ({ }) );
        return 0;
    }
    this_player()->adjust_money( -1, _coin );
    this_object()->adjust_money( 1 );
    _busy = 1;
    this_player()->add_succeeded_mess( this_object(),
        "$N insert$s " + add_a( _coin_short ) + " into $D.\n", ({ }) );
    return 1;
} /* int do_slot() */


/** @ignore */
int do_pull_lever() {
    if( _busy == 0 ) {
        this_player()->add_failed_mess( this_object(),
            "You must put a coin in the slot before you can play.\n", ({ }) );
        return 0;
    }
    if( _busy == 2 ) {
        this_player()->add_failed_mess( this_object(),
            "The wheels of $D are already spinning.\n", ({ }) );
        return 0;
    }
    this_player()->add_succeeded_mess( this_object(),
        ({"You pull the lever.  The pictures behind the windows blur... one by "
             "one they are replaced by a new picture that appears with a sharp "
             "click.\n",
        "$N pull$s the lever on $D and the machine whirs softly.  The sound is "
        "cut short by three sharp clicks.\n"
        }), ({ }) );
    call_out( "rollem", 0, this_player() );
    _busy = 2;
    return 1;
} /* int do_pull_lever() */


/** @ignore */
void rollem( object player ) {
    int a1, a2, a3, c;
    int adj1 = random( 10 ),
        adj2 = random( 10 ),
        adj3 = random( 10 );

    c = random( _chances[4] );
    if( c < _chances[0] )
        a1 = 0;
    else if( c < _chances[1] )
        a1 = 1;
    else if( c < _chances[2] )
        a1 = 2;
    else if( c < _chances[3] )
        a1 = 3;
    else
        a1 = 4;

    c = random( _chances[4] );
    if( c < _chances[0] )
        a2 = 0;
    else if( c < _chances[1] )
        a2 = 1;
    else if( c < _chances[2] )
        a2 = 2;
    else if( c < _chances[3] )
        a2 = 3;
    else
        a2 = 4;

    c = random( _chances[4] );
    if( c < _chances[0] )
        a3 = 0;
    else if( c < _chances[1] )
        a3 = 1;
    else if( c < _chances[2] )
        a3 = 2;
    else if( c < _chances[3] )
        a3 = 3;
    else
        a3 = 4;

    tell_object( player,
        "The windows now show these pictures :\n\n"
         "        " + adject[ adj1 ] + " " + animal[ a1 ] +
         "    " + adject[ adj2 ] + " " + animal[ a2 ] +
         "    " + adject[ adj3 ] + " "+animal[ a3 ] + "\n\n" );
    if( a1 == a2 && a1 == a3 ) {
        if( adj1 == adj2 && adj1 == adj3 ) {
            tell_object( player,
                "WOW!!  A triple " + adject[ adj1 ] + " " + animal[ a1 ] +
                "!!  You really hit the jackpot this time!\n\n"
                "Whistles and bells start sounding like crazy, the whole " +
                this_object()->short() +  " glows with flashing lights.  There "
                "is a loud tingling as " + _prizes[5] + " " +
                MONEY_HAND->query_plural_for( _coin ) + " drop into the tray "
                "of " + my_short() + ".  Slightly stunned you quickly scoop "
                "it up and cackle with delight.\n" );
            tell_room( environment( this_object() ),
                "Whistles and bells start sounding and " + my_short() +
                " starts to flash with coloured lights.  Someone must have "
                "won the jackpot...lucky buggers.  As you watch some coins "
                "drop into the tray at the bottom of " + my_short() + ", " +
                player->one_short() + " snatches it up and cackles with "
                "delight.\n", ({player}) );
            player->adjust_money( _prizes[5] ,_coin );
            this_object()->adjust_money( -(_prizes[ 5 ]) );
            _busy = 0;
            return;
        }

        tell_object( player,
            "Congratulations!  You scored a triple " + animal[ a1 ] + "!\n" );
        player->adjust_money( _prizes[ a1 ], _coin );
        tell_object( player,
            _prizes[ a1 ]+ " " + MONEY_HAND->query_plural_for( _coin ) +
            " fall into the tray at the bottom of " + my_short() + ".  You "
            "quickly scoop them up before anyone else can snaffle them.\n" );
        tell_room( environment( this_object() ),
            player->one_short() + " deftly scoops some coins from the tray at "
            "the bottom of " + my_short() + ".\n", ({player}) );
        this_object()->adjust_money( -(_prizes[ a1 ]) );
        _busy = 0;
        return;
    }
    tell_object( player,
        "You didn't win.  Why not have another go and attempt to recoup your "
        "losses?\n" );
    _busy = 0;
}/*void rollem() */


/** Returns the chances for each image in the order: albatross, frog, hippo,
 * turtle, cabbage.
 * @return an array with the chances
 */
int* query_chances() {
    int* tmp = ({ });
    tmp += ({ _chances[0] });
    tmp += ({ _chances[1] - _chances[0] });
    tmp += ({ _chances[2] - _chances[1] });
    tmp += ({ _chances[3] - _chances[2] });
    tmp += ({ _chances[4] - _chances[3] });
    return tmp;
} /* int* query_chances() */


/** This function returns the number of coins for each image.  The elements are
 * in the order: albatross, frog, hippo, turtle, cabbage, jackpot.
 * @return the array of numbers
 */
int* query_prizes() {
    return _prizes;
} /* int* query_prizes() */


/** Returns the coin-type for use with the bandit.
 * @return the name of the coin
 */
string query_coin_type() {
    return _coin;
} /* string query_coin_type() */


/** @ignore */
string my_short() {
    return this_object()->the_short();
} /* string my_short() */


/** @ignore */
int do_read_dial() {
    string tmp =
        "Stake        : 1 " + _coin_pl + "\n"
        "3x Albatross : " + _prizes[0] + " " + _coin_pl + "\n"
        "3x Frog      : " + _prizes[1] + " " + _coin_pl + "\n"
        "3x Hippo     : " + _prizes[2] + " " + _coin_pl + "\n"
        "3x Turtle    : " + _prizes[3] + " " + _coin_pl + "\n"
        "3x Cabbage   : " + _prizes[4] + " " + _coin_pl + "\n"
        "JACKPOT      : " + _prizes[5] + " " + _coin_pl + "\n";
    this_player()->add_succeeded_mess( this_object(),
        ({tmp, "$N read$s the dial on $D.\n"}), ({ }) );
    return 1;
} /* int do_read_dial() */


