(sue
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You feel like sueing the pants off someone.")
      (rest "$mcname$ look$s$ mad enough to sue.")
    )
  )
  (targeted
    (no-arguments
      (self   "You attempt to sue $hcname$ for all $hpronoun$ $tverb$is,are$ worth.")
      (target "$mcname$ attempt$s$ to sue you for all you are worth.")
      (rest   "$mcname$ attempt$s$ to sue $hcname$ for all $hpronoun$ $tverb$is,are$ worth.")
    )
    (arguments
      (arguments  for gobs of money, for having a name like that, for divorce,
                  for custody, for peace, #)
      (self   "You attempt to sue $hcname$ $arg$.")
      (target "$mcname$ attempt$s$ to sue you $arg$.")
      (rest   "$mcname$ attempt$s$ to sue $hcname$ $arg$.")
    )
  )
)

