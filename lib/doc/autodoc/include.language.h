.DT
language.h
Disk World autodoc help
language.h

.SH Description
.SP 5 5

This is the file that contains the language bits needed for dealing with
languages.  This is for reading and so on too.
.EP
.SP 10 5


Written by Pinkfish

Started in 1992 sometime

.EP

.SH Defines
.SI 3
* %^BOLD%^L_ALWAYS_SPEAK%^RESET%^
.EI
.SP 7 5

Flag to specify that the language is always able to be spoken.

.EP

.SI 3
* %^BOLD%^L_DISTANCE%^RESET%^
.EI
.SP 7 5

Flag to specify that this language can be used over a distance (ie:
tells and shouts).

.EP

.SI 3
* %^BOLD%^L_MAGIC%^RESET%^
.EI
.SP 7 5

Flag to specify that this language is magic language!  We handle these
somewhere differently to a normal language.

.EP

.SI 3
* %^BOLD%^L_SIZED%^RESET%^
.EI
.SP 7 5

Flag to specify that the language is sized.

.EP

.SI 3
* %^BOLD%^L_SPOKEN%^RESET%^
.EI
.SP 7 5

Flag to specify that this language can be spoken.

.EP

.SI 3
* %^BOLD%^L_WRITTEN%^RESET%^
.EI
.SP 7 5

Flag to specify that this language can be written.

.EP

.SI 3
* %^BOLD%^LANGUAGE_HAND%^RESET%^
.EI
.SP 7 5

The language handler.

.EP

.SI 3
* %^BOLD%^LANGUAGE_INHERIT%^RESET%^
.EI
.SP 7 5

The default language inheritable used in all languages so the interface
is consistent.

.EP

.SI 3
* %^BOLD%^LANGUAGE_SKILL_START%^RESET%^
.EI
.SP 7 5

The start part of the languages skill tree.

.EP

.SI 3
* %^BOLD%^MESSAGE_OK%^RESET%^
.EI
.SP 7 5
The whole message was just fine.  This is used by the speech commands. 
.EP

.SI 3
* %^BOLD%^NO_MESSAGE%^RESET%^
.EI
.SP 7 5
There is no text to say.  THis is used by the speech commands. 
.EP

.SI 3
* %^BOLD%^NOT_DISTANCE%^RESET%^
.EI
.SP 7 5
The specific language is not allowed to be used at a distance.  This is used by the speech commands. 
.EP

.SI 3
* %^BOLD%^NOT_KNOWN%^RESET%^
.EI
.SP 7 5
The specific language is not known by the speaker.  This is used by the speech commands. 
.EP

.SI 3
* %^BOLD%^NOT_SPOKEN%^RESET%^
.EI
.SP 7 5
The specific language is not spoken.  This is used by the speech commands. 
.EP

.SI 3
* %^BOLD%^READ_LANG%^RESET%^
.EI
.SP 7 5

The third element of the read array is the language the message is written
in.  The language must exist and must be non-zero.

.EP

.SI 3
* %^BOLD%^READ_MESS%^RESET%^
.EI
.SP 7 5

The first element of the read array, the message itself.

.EP

.SI 3
* %^BOLD%^READ_SIZE%^RESET%^
.EI
.SP 7 5

The fourth element of the read array is the size of the text.  This is used
to make sure not too much stuff can be printed on any one page.

.EP

.SI 3
* %^BOLD%^READ_STR%^RESET%^
.EI
.SP 7 5

The first element of the read array, the message itself.

.EP

.SI 3
* %^BOLD%^READ_TYPE%^RESET%^
.EI
.SP 7 5

The second element of the read array, this is the type.  The type is
something like 'red splotched ink' or whatever.  It can be 0.

.EP

.SI 3
* %^BOLD%^SAY_TYPE%^RESET%^
.EI
.SP 7 5

This is used to tag the transformation type as a say.
This is used inside the language inheritable to tag what type of
transformation is being done on the text. 

.EP

.SI 3
* %^BOLD%^SHOUT_TYPE%^RESET%^
.EI
.SP 7 5

This is used to tag the transformation type as a shout.
This is used inside the language inheritable to tag what type of
transformation is being done on the text. 

.EP

.SI 3
* %^BOLD%^SPOKEN_SKILL%^RESET%^
.EI
.SP 7 5

The name of the spoken skill bit.

.EP

.SI 3
* %^BOLD%^TELL_TYPE%^RESET%^
.EI
.SP 7 5

This is used to tag the transformation type as a tell.
This is used inside the language inheritable to tag what type of
transformation is being done on the text. 

.EP

.SI 3
* %^BOLD%^WHISPER_TYPE%^RESET%^
.EI
.SP 7 5

This is used to tag the transformation type as a whisper.
This is used inside the language inheritable to tag what type of
transformation is being done on the text. 

.EP

.SI 3
* %^BOLD%^WRITTEN_SKILL%^RESET%^
.EI
.SP 7 5

The name of the written skill bit.

.EP

