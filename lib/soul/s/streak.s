(streak
   (pattern "<indirect:any-living>")
   (pattern "<string>")
   (pattern "<string> <indirect:any-living>")
   (pattern "<indirect:any-living> <string>")
   (single
      (no-arguments
       (self "You shed all your clothes and streak around the room naked.")
       (rest "$mcname$ shed$s$ all $mposs$ clothes and streaks around the room naked.")
      )
      (arguments
      (arguments laughing,screaming,in a frenzy,with naughty bits flapping,
        panting,frightening everyone,with your frog,backwards,
        doing a dance to the great frog in the sky,#)
 (self "You shed all your clothes and streak around the room $arg$.")
 (rest "$mcname$ shed$s$ all $mposs$ clothes and streaks around the room $arg$.")
      )
   )
   (targeted
      (no-arguments
         (self   "You shed your clothes and streak by $hcname$.")
         (target "$mcname$ shed$s$ $mposs$ clothes and streaks by you.")
         (rest   "$mcname$ shed$s$ $mposs$ clothes and streaks by $hcname$.")
      )
 (arguments
      (arguments laughing, screaming, in a frenzy, with naughty bits flapping,
        panting, frightening everyone,with your frog,backwards,
        doing a dance to the great frog in the sky,#)
      (self   "You shed your clothes and streak by $hcname$ $arg$.")
      (target "$mcname$ shed$s$ $mposs$ clothes and streaks by you $arg$.")
      (rest   "$mcname$ shed$s$ $mposs$ clothes and streaks by $hcname$ $arg$.")
      )
   )
)
/* Thanks to Iam for the idea of this one. */
