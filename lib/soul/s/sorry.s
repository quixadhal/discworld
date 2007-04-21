(sorry
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You apologise to $hcname$.")
      (target "$mcname$ apologise$s$ to you.")
      (rest  "$mcname$ apologise$s$ to $hcname$.")
    )
    (arguments
      (arguments  profusely,reservedly,perfunctorily,carefully,insultingly,insolently)
      (self   "You apologise $arg$ to $hcname$.")
      (target "$mcname$ apologise$s$ $arg$ to you.")
      (rest  "$mcname$ apologise$s$ $arg$ to $hcname$.")
    )
  )
)

