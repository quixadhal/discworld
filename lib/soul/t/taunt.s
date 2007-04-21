(taunt
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You taunt yourself.")
      (rest "$mcname$ taunts $mobj$self... how odd.")
    )
    (arguments
    (arguments slyly, expertly, from behind a tree, with higher numbers, 
               with a bad Quirmian accent, nastily, very nastily, 
               very nastily indeed, childlishly, rudely, ceaselessly, #) 
      (self "You taunt yourself $arg$.")
      (rest "$mcname$ taunts $mobj$self $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You taunt $hcname$.")
      (target "$mcname$ taunts you.")
      (rest   "$mcname$ taunts $hcname$.")
    )
    (arguments
    (arguments slyly, expertly, from behind a tree, with higher numbers, 
               with a bad Quirmian accent, nastily, very nastily, 
               very nastily indeed, childlishly, rudely, ceaselessly, #)
      (self   "You taunt $hcname$ $arg$.")
      (target "$mcname$ taunts you $arg$.")
      (rest   "$mcname$ taunts $hcname$ $arg$.")
    )
  )
)

