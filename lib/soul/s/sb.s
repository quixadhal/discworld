(sb
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
        (self "You score-brief.")
        (rest "$mcname$ score-briefs.")
     )
    )
    (targeted
      (no-arguments
    (self "You score-brief $hcname$.")
    (target "$mcname$ score-briefs you.")
   (rest "$mcname$ score-briefs $hcname$.")
   )
  )
)
