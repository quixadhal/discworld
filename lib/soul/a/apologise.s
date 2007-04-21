(apologise
  (pattern "[to] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You apologise.")
      (rest "$mcname$ apologise$s$.")
    )
    (arguments
      (arguments  profusely, happily, sadly, reservedly, carefully, slowly, 
                  perfunctorily, curtly, viciously, sulkily, drunkenly, 
                  unhappily, professionally, pompously, wholeheartedly, 
                  partially, wearily, fishily, facetiously, unnecessarily, 
                  spontaneously, twice, thoroughly, humbly, #)
      (self "You apologise $arg$.")
      (rest "$mcname$ apologise$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You apologise to $hcname$.")
      (target "$mcname$ apologise$s$ to you.")
      (rest   "$mcname$ apologise$s$ to $hcname$.")
    )
    (arguments
      (arguments  profusely, happily, sadly, reservedly, carefully, slowly, 
                  perfunctorily, curtly, viciously, sulkily, drunkenly, 
                  unhappily, professionally, pompously, wholeheartedly, 
                  partially, wearily, fishily, facetiously, unnecessarily, 
                  spontaneously, twice, thoroughly, humbly, #)
      (self   "You apologise $arg$ to $hcname$.")
      (target "$mcname$ apologise$s$ $arg$ to you.")
      (rest   "$mcname$ apologise$s$ $arg$ to $hcname$.")
    )
  )
)

