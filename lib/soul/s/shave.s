(shave
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments head, face, legs, armpits, goatee, poodle, womble, 
                 eyebrows, sideburns, moustache, beard, cat, #)
      (self "You shave your $arg$.")
      (rest "$mcname$ shave$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments head, face, legs, armpits, goatee, poodle, womble, 
                 eyebrows, sideburns, moustache, beard, cat, #)
      (self   "You shave $hcname$'s $arg$.") 
      (target "$mcname$ shave$s$ your $arg$.")
      (rest  "$mcname$ shave$s$ $hcname$'s $arg$.")
    )
  )
)
/* by Saffra 4-26-00 */
