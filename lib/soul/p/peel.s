(peel
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  orange, apple, banana, potato)
      (self "You peel your $arg$.")
      (rest "$mcname$ peel$s$ $mposs$ $arg$.")
    )
    (arguments
      (arguments  lips, eyebrows, eyelashes, moustache, ears, mask, clothes, 
                  skin, #)
      (self "You peel off your $arg$.")
      (rest "$mcname$ peel$s$ off $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  orange, apple, banana, potato)
      (self   "You peel $mhcname$ $arg$.")
      (target "$mcname$ peel$s$ your $arg$.")
      (rest   "$mcname$ peel$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  lips, eyebrows, eyelashes, moustache, ears, mask, clothes, 
                  skin, #)
      (self   "You peel off $mhcname$ $arg$.")
      (target "$mcname$ peel$s$ off your $arg$.")
      (rest   "$mcname$ peel$s$ off $mhcname$ $arg$.")
    )
  )
)

