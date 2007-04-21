(signore
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
    (arguments  the world, the universe, tea, turtles, frogs, elephants, 
                the weather, assassins, witches, wizards, thieves, warriors, 
                priests, adventurers, ghosts, creators, Directors, Trustees, #)
      (self "You ignore $arg$.")
      (rest "$mcname$ ignore$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ignore $hcname$.")
      (target "$mcname$ ignore$s$ you.")
      (rest   "$mcname$ ignore$s$ $hcname$.")
    )
    (arguments
      (arguments  forever, for all time, twice, with justification, with zeal, 
                  #)
      (self   "You ignore $hcname$ $arg$.")
      (target "$mcname$ ignore$s$ you $arg$.")
      (rest   "$mcname$ ignore$s$ $hcname$ $arg$.")
    )
  )
)

