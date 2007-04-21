(salaam
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You salaam.")
      (rest "$mcname$ salaams.")
    )
    (arguments
      (arguments  happily, sadly, with open arms, annoyingly, impatiently, 
                  ecstatically, carefully, courteously, hysterically,
                  reservedly, warmly, coldly, insolently, curtly,             
                  absentmindedly, slightly, stylishly, smartly, #)
      (self   "You salaam $arg$.")
      (target "$mcname$ salaams $arg$.")
      (rest   "$mcname$ salaams $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You salaam $hcname$.")
      (target "$mcname$ salaams you.")
      (rest   "$mcname$ salaams $hcname$.")
    )
    (arguments
      (arguments  happily, sadly, with open arms, annoyingly, impatiently, 
                  ecstatically, carefully, courteously, hysterically,
                  reservedly, warmly, coldly, insolently, curtly,             
                  absentmindedly, slightly, stylishly, smartly, #)
      (self   "You salaam $hcname$ $arg$.")
      (target "$mcname$ salaams you $arg$.")
      (rest  "$mcname$ salaams $hcname$ $arg$.")
    )
  )
)

/* Saffra, made single no-args possible, 10-11-00. */