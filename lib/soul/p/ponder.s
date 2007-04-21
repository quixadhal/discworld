(ponder
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ponder.")
      (rest "$mcname$ ponder$s$.")
    )
    (arguments
      (arguments  the meaning of it all, the situation, wombles, cars, fruit, 
                  pigeons, frogs, toy trains, environmental concerns, 
                  nuclear disasters, love, #)
      (self "You ponder $arg$.")
      (rest "$mcname$ ponder$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ponder at $hcname$.")
      (target "$mcname$ ponder$s$ at you.")
      (rest   "$mcname$ ponder$s$ at $hcname$.")
    )
    (arguments
      (arguments  sanity, situation, wombles, cars, fruit, pigeons, frogs, 
                  toy trains, hobbies, passions, loves, hates, hairstyle)
      (self   "You ponder $mhcname$ $arg$.")
      (target "$mcname$ ponder$s$ your $arg$.")
      (rest   "$mcname$ ponder$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  thoughtfully, sarcastically, hard, knowingly, interestingly, 
                  worriedly, happily, sadly, cutely, #)
      (self   "You ponder at $hcname$ $arg$.")
      (target "$mcname$ ponder$s$ at you $arg$.")
      (rest   "$mcname$ ponder$s$ at $hcname$ $arg$.")
    )
  )
)

