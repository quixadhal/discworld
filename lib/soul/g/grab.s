(grab
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You grab $hcname$.")
      (target "$mcname$ grab$s$ you.")
      (rest   "$mcname$ grab$s$ $hcname$.")
    )
    (arguments 
      (arguments arm, hand, foot, attention, fika, essentials, #)
      (self   "You grab $mhcname$ $arg$.")
      (target "$mcname$ grab$s$ your $arg$.")
      (rest   "$mcname$ grab$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments suddenly, unexpectedly, forcefully, roughly, by the collar)
      (self   "You grab $hcname$ $arg$.")
      (target "$mcname$ grab$s$ you $arg$.")
      (rest   "$mcname$ grab$s$ $hcname$ $arg$.")
    )
  )
)
/* Saffra 2-11-01.  Tannah made me. */
