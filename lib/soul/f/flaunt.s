(flaunt
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You flaunt about.")
      (rest "$mcname$ flaunt$s$ about.")
    )
    (arguments
      (arguments wealth, wallet, wardrobe, weapons, hair, armour, jewellery, 
                 popularity, skills, bonus, property, pets, legs, bottom, 
                 figure, abilities, wisdom, luck, shiny things, 
                 magical prowess, vast hordes of treasure, #)
      (self "You flaunt your $arg$.")
      (rest "$mcname$ flaunt$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments wealth, wallet, wardrobe, weapons, hair, armour, jewellery, 
                 popularity, skills, bonus, property, pets, legs, bottom, 
                 figure, abilities, wisdom, luck, shiny things, 
                 magical prowess, vast hordes of treasure, #)
      (self "You flaunt your $arg$ at $hcname$.")
      (target "$mcname$ flaunt$s$ $mposs$ $arg$ at you.")
      (rest "$mcname$ flaunt$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)

/* Saffra Dec 27 2001, due to multiple requests */