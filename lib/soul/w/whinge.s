(whinge
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You whinge.")
      (rest "$mcname$ whinges.")
    )
    (arguments
    (arguments  incessantly, endlessly, annoyingly, loudly,
                repeatedly, whingingly, whiningly, like an 
                assassin, like a witch, like a player, on the boards,
                in real life, meekly, quietly, modestly, in a 
                passive aggressive manner, #)
      (self "You whinge $arg$.")
      (rest "$mcname$ whinge$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You whinge at $hcname$.")
    (target "$mcname$ whinge$s$ at you.")
    (rest "$mcname$ whinge$s$ at $hcname$.")
  )
  (arguments
  (arguments  incessantly, endlessly, annoyingly, loudly,
              repeatedly, whingingly, whiningly, like an 
              assassin, like a witch, like a player, on the boards,
              in real life, meekly, quietly, modestly, in a 
              passive aggressive manner, #)
    (self "You whinge at $hcname$ $arg$." )
    (target "$mcname$ whinge$s$ at you $arg$." )
    (rest "$mcname$ whinge$s$ at $hcname$ $arg$." )
    )
  )
)
