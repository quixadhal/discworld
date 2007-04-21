(cringe
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You cringe.")
      (rest "$mcname$ cringes.")
    )
    (arguments
    (arguments terror,embarrassment,anticipation,#)
      (self "You cringe in $arg$.")
      (rest "$mcname$ cringes in $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You cringe at $hcname$.")
      (target "$mcname$ cringes at you.")
      (rest  "$mcname$ cringes at $hcname$.")
    )
    (arguments
      (arguments terror,embarrassment,anticipation,#)
      (self   "You cringe in $arg$ at $hcname$.")
      (target "$mcname$ cringes in $arg$ at you.")
      (rest  "$mcname$ cringes in $arg$ at $hcname$.")
    )
  )
)

