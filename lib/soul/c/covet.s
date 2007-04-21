(covet
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You covet $hcname$.")
      (target "$mcname$ covets you.")
      (rest  "$mcname$ covets $hcname$.")
    )
    (arguments
      (arguments  neighbor, wife, husband, computer, brownies, cookies,
                  net connection, skills, stats, fabulous hair,  #)
      (self   "You covet $mhcname$ $arg$.")
      (target "$mcname$ covets your $arg$.")
      (rest  "$mcname$ covets $mhcname$ $arg$.")
    )
  )
)

/* Saffra 2-20-00 */
