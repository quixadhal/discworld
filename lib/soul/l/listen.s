(listen
  (pattern "<string>")
  (pattern "[to] <indirect:any-living>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single 
    (no-arguments
      (self "You listen carefully.")
      (rest "$mcname$ listen$s$ carefully.")
    )
    (arguments
      (arguments  intently, carefully, boredly, patiently, silently, rudely,
                  to the birds, to the trees, to the voices in your head, #)
      (self "You listen $arg$.")
      (rest "$mcname$ listen$s$ $arg$.")
    )
  )  
  (targeted
    (no-arguments
      (self   "You listen carefully to $hcname$.")
      (target "$mcname$ listen$s$ carefully to you.")
      (rest   "$mcname$ listen$s$ carefully to $hcname$.")
    )
    (arguments
      (arguments  intently, carefully, boredly, patiently, silently, rudely,
                impatiently, with bated breath, #)
      (self   "You listen $arg$ to $hcname$.")
      (target "$mcname$ listen$s$ $arg$ to you.")
      (rest   "$mcname$ listen$s$ $arg$ to $hcname$.")
    )
  )
)

