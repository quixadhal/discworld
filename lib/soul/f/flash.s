(flash
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You flash your naughty bits!")
      (rest "$mcname$ flash$s$ $mposs$ naughty bits!")
    )
    (arguments
      (arguments frogs,clocks,goods,stock,#)
      (self "You flash your $arg$.")
      (rest "$mcname$ flash$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You flash your naughty bits at $hcname$!")
      (target "$mcname$ flash$s$ $mposs$ naughty bits at you!")
      (rest  "$mcname$ flash$s$ $mposs$ naughty bits at $hcname$!")
    )
    (arguments
      (arguments frogs,clocks,goods,stock,#)
      (self   "You flash your $arg$ at $hcname$.")
      (target "$mcname$ flash$s$ $mposs$ $arg$ at you.")
      (rest  "$mcname$ flash$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)
/* Hobbes, 970520 */
	