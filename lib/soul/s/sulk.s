(sulk
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You sulk in the corner.")
      (rest "$mcname$ sulk$s$ in the corner.")
    )
    (arguments
      (arguments sulkily, stubbornly, whimperingly, whingingly, childishly, 
                 under the desk, under the bed, under the chair, in the closet,
                 #)
      (self "You sulk $arg$.")
      (rest "$mcname$ sulk$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sulk in the corner away from $hcname$.")
      (target "$mcname$ sulk$s$ in the corner away from you.")
      (rest   "$mcname$ sulk$s$ in the corner away from $hcname$.")
    )
    (arguments
      (arguments sulkily, stubbornly, whimperingly, whingingly, childishly, 
                 under the desk, under the bed, under the chair, in the closet,
                 #)
      (self   "You sulk $arg$ at $hcname$.")
      (target "$mcname$ sulk$s$ $arg$ at you.")
      (rest   "$mcname$ sulk$s$ $arg$ at $hcname$.")
    )
  )
)

