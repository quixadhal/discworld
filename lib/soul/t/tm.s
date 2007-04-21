(tm
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You masquerade as a giant TM and run around the room.")
      (rest "$mcname$ masquerade$s$ as a giant TM and runs around the room.")
    )
  )
  (targeted
    (arguments
      (arguments  fighting, covert, magic, faith, other, crafts, #)
      (self   "You turn $hcname$ into a giant $arg$ TM, and watch as everyone chases $hobj$.")
      (target "$mcname$ turn$s$ you into a giant $arg$ TM.  Everyone chases you.")
      (rest  "$mcname$ turn$s$ $hcname$ into a giant $arg$ TM.  Everyone chases $hobj$.")
    )
  )
)

/* Saffra rewrote this to be more useful 02 Apr 2003 */
