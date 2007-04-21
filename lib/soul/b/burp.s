(burp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You burp.")
      (rest "$mcname$ burp$s$.")
    )
    (arguments
      (arguments disgustingly,faintly,loudly,rudely,quickly,
      happily,annoyingly,lovingly,humbly,
      manically,absentmindedly,drunkenly,disturbingly,demonically,pleasantly,
      serenely,viciously,humbly,nastily,#)
      (self "You burp $arg$.")
      (rest "$mcname$ burp$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You burp at $hcname$.")
      (target "$mcname$ burp$s$ at you.")
      (rest  "$mcname$ burp$s$ at $hcname$.")
    )
    (arguments
      (arguments disgustingly,faintly,loudly,rudely,quickly,
      happily,annoyingly,lovingly,humbly,manically,absentmindedly,
      drunkenly,disturbingly,demonically,pleasantly,
      serenely,viciously,humbly,nastily,#)
      (self   "You burp $arg$ at $hcname$.")
      (target "$mcname$ burp$s$ $arg$ at you.")
      (rest  "$mcname$ burp$s$ $arg$ at $hcname$.")
    )
  )
)

