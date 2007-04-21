(titter
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You titter.")
      (rest "$mcname$ titter$s$.")
    )
    (arguments
      (arguments
         quietly,silently,sarcastically,falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,#)
      (self "You titter $arg$.")
      (rest "$mcname$ titter$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You titter at $hcname$.")
      (target "$mcname$ titter$s$ at you.")
      (rest  "$mcname$ titter$s$ at $hcname$.")
    )
    (arguments
      (arguments
         quietly,silently,sarcastically,falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,#)
      (self   "You titter $arg$ at $hcname$.")
      (target "$mcname$ titter$s$ $arg$ at you.")
      (rest  "$mcname$ titter$s$ $arg$ at $hcname$.")
    )
  )
)

