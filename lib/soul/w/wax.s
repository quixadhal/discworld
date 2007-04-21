(wax
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments head, face, legs, armpits, goatee, poodle, womble, 
                 eyebrows, sideburns, moustache, beard, cat, crayons,
                 apples, runners, skis, carriage, #)
      (self "You wax your $arg$.")
      (rest "$mcname$ wax$s$ $mposs$ $arg$.")
    )
    (arguments
      (arguments poetic, philosophical, wry, sarcastic, on, off, and wanes)
      (self "You wax $arg$.")
      (rest "$mcname$ wax$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments head, face, legs, armpits, goatee, poodle, womble, 
                 eyebrows, sideburns, moustache, beard, cat, crayons,
                 apples, #)
      (self   "You wax $hcname$'s $arg$.") 
      (target "$mcname$ wax$s$ your $arg$.")
      (rest  "$mcname$ wax$s$ $hcname$'s $arg$.")
    )
    (arguments
      (arguments poetic, philosophical, wry, sarcastic, on, off, and wanes)
      (self "You wax $arg$ at $hcname$.")
      (target "$mcname$ wax$s$ $arg$ at you.")
      (rest "$mcname$ wax$s$ $arg$ at $hcname$.")
    )
  )
)
/* by Saffra 2-12-02 for Lewwwwwda */


