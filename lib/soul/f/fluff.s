(fluff
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You fluff.")
      (rest  "$mcname$ $verb$fluffs,fluff$.")
    )
    (arguments
      (arguments happily,affectionately,warmly,gently,absentmindedly,
         carefully,warmly,absolutely,triumphantly,capriciously,
         suspiciously,drunkenly,aesthetically,insolently,faintly,
         delicately,hysterically,slightly,ecstatically,with a hair dryer,
         with a frog,in the nose,is this something dirty?,
         under a coffee table,#)
      (self   "You fluff $arg$.")
      (rest  "$mcname$ $verb$fluffs,fluff$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fluff $hcname$.")
      (target "$mcname$ $verb$fluffs,fluff$ you.")
      (rest  "$mcname$ $verb$fluffs,fluff$ $hcname$.")
    )
    (arguments
      (arguments happily,affectionately,warmly,gently,absentmindedly,
         carefully,warmly,absolutely,triumphantly,capriciously,
         suspiciously,drunkenly,aesthetically,insolently,faintly,
         delicately,hysterically,slightly,ecstatically,with a hair dryer,
         with a frog,in the nose,is this something dirty?,
         under a coffee table,#)
      (self   "You fluff $hcname$ $arg$.")
      (target "$mcname$ $verb$fluffs,fluff$ you $arg$.")
      (rest  "$mcname$ $verb$fluffs,fluff$ $hcname$ $arg$.")
    )
  )
)
/* Hobbes */

