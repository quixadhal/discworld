(stagger
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You stagger around.")
      (rest "$mcname$ stagger$s$ around.")
    )
    (arguments
      (arguments  drunkenly, blindly, in the dark, with a hat on, like a frog, 
                  #)
      (self "You stagger around $arg$.")
      (rest "$mcname$ stagger$s$ around $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You stagger around $hcname$.")
      (target "$mcname$ stagger$s$ around you.")
      (rest   "$mcname$ stagger$s$ around $hcname$.")
    )
    (arguments
      (arguments  burden, with, with the added burden of carrying)
      (self   "You stagger around with the added burden of carrying $hcname$.")
      (target "$mcname$ stagger$s$ around with the added burden of carrying you.")
      (rest   "$mcname$ stagger$s$ around with the added burden of carrying $hcname$.")
    )
    (arguments
      (arguments  drunkenly, blindly, in the dark, with a hat on, like a frog, 
                  #)
      (self   "You stagger around $hcname$ $arg$.")
      (target "$mcname$ stagger$s$ around you $arg$.")
      (rest   "$mcname$ stagger$s$ around $hcname$ $arg$.")
    )
  )
)

