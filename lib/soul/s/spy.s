(spy
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You spy on people with rays.")
      (rest "$mcname$ spy$s$ on people with rays.")
    )
    (arguments
    (arguments  with rays, with closed circuit cameras, with 
                telephone bugs, despite the tinfoil hat, with
                crystal balls, through a painting, cunningly,
                cleverly, obviously, incessantly, surreptitiously,
                covertly, like a High Lord, with Ray, #)
      (self "You spy on people $arg$.")
      (rest "$mcname$ spy$s$ on people $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You spy on $hcname$.")
    (target "$mcname$ spy$s$ on you.")
    (rest "$mcname$ spy$s$ on $hcname$.")
  )
  (arguments
  (arguments  with rays, with closed circuit cameras, with 
              telephone bugs, despite the tinfoil hat, with
              crystal balls, through a painting, cunningly,
              cleverly, obviously, incessantly, surreptitiously,
                covertly, like a High Lord, with Ray, #)
    (self "You spy on $hcname$ $arg$." )
    (target "$mcname$ spy$s$ on you $arg$." )
    (rest "$mcname$ spy$s$ on $hcname$ $arg$." )
    )
  )
)
