(smack
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You smack your lips.")
      (rest "$mcname$ smack$s$ $mposs$ lips.")
    )
  )
  (targeted
    (no-arguments
      (self   "You smack $hcname$.")
      (target "$mcname$ smack$s$ you.")
      (rest  "$mcname$ smack$s$ $hcname$.")
    )
    (arguments
      (arguments  furiously, with gusto, without a care, twice, 
                  lots of times, happily, nastily, excitedly, 
                  upside the head, with a coffee table, 
                  with a tabby cat, unbelieveably hard, 
                  with a smallish potted plant, with a keyboard, #)
      (self   "You smack $hcname$ $arg$.")
      (target "$mcname$ smack$s$ you $arg$.")
      (rest  "$mcname$ smack$s$ $hcname$ $arg$.")
    )
  )
)

