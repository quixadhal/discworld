(mowr
  (pattern "<string>" )
  (pattern "<indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> [at] <indirect:any-living>") 
  (single
    (no-arguments
      (self "You mowr.")
      (rest "$mcname$ mowr$s$.")
    )
    (arguments
      (arguments  like a cat, like a cute kitten, like a lion, 
                happily, mischievously, affectionately, seductively, 
                flirtatiously, angrily, hungrily, sadly, #)
      (self "You mowr $arg$.")
      (rest "$mcname$ mowr$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You mowr at $hcname$.")
      (target "$mcname$ mowrs$s$ at you.")
      (rest "$mcname$ mowr$s$ at $hcname$.")
    )
    (arguments
      (arguments  like a cat, like a cute kitten, like a lion, 
                  happily, mischievously, affectionately, seductively, 
                  flirtatiously, angrily, hungrily, sadly, #)
      (self "You mowr $arg$ at $hcname$.")
      (target "$mcname$ mowr$s$ $arg$ at you.")
      (rest "$mcname$ mowr$s$ $arg$ at $hcname$.")
    )
  )
)

