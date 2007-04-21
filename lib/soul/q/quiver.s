(quiver
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You quiver.")
      (rest "$mcname$ quiver$s$.")
    )
    (arguments
      (arguments tensely, in anticipation, tremblingly, #)
      (self "You quiver $arg$.")
      (rest "$mcname$ quiver$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You quiver at $hcname$.")
      (target "$mcname$ quiver$s$ at you.")
      (rest  "$mcname$ quiver$s$ at $hcname$.")
    )
    (arguments
      (arguments tensely, in anticipation, tremblingly, #)
      (self   "You quiver $arg$ at $hcname$.")
      (target "$mcname$ quiver$s$ $arg$ at you.")
      (rest  "$mcname$ quiver$s$ $arg$ at $hcname$.")
    )
  )
)

/* Gave more uses, Saffra 04/22/03 */
