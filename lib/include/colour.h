#ifndef __SYS__COLOUR
#define __SYS__COLOUR

#define COL_RED 0
#define COL_PINK 1
#define COL_ORANGE 2
#define COL_YELLOW 3
#define COL_GREEN 4
#define COL_BLUE 5
#define COL_PURPLE 6
#define COL_WHITE 7
#define COL_GREY 8
#define COL_BROWN 9
#define COL_BLACK 10
#define COL_GOLD 11
#define COL_SILVER 12
#define COL_COPPER 13
#define COL_MULTI 14
#define COLOURS ({ "red", "pink", "orange", "yellow", "green", "blue", "purple", "white", "grey", "brown", "black", "gold", "silver", "copper", "multicoloured" })
#define ANSI_COLOURS ({ "%^RED%^", "%^MAGENTA%^", "%^ORANGE%^", "%^YELLOW%^", "%^GREEN%^", "%^BOLD%^%^BLUE%^", "%^MAGENTA%^", "%^BOLD%^%^WHITE%^", "%^CYAN%^", "%^ORANGE%^", "%^BOLD%^%^BLUE%^"  })

#define REDDISH 0
#define PINKISH 1
#define YELLOWISH 3
#define GREENISH 4
#define BLUISH 5
#define PURPLISH 6
#define PALE 7
#define GREYISH 8
#define BROWNISH 9
#define BLACKISH 10
#define OFF 11
#define MUDDY 12
#define WHITISH 13
#define GOLDEN 14
#define LIGHT 15
#define DARK 16
#define VARIEGATED 17
#define SILVERY 18
#define METALLIC 19
#define MODIFIERS ({ "reddish ", "pinkish ", "orange-", "yellowish ", "greenish ", "bluish ", "purplish ", "pale ", "greyish ", "brownish ", "blackish ", "off-", "muddy ", "whitish ", "golden ", "light ", "dark ", "variegated ", "silvery ", "metallic " })


#define USER_COLOUR_LIST ({"newbie", "shout", "tell", "say", "whisper", \
                            "soul", "emote", "remote", "exits", "inventory", \
                           "weather", "group", "shopchat", "emoteall", \
                           "combat"})
#endif /* __SYS__COLOUR */
