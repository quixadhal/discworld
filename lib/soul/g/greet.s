(greet
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You greet $hcname$.")
      (target "$mcname$ greet$s$ you.")
      (rest  "$mcname$ greet$s$ $hcname$.")
    )
    (arguments
      (arguments  happily, sadly,with open arms, annoyingly, impatiently, 
                  ecstatically, carefully, courteously, hysterically, 
                  reservedly, warmly, coldly, insolently, curtly, 
                  absentmindedly, slightly, serenely, from the moon, 
                  #)
      (self   "You greet $hcname$ $arg$.")
      (target "$mcname$ greet$s$ you $arg$.")
      (rest  "$mcname$ greet$s$ $hcname$ $arg$.")
    )
  )
)

