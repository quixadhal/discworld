(worry
  (pattern "[about] <string>")
  (pattern "[about] <indirect:any-living>")
  (pattern "<string> [about] <indirect:any-living>")
  (pattern "[about] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You worry about yourself.")
      (rest "$mcname$ worry$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You worry about $hcname$.")
      (target "$mcname$ worry$s$ about you.")
      (rest   "$mcname$ worry$s$ about $hcname$.")
    )
    (arguments
      (arguments  attitude, response, feelings, sleep patterns, #)
      (self   "You worry about $mhcname$ $arg$.")
      (target "$mcname$ worry$s$ about your $arg$.")
      (rest   "$mcname$ worry$s$ about $mhcname$ $arg$.")
    )
  )
)

