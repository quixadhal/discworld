(admire
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You admire $hcname$.")
      (target "$mcname$ admire$s$ you.")
      (rest  "$mcname$ admire$s$ $hcname$.")
    )
    (arguments
      (arguments  from afar)
      (self   "You admire $hcname$ $arg$.")
      (target "$mcname$ admire$s$ you $arg$.")
      (rest  "$mcname$ admire$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  eyes, legs, bottom, body, ears, nose, fingers, toes,
                  dress, trousers, shirt, tenacity, #)
      (self   "You admire $mhcname$ $arg$.")
      (target "$mcname$ admire$s$ your $arg$.")
      (rest  "$mcname$ admire$s$ $mhcname$ $arg$.")
    )
  )
)

