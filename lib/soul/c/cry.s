(cry
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [on] <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "Wahhhhhh.")
      (rest "$mcname$ cries loudly.")
    )
  )
  (targeted
    (arguments
      (arguments shoulder,chest,breast)
      (self "You cry on $mhcname$ $arg$.")
      (target "$mcname$ cries on your $arg$.")
      (rest "$mcname$ cries on $mhcname$ $arg$.")
    )
  )
)

