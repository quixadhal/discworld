(wibble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wibble.")
      (rest "$mcname$ wibbles.")
    )
    (arguments
      (arguments  like bil, loudly, constantly, annoyingly, around, up, down,
                  sideways, willfully, #)
      (self "You wibble $arg$.")
      (rest "$mcname$ wibble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You wibble at $hcname$.")
      (target "$mcname$ wibble$s$ at you.")
      (rest "$mcname$ wibble$s$ at $hcname$.")
    )
    (arguments 
      (arguments  like bil, loudly, constantly, annoyingly, around, up, down,
                  sideways, willfully, #)
      (self   "You wibble $arg$ at $hcname$.")
      (target "$mcname$ wibble$s$ $arg$ at you.")
      (rest   "$mcname$ wibble$s$ $arg$ at $hcname$.")
    )
  )
)
