(wake
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments up,up and smell the cat food,the dead,
                 the wombles of the world up)
      (self "You wake $arg$.")
      (rest "$mcname$ wake$s$ up.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wake up $hcname$.")
      (target "$mcname$ wake$s$ you up.")
      (rest   "$mcname$ wake$s$ up $hcname$.")
    )
  )
)

