(honk
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You honk.")
      (rest "$mcname$ honk$s$.")
    )
    (arguments
      (arguments like a goose,like a confused frog, happily, sadly, joyfully, 
                 sagely, knowingly, quickly, loudly, carefully, balefully, 
                 excitedly, nervously, brightly, uncertainly, threateningly, 
                 grumpily, fluffily, sarcastically,#)
      (self "You honk $arg$.")
      (rest "$mcname$ honk$s$ $arg$.")
    )
    (arguments
      (arguments horn)
       (self "You honk your $arg$.")
       (rest "$mcname$ honk$s$ $mposs$ $arg$.")
	  )
          )
  (targeted
    (no-arguments
      (self   "You honk at $hcname$.")
      (target "$mcname$ honk$s$ at you.")
      (rest  "$mcname$ honk$s$ at $hcname$.")
    )
    (arguments
      (arguments like a goose,like a confused frog, happily, sadly, joyfully, 
                 sagely, knowingly, quickly, loudly, carefully, balefully, 
                 excitedly, nervously, brightly, uncertainly, threateningly, 
                 grumpily, fluffily, sarcastically,#)
      (self   "You honk $arg$ at $hcname$.")
      (target "$mcname$ honk$s$ $arg$ at you.")
      (rest  "$mcname$ honk$s$ $arg$ at $hcname$.")
    )
     (arguments
     (arguments horn)
      (self   "You honk your $arg$ at $hcname$.")
      (target "$mcname$ honk$s$ $mposs$ $arg$ at you.")
      (rest  "$mcname$ honk$s$ $mposs$ $arg$ at $hcname$.")
      )
    )
  )
	
/* Hobbes, 970524 */


