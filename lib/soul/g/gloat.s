(gloat
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You gloat.")
      (rest "$mcname$ gloat$s$.")
    )
    (arguments
      (arguments manically, evilly, wickedly, insanely, mischievously, 
                 devilishly, impishly, cheekily, idiotically, tentatively, 
                 wryly, skeptically, sickeningly, stupidly, slightly, 
                 strangely,#)
      (self "You gloat $arg$.")
      (rest "$mcname$ gloat$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You gloat at $hcname$.")
      (target "$mcname$ gloat$s$ at you.")
      (rest  "$mcname$ gloat$s$ at $hcname$.")
    )
    (arguments
      (arguments manically, evilly, wickedly, insanely, mischievously, 
                 devilishly, impishly, cheekily, idiotically, tentatively, 
                 wryly, skeptically, sickeningly, stupidly, slightly, 
                 strangely,#)
      (self   "You gloat $arg$ at $hcname$.")
      (target "$mcname$ gloat$s$ $arg$ at you.")
      (rest  "$mcname$ gloat$s$ $arg$ at $hcname$.")
    )
  )
)
