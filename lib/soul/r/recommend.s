(recommend
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> [to] <indirect:any-living>")
  (single
    (no-arguments
      (self "You recommend that.")
      (rest "$mcname$ recommend$s$ that.")
    )
    (arguments
      (arguments  don't, doesn't)
      (self "You don't recommend that.")
      (rest "$mcname$ doesn't recommend that.")
    )
  )
  (targeted
    (no-arguments
      (self   "You recommend $hcname$.")
      (target "$mcname$ recommend$s$ you.")
      (rest   "$mcname$ recommend$s$ $hcname$.")
    )
    (arguments
      (arguments  yourself, someone else, a psychologist, a psychiatrist, 
                  a proctologist, a good enema, #)
      (self   "You recommend $arg$ to $mhcname$.")
      (target "$mcname$ recommend$s$ $arg$ to you.")
      (rest   "$mcname$ recommend$s$ $arg$ to $mhcname$.")
    )
  )
)

