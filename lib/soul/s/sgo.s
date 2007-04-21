(sgo
  (pattern "<string>" )
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
    (arguments  #)
      (self "You go $arg$.")
      (rest "$mcname$ go$s$ $arg$.")
    )
  )
  (targeted
    (arguments
    (arguments  #)
    (self   "You go $arg$ at $hcname$.")
    (target "$mcname$ go$s$ $arg$ at you.")
    (rest   "$mcname$ go$s$ $arg$ at $hcname$.")
    )
  )
)
