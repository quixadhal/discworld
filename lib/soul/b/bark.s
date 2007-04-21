(bark
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You bark.")
      (rest "$mcname$ bark$s$.")
    )
    (arguments
      (arguments like a dog, like a cat, like a tree, like a wolf,
                 like a bull terrier, like something else that barks,
                 like one of those darned obnoxious little dogs, 
                 happily, annoyingly, without a care in the world,
                 warily, watchfully, bashfully, snidely,
                 enthusiastically, excitedly, obnoxiously, #)
      (self "You bark $arg$.")
      (rest "$mcname$ bark$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bark at $hcname$.")
      (target "$mcname$ bark$s$ at you.")
      (rest   "$mcname$ bark$s$ at $hcname$.")
    )
    (arguments
      (arguments like a dog, like a cat, like a tree, like a wolf,
                 like a bull terrier, like something else that barks,
                 like one of those darned obnoxious little dogs, 
                 happily, annoyingly, without a care in the world,
                 warily, watchfully, bashfully, snidely,
                 enthusiastically, excitedly, obnoxiously, #)                  
      (self "You bark $arg$ at $hcname$.")
      (target "$mcname$ bark$s$ $arg$ at you.")
      (rest "$mcname$ bark$s$ $arg$ at $hcname$.")
    )
  )
)

/* changed by Hobbes, 960911 Changed by Saffra 3/30/03 */
