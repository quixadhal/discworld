(pat
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You pat yourself on the back.")
      (rest  "$mcname$ pat$s$ $mobj$self on the back.")
    )
    (arguments
      (arguments  head, back, leg, arm, nose, bottom, tummy, belly, #)
      (self  "You pat yourself on the $arg$.")
      (rest  "$mcname$ pat$s$ $mobj$self on the $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  head, back, leg, arm, nose, bottom, tummy, belly, #)
      (self   "You pat $hcname$ on the $arg$.")
      (target "$mcname$ pat$s$ you on the $arg$.")
      (rest  "$mcname$ pat$s$ $hcname$ on the $arg$.")
    )
  )
)
