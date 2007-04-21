(stroke
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You stroke your chin thoughtfully.")
      (rest "$mcname$ stroke$s$ $mposs$ chin thoughtfully.")
    )
    (arguments
      (arguments  ear, nose, cheek, hand, forehead, eyebrow, 
                  finger, foot, toe, fingernail, back, spine, arm, thigh, 
                  upper thigh, calf, elbow, knee, fingers, neck, 
                  shoulder, palm, toes, hair, chicken, ego, beard, goatee, 
                  moustache, #)
      (self   "You stroke your $arg$.")
      (rest   "$mcname$ stroke$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You stroke $hcname$.")
      (target "$mcname$ stroke$s$ you.")
      (rest  "$mcname$ stroke$s$ $hcname$.")
         )
    (arguments
      (arguments  gently, warmly, softly, lovingly, roughly, lightly,
                  quickly, slowly)
      (self   "You stroke $hcname$ $arg$.")
      (target "$mcname$ stroke$s$ you $arg$.")
      (rest   "$mcname$ stroke$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  ear, nose, cheek, hand, forehead, eyebrow, 
                  finger, foot, toe, fingernail, back, spine, arm, thigh, 
                  upper thigh, calf, elbow, knee, fingers, neck, 
                  shoulder, palm, toes, hair, chicken, ego, beard, goatee, 
                  moustache, #)
      (self   "You stroke $mhcname$ $arg$.")
      (target "$mcname$ stroke$s$ your $arg$.")
      (rest   "$mcname$ stroke$s$ $mhcname$ $arg$.")
    )
  )
)

/* Hobbes 970513*/
