(snewbie
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments clueless, lost, without a clue, mindless, annoying, froglike)
      (self "You wander around like a newbie, looking completely and utterly $arg$.")
      (rest "$mcname$ wander$s$ around like a newbie, looking completely and utterly $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments clueless, lost, without a clue, mindless, annoying, froglike)
      (self   "You turn $hcname$ into a newbie, who looks utterly and completely $arg$.")
      (target "$mcname$ turn$s$ you into a newbie.  You look completely and utterly $arg$.")
      (rest   "$mcname$ turn$s$ $hcname$ into a newbie, who looks completely and utterly $arg$.")
    )
  )
)

