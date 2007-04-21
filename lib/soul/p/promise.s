(promise
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
    (single
      (no-arguments 
      (self "You promise.")
      (rest "$mcname$ promise$s$.")
      )
      (arguments
        (arguments  the world, on your honour, you really mean it this time, #)
        (self   "You promise $arg$.")
        (rest  "$mcname$ promise$s$ $arg$.")
      )
    )
    (targeted
      (arguments
      (arguments  the world, on your honour, #)
        (self   "You promise $hcname$ $arg$.")
        (target "$mcname$ promise$s$ you $arg$.")
        (rest  "$mcname$ promise$s$ $hcname$ $arg$.")
      )
      (arguments you really mean it this time)
        (self   "You promise $hcname$ you really mean it this time.")
        (target "$mcname$ promise$s$ you $mpronoun$ really means it this time.")
        (rest  "$mcname$ promise$s$ $hcname$ $mpronoun$ really means it this 
                time.")
      )
   )
)
