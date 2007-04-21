(squeak
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You squeak.")
      (rest "$mcname$ squeak$s$.")
    )
    (arguments
      (arguments manically,evilly,affirmatively,excitedly,unhappily,moodily,sulkily,wickedly,insanely,mischievously,devilishly,impishly,cheekily,idiotically,tentatively,wryly,sceptically,happily,like a mouse,like a confused cat, #)
      (self "You squeak $arg$.")
      (rest "$mcname$ squeak$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You squeak at $hcname$.")
      (target "$mcname$ squeak$s$ at you.")
      (rest  "$mcname$ squeak$s$ at $hcname$.")
    )
    (arguments
      (arguments  manically,evilly,affirmatively,excitedly,unhappily,moodily,sulkily,wickedly,insanely,mischievously,devilishly,impishly,cheekily,idiotically,wryly,sceptically,happily,like a mouse,like a confused cat, #)
      (self   "You squeak $arg$ at $hcname$.")
      (target "$mcname$ squeak$s$ $arg$ at you.")
      (rest  "$mcname$ squeak$s$ $arg$ at $hcname$.")
    )
  )
)
/* Hobbes, 96 */