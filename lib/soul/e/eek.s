(eek
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You eek.")
      (rest "$mcname$ eek$s$.")
    )
    (arguments
      (arguments loudly,in shock,in amazement,in the basement,happily,
               sadly,carefully,brightly,manically,evilly,wickedly,insanely,
               mischievously,devilishly,impishly,cheekily,idiotically,
               tentatively,wryly,sceptically,#)
      (self "You eek $arg$.")
      (rest "$mcname$ eek$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You eek at $hcname$.")
      (target "$mcname$ eek$s$ at you.")
      (rest  "$mcname$ eek$s$ at $hcname$.")
    )
    (arguments
      (arguments loudly,in shock,in amazement,in the basement,happily,
               sadly,carefully,brightly,manically,evilly,wickedly,insanely,
               mischievously,devilishly,impishly,cheekily,idiotically,
               tentatively,wryly,sceptically,#)
      (self   "You eek $arg$ at $hcname$.")
      (target "$mcname$ eek$s$ $arg$ at you.")
      (rest  "$mcname$ eek$s$ $arg$ at $hcname$.")
    )
  )
)

