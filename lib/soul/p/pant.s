(pant
  (pattern "<string>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You pant.")
      (rest "$mcname$ pant$s$.")
    )
    (arguments
      (arguments deeply, silently, desperately, tiredly, sadly, dreamily,
                 happily, melodramatically, wistfully, heavily, suggestively,
                 sarcastically, unconvincingly)
      (self "You pant $arg$.")
      (rest "$mcname$ pant$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
       (self "You pant at $hcname$.")
       (target "$mcname$ pant$s$ at you.")
       (rest "$mcname$ pant$s$ at $hcname$.")
    )
    (arguments
      (arguments deeply, silently, desperately, tiredly, sadly, dreamily,
                 happily, melodramatically, wistfully, heavily, suggestively,
                 sarcastically, unconvincingly)
      (self   "You pant $arg$ at $hcname$.")
      (target "$mcname$ pant$s$ $arg$ at you.")
      (rest   "$mcname$ pant$s$ $arg$ at $hcname$.")
    )
  )
)

