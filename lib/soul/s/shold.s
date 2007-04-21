(shold
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You hold $hcname$.")
      (target "$mcname$ hold$s$ you.")
      (rest   "$mcname$ hold$s$ $hcname$.")
    )
    (arguments
      (arguments  hand, finger, toe, ear)
      (self   "You hold $mhcname$ $arg$.")
      (target "$mcname$ hold$s$ your $arg$.")
      (rest   "$mcname$ hold$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  tightly, close, tenderly, lovingly, carefully, friendlily, 
                  happily, sadly, needfully, to your chest, 
                  and doesn't let go, #)
      (self   "You hold $hcname$ $arg$.")
      (target "$mcname$ hold$s$ you $arg$.")
      (rest   "$mcname$ hold$s$ $hcname$ $arg$.")
    )
  )
)

