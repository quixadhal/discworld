(wish
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wish.")
      (rest "$mcname$ wishes.")
    )
    (arguments
      (arguments upon a star, it were true, it wasn't so, #)
      (self "You wish $arg$.")
      (rest "$mcname$ wish$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  birthday, Hogswatch Day, Soul Cake Tuesday, 
                  Sektobers Fool's Day, Small Gods Day)
      (self   "You wish $hcname$ a happy $arg$.")
      (target "$mcname$ wish$s$ you a happy $arg$.")
      (rest   "$mcname$ wish$s$ $hcname$ a happy $arg$.")
    )
    (arguments
      (arguments  good luck, good night, good-bye, #)
      (self "You wish $hcname$ $arg$.")
      (target "$mcname$ wish$s$ you $arg$.")
      (rest "$mcname$ wish$s$ $hcname$ $arg$.")
    )
  )
)

/*Implemeted by Hobbes, suggested by Deutha 961014 */
