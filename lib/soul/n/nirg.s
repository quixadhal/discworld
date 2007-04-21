(nirg
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You nirg.")
      (rest "$mcname$ nirg.")
    )
    (arguments
      (arguments manically,evilly,wickedly,insanely,mischievously,
                 devilishly,impishly,cheekily,idiotically,tentatively,
                 wryly,sceptically)
      (self "You nirg $arg$.")
      (rest "$mcname$ snirg $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You nirg at $hcname$.")
      (target "$mcname$ snirg at you.")
      (rest   "$mcname$ snirg at $hcname$.")
    )
    (arguments
      (arguments manically,evilly,wickedly,insanely,mischievously,
                 devilishly,impishly,cheekily,idiotically,wryly,
                 sceptically)
      (self   "You nirg $arg$ at $hcname$.")
      (target "$mcname$ snirg $arg$ at you.")
      (rest   "$mcname$ snirg $arg$ at $hcname$.")
    )
  )
)

