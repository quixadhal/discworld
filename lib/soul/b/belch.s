#  -*- LPC -*- #
#
# $Locker:  $
# $Id: belch.s,v 1.2 2003/04/24 01:54:11 presto Exp $
#
#
#

(belch
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You belch.")
      (rest "$mcname$ belch$s$.")
    )
    (arguments
      (arguments disgustingly,faintly,loudly,rudely,quickly,
      happily,annoyingly,lovingly,humbly,
      manically,absentmindedly,drunkenly,disturbingly,demonically,pleasantly,
              serenely, viciously, humbly, nastily, like Belcar, #)
      (self "You belch $arg$.")
      (rest "$mcname$ belch$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You belch at $hcname$.")
      (target "$mcname$ belch$s$ at you.")
      (rest  "$mcname$ belch$s$ at $hcname$.")
    )
    (arguments
      (arguments disgustingly,faintly,loudly,rudely,quickly,
      happily,annoyingly,lovingly,humbly,manically,absentmindedly,
      drunkenly,disturbingly,demonically,pleasantly,
             serenely, viciously, humbly, nastily, like Belcar, #)
      (self   "You belch $arg$ at $hcname$.")
      (target "$mcname$ belch$s$ $arg$ at you.")
      (rest  "$mcname$ belch$s$ $arg$ at $hcname$.")
    )
  )
)

