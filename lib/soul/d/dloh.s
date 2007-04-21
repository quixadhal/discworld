(dloh
  (pattern "[onto] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments nose,#)
      (self "You hold your $arg$.")
      (rest "$mcname$ hold$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hold $hcname$.")
      (target "$mcname$ hold$s$ you.")
      (rest  "$mcname$ hold$s$ $hcname$.")
    )
    (arguments
      (arguments  close, tightly, warmly, gently, lovingly, back, up, #)
      (self   "You hold $hcname$ $arg$.")
      (target "$mcname$ hold$s$ you $arg$.")
      (rest  "$mcname$ hold$s$ $hcname$ $arg$.")
    )
  )
)

