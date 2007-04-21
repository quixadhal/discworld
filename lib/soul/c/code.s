(code
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments like Pinkfish,like Deutha,something to annoy all the players,a bug,#)
      (self "You code $arg$.")
      (rest "$mcname$ codes $arg$.")
    )
  )
  (targeted
    (arguments
       (arguments like Pinkfish,like Deutha,something to annoy all the players,a bug,#)
      (self   "You code $arg$ at $hcname$.")
      (target "$mcname$ codes $arg$ at you.")
      (rest  "$mcname$ codes $arg$ at $hcname$.")
    )
  )
)

