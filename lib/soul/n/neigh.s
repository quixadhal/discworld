(neigh
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You neigh.")
      (rest "$mcname$ neighs.")
    )
    (arguments
      (arguments horse, horsie, my little Binky, dog, cat, tree, wolf,
                 labrador, german shepherd, bull terrier)
      (self "You neigh like a $arg$.")
      (rest "$mcname$ neigh$s$ like a $arg$.")
    )
    (arguments
      (arguments something else that neighs,
                 one of those darned obnoxious little horse, horsie, my little Binky, dogs)
      (self "You neigh like $arg$.")
      (rest "$mcname$ neigh$s$ like $arg$.")
    )
    (arguments
      (arguments happily, annoyingly, without a care in the world,
                 warily, watchfully, bashfully, snidely,
                 enthusiastically, excitedly, obnoxiously, #)
      (self "You neigh $arg$.")
      (rest "$mcname$ neigh$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You neigh at $hcname$.")
      (target "$mcname$ neigh$s$ at you.")
      (rest  "$mcname$ neigh$s$ at $hcname$.")
    )
    (arguments
      (arguments horse, horsie, my little Binky, dog, cat, tree, wolf,
                 labrador, german shepherd, bull terrier)
      (self "You neigh like a $arg$ at $hcname$.")
      (rest "$mcname$ neigh$s$ like a $arg$ at you.")
      (rest "$mcname$ neigh$s$ like a $arg$ at $hcname$.")
    )
    (arguments
      (arguments something else that neighs,
                 one of those darned obnoxious little horse, horsie, my little Binky, dogs)
      (self "You neigh like $arg$ at $hcname$.")
      (rest "$mcname$ neigh$s$ like $arg$ at you.")
      (rest "$mcname$ neigh$s$ like $arg$ at $hcname$.")
    )
    (arguments
      (arguments happily, annoyingly, without a care in the world,
                 warily, watchfully, bashfully, snidely,
                 enthusiastically, excitedly, obnoxiously, #)
      (self "You neigh $arg$ at $hcname$.")
      (rest "$mcname$ neigh$s$ $arg$ at you.")
      (rest "$mcname$ neigh$s$ $arg$ at $hcname$.")
    )
  )
)

/* changed by Hobbes, 960911 */
