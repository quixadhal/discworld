(scold
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You scold $hcname$.")
      (target "$mcname$ scold$s$ you.")
      (rest  "$mcname$ scold$s$ $hcname$.")
    )
    (arguments
      (arguments  playfully, mercilessly, emphatically, 
                  whilst stifling a giggle, lovingly, sneakily, pointlessly,     
                  uselessly, loudly, endlessly,  irritably, conveniently, #)
      (self   "You scold $hcname$ $arg$.")
      (target "$mcname$ scold$s$ you $arg$.")
      (rest  "$mcname$ scold$s$ $hcname$ $arg$.")
    )
  )
)
