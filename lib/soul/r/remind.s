(remind
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You remind $hcname$.")
      (target "$mcname$ remind$s$ you.")
      (rest  "$mcname$ remind$s$ $hcname$.")
    )
    (arguments
      (arguments  #)
      (self   "You remind $hcname$ $arg$.")
      (target "$mcname$ remind$s$ you $arg$.")
      (rest  "$mcname$ remind$s$ $hcname$ $arg$.")
    )
  )
)

/* Hobbes 961204 */
	