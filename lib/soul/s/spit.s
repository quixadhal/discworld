(spit
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You spit.")
      (rest "$mcname$ spit$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You spit in $mhcname$ face.")
      (target "$mcname$ spit$s$ in your face.")
      (rest   "$mcname$ spit$s$ in $mhcname$ face.")
    )
    (arguments
      (arguments  food, drink, general direction, face)
      (self   "You spit in $mhcname$ $arg$.")
      (target "$mcname$ spit$s$ in your $arg$.")
      (rest   "$mcname$ spit$s$ in $mhcname$ $arg$.")
    )
    (arguments
      (arguments  in disgust, in loathing, hatefully, #)
      (self   "You spit $arg$ at $hcname$.")
      (target "$mcname$ spit$s$ $arg$ at you.")
      (rest   "$mcname$ spit$s$ $hcname$ face.")
    )
  )
)

