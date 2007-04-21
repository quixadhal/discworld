(phew
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wipe your brow and go: PHEW!")
      (rest "$mcname$ wipe$s$ $mposs$ brow and go$s$: PHEW!")
    )
    (arguments
      (arguments with relief,happily,#)
      (self "You wipe your brow $arg$ and go: PHEW! $arg$.")
      (rest "$mcname$ wipe$s$ $mposs$ brow $arg$ and go$s$: PHEW!")
    )
  )
  (targeted
    (no-arguments
      (self   "You wipe your brow and go: PHEW! at $hcname$.")
      (target "$mcname$ wipe$s$ $mposs$ brow and go$s$: PHEW! at you.")
      (rest  "$mcname$ wipe$s$ $mposs$ brow and go$s$: PHEW! at $hcname$.")
    )
    (arguments
      (arguments  with relief,happily,#)
      (self   "You wipe your brow $arg$ and go: PHEW! at $hcname$.")
      (target "$mcname$ wipe$s$ $mposs$ brow $arg$ and go$s$: PHEW! at you.")
      (rest  "$mcname$ wipe$s$ $mposs$ brow $arg$ and go$s$: PHEW! at $hcname$.")
    )
  )
)

