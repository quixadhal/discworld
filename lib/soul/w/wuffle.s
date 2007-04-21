(wuffle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You wuffle about.")
      (rest  "$mcname$ wuffles about.")
    )   
    (arguments
      (arguments affectionately, warmly, gently, absentmindedly, carefully, 
                  warmly, absolutely, triumphantly, capriciously, suspiciously,
                  drunkenly, astheticaly, insolently, faintly, delicately, #)
      (self  "You wuffle $arg$.")
      (rest  "$mcname$ wuffle$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  affectionately, warmly, gently, absentmindedly, carefully, 
                  warmly, absolutely, triumphantly, capriciously, suspiciously,
                  drunkenly, astheticaly, insolently, faintly, delicately, #)
      (self   "You wuffle $hcname$ $arg$.")
      (target "$mcname$ wuffle$s$ you $arg$.")
      (rest   "$mcname$ wuffle$s$ $hcname$ $arg$.")
    )
  )
)

