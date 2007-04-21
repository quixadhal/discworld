(tongue
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You poke your tongue out.")
      (rest  "$mcname$ poke$s$ $mposs$ tongue out.")
    )
    (arguments
      (arguments at the world, at everyone, at no one in particular, stupidly, 
                 lazily, quickly, like a snake, like a lizard, 
                 like a salamander, like a frog, like a toad, like a newt,
                 like a hummingbird, #)
      (self  "You poke your tongue out.")
      (rest  "$mcname$ poke$s$ $mposs$ tongue out.")
    )
  )
  (targeted
    (no-arguments
      (self   "You poke your tongue out at $hcname$.")
      (target "$mcname$ poke$s$ $mposs$ tongue out at you.")
      (rest   "$mcname$ poke$s$ $mposs$ tongue out at $hcname$.")
    )
    (arguments
      (arguments stupidly, lazily, quickly, like a snake, like a lizard, 
                 like a salamander, like a frog, like a toad, like a newt,
                 like a hummingbird, #)
      (self   "You poke your tongue out at $hcname$ $arg$.")
      (target "$mcname$ poke$s$ $mposs$ tongue out at you $arg$.")
      (rest   "$mcname$ poke$s$ $mposs$ tongue out at $hcname$ $arg$.")
    )
    (arguments
      (arguments pierced, tatooed, frozen, hairy, fluffy, messy,
                 baked bean covered, green, hard)
      (self   "You poke your $arg$ tongue out at $hcname$ and wiggle it.")
      (target "$mcname$ poke$s$ $mposs$ $arg$ tongue out at $hcname$ and wiggles i
t.")
      (rest   "$mcname$ poke$s$ $mposs$ $arg$ tongue out at $hcname$ and wiggles i
t.")
    )
  )
)
