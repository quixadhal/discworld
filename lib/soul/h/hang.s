(hang
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments shame,sorrow,despair,happiness)
      (self "You hang your head in $arg$.")
      (rest "$mcname$ hang$s$ $mposs$ head in $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hang onto $hcname$.")
      (target "$mcname$ hang$s$ onto you.")
      (rest  "$mcname$ hang$s$ onto $hcname$.")
    )
    (arguments
      (arguments  tightly,passionately,fearfully,possessively,paranoically)
      (self   "You hang onto $hcname$ $arg$.")
      (target "$mcname$ hang$s$ onto you $arg$.")
      (rest  "$mcname$ hang$s$ onto $hcname$ $arg$.")
    )
  )
)

