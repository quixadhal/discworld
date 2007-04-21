(scamper
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You scamper around.")
      (rest "$mcname$ scamper$s$ around.")
    )
    (arguments
      (arguments cutely, madly, happily, enticingly, 
                 suggestively, vigorously, nonchalantly, quickly, 
                 like a playful puppy, playfully, happily, energetically,
                 in a hyper manner, from too much caffeine, from too much
                 crack, #)
      (self "You scamper around $arg$.")
      (rest "$mcname$ scamper$s$ around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You scamper around $hcname$.")
      (target "$mcname$ scamper$s$ around you.")
      (rest  "$mcname$ scamper$s$ around $hcname$.")
    )
    (arguments
      (arguments cutely, all, madly, happily, enticingly, 
                 suggestively, vigorously, nonchalantly, quickly, 
                 playfully, happily, energetically, in a hyper manner, 
                 from too much caffeine, from too much crack, #)
      (self   "You scamper $arg$ around $hcname$.")
      (target "$mcname$ scamper$s$ $arg$ around you.")
      (rest  "$mcname$ scamper$s$ $arg$ around $hcname$.")
    )
    (arguments
      (arguments feet, legs)
      (self   "You scamper around $mhcname$ $arg$.")
      (target "$mcname$ scamper$s$ around your $arg$.")
      (rest  "$mcname$ scamper$s$ around $mhcname$ $arg$.")
    )
    (arguments
      (arguments like a playful puppy)
      (self   "You scamper up to $hcname$ $arg$.")
      (target "$mcname$ scamper$s$ up to you $arg$.")
      (rest  "$mcname$ scamper$s$ up to $hcname$ $arg$.")
    )
  )
)
