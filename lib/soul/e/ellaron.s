(ellaron
  (pattern "[around] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ellaron around.")
      (rest "$mcname$ ellaron$s$ around.")
    )
    (arguments
      (arguments #)
      (self "You ellaron around $arg$.")
      (rest "$mcname$ ellaron$s$ around $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments expertly, without really knowing how, drunkenly,
                 happily, backwards, like Ellaron, wearing a fez,
                 and down to the pub, professionally, with your frog)
      (self   "You ellaron around $hcname$ $arg$.")
      (target "$mcname$ ellaron$s$ around you $arg$.")
      (rest   "$mcname$ ellaron$s$ around $hcname$ $arg$.")
    )
  )
)

