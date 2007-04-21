(wobble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wobble.")
      (rest "$mcname$ wobble$s$.")
    )
    (arguments
      (arguments  manically,around a bit,like jelly,constantly,annoyingly,
         around,up,down,sideways, evilly,wickedly,insanely,mischievously,
       devilishly,impishly,cheekily,idiotically,tentatively,wryly,sceptically,#)
      (self "You wobble $arg$.")
      (rest "$mcname$ wobble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wobble at $hcname$.")
      (target "$mcname$ wobble$s$ at you.")
      (rest  "$mcname$ wobble$s$ at $hcname$.")
    )
     (arguments
      (arguments  manically,around a bit,like jelly,constantly,annoyingly,
        around,up,down,sideways,evilly,wickedly,insanely,mischievously,
        devilishly,impishly,cheekily,idiotically,tentatively,wryly,sceptically,#)
      (self   "You wobble $arg$ at $hcname$.")
      (target "$mcname$ wobble$s$ $arg$ at you.")
      (rest  "$mcname$ wobble$s$ $arg$ at $hcname$.")
    )
  )
)

