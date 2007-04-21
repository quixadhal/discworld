(worship
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You worship Pinkfish.")
      (rest "$mcname$ worship$s$ Pinkfish.")
    )
    (arguments
    (arguments Pishe, Fish, Sek, Gufnork, Sessifet, Hat, Gapp, #)
      (self "You worship $arg$.")
      (rest "$mcname$ worship$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You worship $hcname$.")
      (target "$mcname$ worship$s$ you.")
      (rest   "$mcname$ worship$s$ $hcname$.")
    )
    (arguments
      (arguments slightly, sarcastically, uncertainly, fawningly, sickeningly,     
                 embarrassingly, #)
      (self "You worship $hcname$ $arg$.")
      (target "$mcname$ worship$s$ you $arg$.")
      (rest   "$mcname$ worship$s$ $hcname$ $arg$.")
    ) 
  )
)
