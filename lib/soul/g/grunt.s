(grunt
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You grunt.")
      (rest "$mcname$ grunt$s$.")
    )
    (arguments
      (arguments loudly,evilly,wickedly,insanely,mischievously,
        devilishly,impishly,cheekily,idiotically,tentatively,wryly,
        sceptically,softly,#)
      (self "You grunt $arg$.")
      (rest "$mcname$ grunt$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You grunt at $hcname$.")
      (target "$mcname$ grunt$s$ at you.")
      (rest  "$mcname$ grunt$s$ at $hcname$.")
    )
    (arguments
      (arguments  loudly,evilly,wickedly,insanely,mischievously,
        devilishly,impishly,cheekily,idiotically,wryly,
        sceptically,softly,#)
      (self   "You grunt $arg$ at $hcname$.")
      (target "$mcname$ grunt$s$ $arg$ at you.")
      (rest  "$mcname$ grunt$s$ $arg$ at $hcname$.")
    )
  )
)
