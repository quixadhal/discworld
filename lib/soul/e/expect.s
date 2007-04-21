(expect
  (pattern "[at] <indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You look expectantly at $hcname$.")
      (target "$mcname$ look$s$ expectantly at you.")
      (rest  "$mcname$ look$s$ expectantly at $hcname$.")
    )
  )
)

