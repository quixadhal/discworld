(ear
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You pull a quarter from your ear.")
      (rest "$mcname$ pull$s$ a quarter from $mposs$ ear.")
    )
    (arguments
      (arguments  a pence, a dollar, a royal, a womble, a string of sausages, 
                  a brain cell, #)
      (self "You pull $arg$ from your ear.")
      (rest "$mcname$ pull$s$ $arg$ from $mposs$ ear.")

    )
  )
  (targeted
    (no-arguments
      (self   "You pull a quarter from $mhcname$ ear.")
      (target "$mcname$ pull$s$ a quarter from your ear.")
      (rest "$mcname$ pull$s$ a quarter from $mhcname$ ear.")
    )
    (arguments
      (arguments  a pence, a dollar, a royal, a womble, a string of sausages,  
                  a brain cell, #)
      (self   "You pull $arg$ from $mhcname$ ear.")
      (target "$mcname$ pull$s$ $arg$ from your ear.")
      (rest   "$mcname$ pull$s$ $arg$ from $mhcname$ ear.")
    )
  )
)
/* by Saffra 4-26-00 */
