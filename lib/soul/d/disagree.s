(disagree
  (pattern "[with] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [with] <indirect:any-living>")
  (pattern "[with] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  wholeheartedly, partially, wearily, sadly, belatedly, 
                  happily, under duress, about everything, 
                  out of stubbornness, #)
      (self "You disagree $arg$.")
      (rest "$mcname$ disagree$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You disagree with $hcname$.")
      (target "$mcname$ disagree$s$ with you.")
      (rest  "$mcname$ disagree$s$ with $hcname$.")
    )
    (arguments
      (arguments  wholeheartedly, partially, wearily, sadly, belatedly, 
                  happily, under duress, #)
      (self   "You disagree with $hcname$ $arg$.")
      (target "$mcname$ disagree$s$ with you $arg$.")
      (rest   "$mcname$ disagree$s$ with $hcname$ $arg$.")
    )
  )
)
