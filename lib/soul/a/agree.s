(agree
  (pattern "[with] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [with] <indirect:any-living>")
  (pattern "[with] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  wholeheartedly, partially, wearily, sadly, belatedly, 
                  happily, under duress, about everything, 
                  about the state of the world, #)
      (self "You agree $arg$.")
      (rest "$mcname$ agree$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  wholeheartedly, partially, wearily, sadly, belatedly, 
                  happily, under duress, #)
      (self   "You agree with $hcname$ $arg$.")
      (target "$mcname$ agree$s$ with you $arg$.")
      (rest   "$mcname$ agree$s$ with $hcname$ $arg$.")
    )
  )
)

