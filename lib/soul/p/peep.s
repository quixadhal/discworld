(peep
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern " <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string> ")
  (single
    (no-arguments
      (self "You peep like a cute fluffy chick.")
      (rest "$mcname$ peep$s$ like a cute fluffy chick.")
    )
    (arguments
      (arguments cutely, adorably, fluffily, brightly, quietly,
                 like a wet hen, like a ruffled chicken, like a 
                 marshmallow peep,  # )
      (self "You peep $arg$.")
      (rest "$mcname$ peep$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You peep at $hcname$ like a cute fluffy chick.")
      (target "$mcname$ peep$s$ at you like a cute fluffy chick.")
      (rest "$mcname$ peep$s$ at $hcname$ like a cute fluffy chick.")
    )
    (arguments
      (arguments cutely, adorably, fluffily, brightly, quietly,
                 like a wet hen, like a ruffled chicken, like a 
                 marshmallow peep,  # )
      (self   "You peep $arg$ at $hcname$.")
      (target "$mcname$ peep$s$ $arg$ at you.")
      (rest  "$mcname$ peep$s$ $arg$ at $hcname$.")
    )
  )
)
