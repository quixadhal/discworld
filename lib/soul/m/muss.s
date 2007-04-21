(muss
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You muss $mhcname$ hair.")
      (target "$mcname$ muss$s$ your hair.")
      (rest   "$mcname$ muss$s$ $mhcname$ hair.")
    )
    (arguments
      (arguments slowly,lovingly,with a brick,patronisingly,
                 suggestively,just like Kylie would)
      (self   "You muss $mhcname$ hair $arg$.")
      (target "$mcname$ muss$s$ your hair $arg$.")
      (rest   "$mcname$ muss$s$ $mhcname$ hair $arg$.")
    )
  )
)

