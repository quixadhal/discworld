(yodel
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You yodel loudly.")
      (rest   "$mcname$ yodel$s$ loudly.")
    )
    (arguments
      (arguments  happily, sadly, annoyingly, romantically, dreamily, 
                  incessantly, constantly, to the tune of william tell's 
                  overture, carefully, loudly, sternly, pinkly, 
                  like a blackboard, with the frogs, yodel-ay-hee-hoo, 
                  like a mountain goat, like a lonely goatherd, #)
      (self "You yodel $arg$.")
      (rest "$mcname$ yodel$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You yodel at $hcname$.")
      (target "$mcname$ yodel$s$ at you.")
      (rest   "$mcname$ yodel$s$ at $hcname$.")
    )
    (arguments
      (arguments  happily, sadly, annoyingly, romantically, dreamily, 
                  incessantly, constantly, to the tune of william tell's 
                  overture, carefully, loudly, sternly, pinkly, 
                  like a blackboard, with the frogs, yodel-ay-hee-hoo, 
                  like a mountain goat, like a lonely goatherd, #)
      (self   "You yodel $arg$ at $hcname$.")
      (target "$mcname$ yodel$s$ $arg$ at you.")
      (rest  "$mcname$ yodel$s$ $arg$ at $hcname$.")
    )
  )
)
