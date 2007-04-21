(salute
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You salute smartly.")
      (rest "$mcname$ salutes smartly.")
    )
    (arguments
      (arguments  slowly, quickly, respectfully, insolently, mockingly, #)
      (self "You salute $arg$.")
      (rest "$mcname$ salutes $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You salute $hcname$.")
      (target "$mcname$ salutes you.")
      (rest   "$mcname$ salutes $hcname$.")
    )
    (arguments
      (arguments  slowly, quickly, respectfully, insolently, mockingly, #)
      (self   "You salute $hcname$ $arg$.")
      (target "$mcname$ salutes you $arg$.")
      (rest   "$mcname$ salutes $hcname$ $arg$.")
    )
  )
)

