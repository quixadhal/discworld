(jump
  (pattern "[on] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [on] <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  excitedly, happily, carefully, slowly, joyfully, 
                  unsteadily, ecstatically, worriedly, sideways, 
                  absentmindedly, like Pinkfish, like a prancing piranha, #)
      (self "You $position$jump up and down $arg$.")
      (rest "$mcname$ $position$jump$s$ up and down $arg$.")
      (position
        (standing "ignore")
        (default
          (cmd "stand")
          (self "stand up and ")
          (rest "stands up and ")
        )
        (silent "yes")
      )
    )
  )
  (targeted
    (no-arguments
      (self   "You $aposition$jump up and down on $hcname$.")
      (target "$mcname$ $aposition$jump$s$ up and down on you.")
      (rest   "$mcname$ $aposition$jump$s$ up and down on $hcname$.")
      (position
        (actor
          (default
            (cmd "stand")
            (self "stand up and ")
            (target "stands up and ")
            (rest "stands up and ")
          )
          (standing "ignore")
          (silent "yes")
        )
      )
    )
    (arguments
      (arguments  excitedly, happily, carefully, slowly, joyfully, 
                  unsteadily, ecstatically, worriedly, sideways, 
                  absentmindedly, like Pinkfish, like a prancing piranha, #)
      (self   "You $aposition$jump up and down on $hcname$ $arg$.")
      (target "$mcname$ $aposition$jump$s$ up and down on you $arg$.")
      (rest   "$mcname$ $aposition$jump$s$ up and down on $hcname$ $arg$.")
      (position
        (actor
          (default
            (cmd "stand")
            (self "stand up and ")
            (target "stands up and ")
            (rest "stands up and ")
          )
          (standing "ignore")
          (silent "yes")
        )
      )
    )
  )
)
