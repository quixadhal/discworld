(dribble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You start to dribble all over the front of your shirt.")
      (rest "$mcname$ start$s$ to dribble all over the front of $mposs$ shirt.")
    )
    (arguments
      (arguments milk, beer, wine, water, scumble, tea, juice, baby food, #)
      (self "You start to dribble $arg$ all over the front of your shirt.")
      (rest "$mcname$ start$s$ to dribble $arg$ all over the front of $mposs$ shirt.")
    )
  )
  (targeted
    (no-arguments
      (self   "You start dribbling on $hcname$.")
      (target "$mcname$ start$s$ dribbling all over your best shirt.")
      (rest  "$mcname$ start$s$ dribbling all over $hcname$.")
    )
    (arguments
      (arguments milk, beer, wine, water, scumble, tea, juice, baby food, #)
      (self "You start to dribble $arg$ all over $hcname$.")
      (target "$mcname$ start$s$ to dribble $arg$ all over $hcname$.")
      (rest "$mcname$ start$s$ to dribble $arg$ all over $hcname$.")
    )
  )
)
