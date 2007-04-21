(hop
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You hop around a bit.")
      (rest "$mcname$ hop$s$ around a bit.")
    )
  )
  (targeted
    (arguments
      (arguments  arms,pool,house,car,bed,frog,television,pocket,#)
      (self   "You hop into $mhcname$ $arg$.")
      (target "$mcname$ hop$s$ into your $arg$.")
      (rest  "$mcname$ hop$s$ into $mhcname$ $arg$.")
    )
  )
)

