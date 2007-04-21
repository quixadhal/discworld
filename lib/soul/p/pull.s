(pull
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You pull yourself together.")
      (rest "$mcname$ pull$s$ $mobj$self together.")
    )
    (arguments
     (arguments  leg, hair, finger, sleeve, naughty bits, lever, blue lever,  
                 hamstring, leash, beard, jock strap, file, act together, #)
      (self "You pull your $arg$.")
      (rest "$mcname$ pull$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You pull $hcname$ together.")
      (target "$mcname$ pull$s$ you together.")
      (rest   "$mcname$ pull$s$ $hcname$ together.")
    )
    (arguments
     (arguments leg, hair, finger, sleeve, naughty bits, lever, blue lever,  
                hamstring, leash, beard, jock strap, file, act together, #)
      (self "You pull $mhcname$ $arg$.")
      (target "$mcname$ pull$s$ your $arg$.")
      (rest "$mcname$ pull$s$ $mhcname$ $arg$.")
    )
  )
)
/* Saffra 4-26-00 */

