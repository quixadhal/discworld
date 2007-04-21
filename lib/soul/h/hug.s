(hug
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You hug $hcname$.")
      (target "$mcname$ hug$s$ you.")
      (rest  "$mcname$ hug$s$ $hcname$.")
    )
    (arguments
      (arguments  tightly,warmly,gently,close,#,like a huge bear)
      (self   "You hug $hcname$ $arg$.")
      (target "$mcname$ hug$s$ you $arg$.")
      (rest  "$mcname$ hug$s$ $hcname$ $arg$.")
    )
  )
)

