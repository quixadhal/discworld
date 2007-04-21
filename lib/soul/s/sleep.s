(sleep
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You go to sleep.")
      (rest "$mcname$ go$s$ to sleep.")
    )
  )
  (targeted
    (no-arguments
      (self   "You go to sleep in $mhcname$ arms.")
      (target "$mcname$ go$s$ to sleep in your arms.")
      (rest   "$mcname$ go$s$ to sleep in $mhcname$ arms.")
    )
    (arguments
      (arguments  bed, bathtub, car, refrigerator, dreams,
                  thoughts, heart, shower, house, lap, 
                  room, pants, #)  
      (self   "You go to sleep in $mhcname$ $arg$.")
      (target "$mcname$ go$s$ to sleep in your $arg$.")
      (rest   "$mcname$ go$s$ to sleep in $mhcname$ $arg$.")
    )
  )
)

/* Made it take args. Saffra, 10-6-00 */