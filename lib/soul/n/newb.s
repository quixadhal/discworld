(newb
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments clueless,lost,without a clue,lost,mindless,annoying,froglike)
      (self "You wander around like a newbie, completely and utterly $arg$.")
      (rest "$mcname$ wander$s$ around like a newbie, completely and utterly $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments clueless,mindless,froglike,lost,stupid)
      (self   "You turn $hcname$ into a newbie, $hpronoun$ look$s$ utterly and completely $arg$.")
      (target "$mcname$ turn$s$ you into a newbie, you look completely and utterly $arg$.")
      (rest   "$mcname$ turn$s$ $hcname$ into a newbie, $hpronoun$ look$s$ completely and utterly $arg$.")
    )
  )
)

