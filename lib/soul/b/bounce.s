(bounce
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "B O U N C E ! !")
      (rest "$mcname$ bounce$s$ around.")
    )
    (arguments
      (arguments excitedly, fearfully, happily, wildly)
      (self "You bounce around $arg$.")
      (rest "$mcname$ bounce$s$ around $arg$.")
    )
    (arguments
      (arguments off walls, up and down, away, #)
      (self "You bounce $arg$.")
      (rest "$mcname$ bounce$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bounce around $hcname$.")
      (target "$mcname$ bounce$s$ around you.")
      (rest  "$mcname$ bounce$s$ around $hcname$.")
    )
    (arguments
      (arguments up to, away from, up and down at, next to, with)
      (self "You bounce $arg$ $hcname$.")
      (target "$mcname$ bounce$s$ $arg$ you.")
      (rest "$mcname$ bounce$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments ankles)
      (self "You bounce $hcname$ up and down by the ankles.")
      (target "$mcname$ bounce$s$ you up and down by the ankles.")
      (rest "$mcname$ bounce$s$ $hcname$ up and down by the ankles.")
    )
    (arguments
      (arguments  happily, carefully, wildly, excitedly, pinkly, joyfully, 
                  once, slowly, suggestively, viciously, sarcastically, 
                  happily, womblely, #)
      (self   "You bounce around $hcname$ $arg$.")
      (target "$mcname$ bounce$s$ around you $arg$.")
      (rest  "$mcname$ bounce$s$ around $hcname$ $arg$.")
    )
  )
)
