(mourn
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You mourn.")
      (rest   "$mcname$ mourn$s$.")
    )
    (arguments
      (arguments the slow death of frogs,#)
      (self "You mourn $arg$.")
      (rest "$mcname$ mourn$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You mourn $hcname$.")
      (target "$mcname$ mourn$s$ you.")
      (rest   "$mcname$ mourn$s$ $hcname$.")
    )
    (arguments
    (arguments mournfully, sadly, sincerely, apologetically, ghoulishly, 
               macabrely, gleefully, smugly, again, not at all, #)
      (self   "You mourn $hcname$ $arg$.")
      (target "$mcname$ mourn$s$ you $arg$.")
      (rest   "$mcname$ mourn$s$ $hcname$ $arg$.")
    )
  )
)
