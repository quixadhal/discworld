(grumble
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You grumble.")
      (rest "$mcname$ grumble$s$.")
    )
    (arguments
      (arguments
         quietly,bitterly,menacingly,halfheartedly,seductively,suggestively,loudly,
         annoyingly,devastatingly,carefully,wildly,more or less,         
         silently,sarcastically,falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,#)
      (self "You grumble $arg$.")
      (rest "$mcname$ grumble$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You grumble at $hcname$.")
      (target "$mcname$ grumble$s$ at you.")
      (rest  "$mcname$ grumble$s$ at $hcname$.")
    )
    (arguments
      (arguments
         quietly,bitterly,menacingly,halfheartedly,seductively,suggestively,loudly,
         annoyingly,devastatingly,carefully,wildly,more or less,
         silently,sarcastically,falsely,manically,evilly,wickedly,insanely,
         mischievously,devilishly,impishly,cheekily,idiotically,
         tentatively,wryly,sceptically,#)
      (self   "You grumble $arg$ at $hcname$.")
      (target "$mcname$ grumble$s$ $arg$ at you.")
      (rest  "$mcname$ grumble$s$ $arg$ at $hcname$.")
    )
  )
)

