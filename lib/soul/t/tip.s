(tip
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments hat, nose, frog, pointy black hat, pointy hat, top hat, 
                 beret, wig, fedora, bottle, thinking cap, dunce cap, 
                 crown, tiara, laurel wreath, #)
      (self "You tip your $arg$ stylishly.")
      (rest "$mcname$ tip$s$ $mposs$ $arg$ stylishly.")
    )
  )
  (targeted
    (arguments
      (arguments hat, nose, frog, pointy black hat, pointy hat, top hat, 
                 beret, wig, fedora, bottle, thinking cap, dunce cap, 
                 crown, tiara, laurel wreath, #)
      (self   "You tip your $arg$ at $hcname$ stylishly.")
      (target "$mcname$ tip$s$ $mposs$ $arg$ at you stylishly.")
      (rest  "$mcname$ tip$s$ $mposs$ $arg$ at $hcname$ stylishly.")
    )
  )
)

