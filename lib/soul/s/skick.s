(skick
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You kick yourself.")
      (rest "$mcname$ kick$s$ $mobj$self.")
    )
    (arguments
      (arguments #)
      (self "You kick yourself $arg$.")
      (rest "$mcname$ kick$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You kick $hcname$.")
      (target "$mcname$ kick$s$ you.")
      (rest  "$mcname$ kick$s$ $hcname$.")
    )
    (arguments
      (arguments  #)
      (self   "You kick $hcname$ $arg$.")
      (target "$mcname$ kick$s$ you $arg$.")
      (rest  "$mcname$ kick$s$ $hcname$ $arg$.")
    )
  )
)

