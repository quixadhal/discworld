(shriek
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shriek.")
      (rest "$mcname$ shriek$s$.")
    )
    (arguments
      (arguments loudly, excitedly, nervously, piercingly, shrilly, horribly, 
                 in fear, in excitement, in delight, like a schoolgirl, 
                 like a nazgul, #)
      (self "You shriek $arg$.")
      (rest "$mcname$ shriek$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments loudly, excitedly, nervously, piercingly, shrilly, horribly,
                 in fear, in excitement, in delight, like a schoolgirl, 
                 like a nazgul, #)
      (self   "You shriek $arg$ at $hcname$.")
      (target "$mcname$ shriek$s$ $arg$ at you.")
      (rest  "$mcname$ shriek$s$ $arg$ at $hcname$.")
    )
  )
)

/* Saffra Dec 27 2001 */