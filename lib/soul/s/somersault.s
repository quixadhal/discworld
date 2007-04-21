(somersault
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  forward, backward, double, double backward)
      (self "You do a $arg$ somersault.")
      (rest "$mcname$ do$s$ a $arg$ somersault.")
    )
    (arguments
      (arguments  like a frog, like a hamster, off the bed, 
                  from a standing start, happily, sadly, ineptly, carefully, 
                  but doesn't quite make the landing, head over heels, 
                  off into the distance, into the garbage bin, viciously, 
                  dangerously, spectacularly, #)
      (self "You somersault $arg$.")
      (rest "$mcname$ somersault$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  forward, backward, double, double backward)
      (self   "You do a $arg$ somersault over $hcname$.")
      (target "$mcname$ do$s$ a $arg$ somersault over you.")
      (rest   "$mcname$ do$s$ a $arg$ somersault over $hcname$.")
    )
    (arguments)
      (arguments  like a frog, like a hamster, off the bed, 
                  from a standing start, happily, sadly ineptly, carefully, 
                  but don't quite make the landing, head over heels, 
                  off into the distance, into the garbage bin, viciously, 
                  dangerously, spectacularly, #)
      (self   "You somersault over $hcname$ $arg$.")
      (target "$mcname$ somersault$s$ over you $arg$.")
      (rest   "$mcname$ somersault$s$ over $hcname$ $arg$.")
    )
    (arguments
      (arguments  lap)
      (self   "You somersault and land on $mhcname$ $arg$.")
      (target "$mcname$ somersault$s$ and lands on your $arg$.")
      (rest   "$mcname$ somersault$s$ and lands on $mhcname$ $arg$.") 
    )
    (arguments
      (arguments  shoulders)
      (self   "You somersault and land on $mhcname$ $arg$.")
      (target "$mcname$ somersault$s$ and lands on your $arg$.")
      (rest   "$mcname$ somersault$s$ and lands on $mhcname$ $arg$.")
    )
    (arguments
      (arguments  diving board, trampoline, bed, chair, lounge, roof, ceiling, 
                  feet)
      (self   "You somersault off your $arg$ and land on $hcname$.")
      (target "$mcname$ somersault$s$ off $mposs$ $arg$ and lands on you.")
      (rest   "$mcname$ somersault$s$ off $mposs$ $arg$ and lands on $hcname$.")
    )
  )
)

