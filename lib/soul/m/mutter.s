(mutter
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You mutter under your breath.")
      (rest "$mcname$ mutter$s$ under $mposs$ breath.")
    )
    (arguments
      (arguments  lag, creators, TM's, #)
         (self "You mutter something about $arg$ under your breath.")
         (rest "$mcname$ mutter$s$ something about $arg$ under $mposs$ breath.")
      )
    )
    (targeted
      (no-arguments
      (self   "You mutter under your breath at $hcname$.")
      (target "$mcname$ mutter$s$ under $mposs$ breath at you.")
      (rest   "$mcname$ mutter$s$ under $mposs$ breath at $hcname$.")
    )
    (arguments
      (arguments  lag, creators, TM's, #)
      (self   "You mutter under your breath about $arg$ at $hcname$.")
      (target "$mcname$ mutter$s$ under $mposs$ breath about $arg$ at you.")
      (rest   "$mcname$ mutter$s$ under $mposs$ breath about $arg$ at $hcname$.")
    )
    (arguments
      (arguments  darkly, quietly, loudly, verbosely, briefly, idly)
      (self   "You mutter $arg$ at $hcname$.")
      (target "$mcname$ mutter$s$ $arg$ at you.")
      (rest   "$mcname$ mutter$s$ $arg$ at $hcname$.")
    )
  )
)
/*Hobbes 970311*/