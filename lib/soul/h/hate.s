(hate
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You hate the world.")
      (rest "$mcname$ hate$s$ the world.")
    )
    (arguments
      (arguments #)
      (self "You hate $arg$.")
      (rest "$mcname$ hate$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hate $hcname$.")
      (target "$mcname$ hate$s$ you.")
      (rest  "$mcname$ hate$s$ $hcname$.")
    )
    (arguments
      (arguments  #)
      (self   "You hate $hcname$ $arg$.")
      (target "$mcname$ hate$s$ you $arg$.")
      (rest  "$mcname$ hate$s$ $hcname$ $arg$.")
    )
  )
)
/*Hobbes 970415, suggested by Gruper. */
