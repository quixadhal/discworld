(bah
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You pause for a moment of reflection, and then exclaim 'Bah!'.")
      (rest "$mcname$ pause$s$ for a moment of reflection, and then exclaim$s$ 'Bah!'.")
    )
    (arguments
      (arguments  hand, paw, foot, elbow, marshmallow, handkerchief, watermelon, suitcase, #)
      (self "You exclaim 'Bah!' and wave your $arg$ dismissively.")
      (rest "$mcname$ exclaim$s$ 'Bah!' and wave$s$ $mposs$ $arg$ dismissively.")
    )
  )
  (targeted
    (no-arguments
      (self   "You exclaim 'Bah!' at $hcname$.")
      (target "$mcname$ exclaims 'Bah!' at you.")
      (rest   "$mcname$ exclaims 'Bah!' at $hcname$.")
    )
    (arguments
      (arguments  hand, paw, foot, elbow, marshmallow, handkerchief, watermelon, suitcase, #)
      (self   "You exclaim 'Bah!' and wave your $arg$ dismissively at $hcname$.")
      (target "$mcname$ exclaim$s$ 'Bah!' and wave$s$ $mposs$ $arg$ dismissively at you.")
      (rest   "$mcname$ exclaim$s$ 'Bah!' and wave$s$ $mposs$ $arg$ dismissively at $hcname$.")
    )
  )
)

