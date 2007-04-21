(bonk
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "B O N K !")
      (rest "$mcname$ go$s$ B O N K !")
    )
  )
  (targeted
    (no-arguments
      (self   "You bonk $hcname$ on the head.")
      (target "$mcname$ bonk$s$ you on the head.")
      (rest   "$mcname$ bonk$s$ $hcname$ on the head.")
    )
    (arguments
       (arguments  on the head, with a coffee table, between the sheets, 
                   like a puppet, with a lemon, onto the grass, 
                   with a foam bat, #)
      (self   "You bonk $hcname$ $arg$.")
      (target "$mcname$ bonk$s$ you $arg$.")
      (rest   "$mcname$ bonk$s$ $hcname$ $arg$.")
    )
  )
)
