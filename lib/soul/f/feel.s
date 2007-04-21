(feel
  (pattern "<string>" )
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You feel around.")
      (rest "$mcname$ feel$s$ around.")
    )
    (arguments
    (arguments  fine, good, bad, happy, sad, emotional, unemotional, up, down,
                blue, guilty, thoughtful, hungry, thirsty, tired, energetic,
                chatty, whiny, fluffy, flamey, like a million dollars, 
                like a million royals, like a million Rhinu, 
                fine at the end of the world, #)
      (self "You feel $arg$.")
      (rest "$mcname$ feel$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
    (self   "You feel $hcname$.")
    (target "$mcname$ feel$s$ you.")
    (rest   "$mcname$ feel$s$ $hcname$.")
    )
    (arguments
    (arguments  forehead, pulse, pain, muscles, love, wrath, feelings, 
                derision, fluff, eyes, #)
    (self   "You feel $mhcname$ $arg$.")
    (target "$mcname$ feel$s$ $mhcname$ $arg$.")
    (rest   "$mcname$ feel$s$ $mhcname$ $arg$.")
    )
  )
)

/* Saffra 04-18-03 for Terano */

