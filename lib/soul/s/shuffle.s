(shuffle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
   (no-arguments 
      (self "You shuffle your feet in embarrassment.")
      (rest "$mcname$ shuffle$s$ $mposs$ feet in embarrassment.")
    )
  )
  (targeted     
    (arguments
      (arguments  away from, up to, towards, #)
      (self   "You shuffle $arg$ $hcname$.")
      (target "$mcname$ shuffle$s$ $arg$ you.")
      (rest   "$mcname$ shuffle$s$ $arg$ $hcname$.")
     )
     (arguments
      (arguments  feet, deck, cards)
      (self   "You shuffle your $arg$ in embarrassment at $hcname$.")
      (target "$mcname$ shuffle$s$ $mposs$ $arg$ in embarrassment at you.")
      (rest   "$mcname$ shuffle$s$ $mposs$ $arg$ in embarrassment at $hcname$.")
     )
   )
 )
