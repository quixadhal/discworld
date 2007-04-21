(pinch
  (pattern "<indirect:any-living> on the <string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> [on] <string>")
  (single
    (no-arguments
      (self "You pinch yourself.")
      (rest "$mcname$ pinch$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You pinch $hcname$.")
      (target "$mcname$ pinch$s$ you.")
      (rest   "$mcname$ pinch$s$ $hcname$.")
    )
    (arguments
      (arguments  bottom, leg, arm, frog, cheek, #)
      (self   "You pinch $hcname$ on the $arg$.")
      (target "$mcname$ pinch$s$ you on the $arg$.")
      (rest   "$mcname$ pinch$s$ $hcname$ on the $arg$.")
    )
  )
)

