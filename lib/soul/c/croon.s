(croon
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You croon.")
      (rest "$mcname$ croons.")
    )
    (arguments
    (arguments  softly, sweetly, slow songs, love songs, a lullaby, #)
      (self "You croon $arg$.")
      (rest "$mcname$ croons $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You croon into $hcname$'s ear.")
    (target "$mcname$ croons into your ear.")
    (rest "$mcname$ croons into $hcname$'s ear.")
  )
  (arguments
    (arguments  softly, sweetly, slow songs, love songs, a lullaby, #)
    (self "You croon $arg$ at $hcname$." )
    (target "$mcname$ croons $arg$ at you." )
    (rest "$mcname$ croons $arg$ at $hcname$." )
    )
  )
)
