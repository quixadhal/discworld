(hunt
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You hunt $hcname$.")
      (target "$mcname$ hunt$s$ you.")
      (rest   "$mcname$ hunt$s$ $hcname$.")
    )
    (arguments
      (arguments  up and down the streets)
      (self   "You hunt $arg$ for $hcname$.")
      (target "$mcname$ hunt$s$ $arg$ for you.")
      (rest   "$mcname$ hunt$s$ $arg$ for $hcname$.")
    )
    (arguments
      (arguments  affectionately, nicely, kindly, 
                  with the gleam of revenge in both eyes, relentlessly, #)
      (self   "You hunt $hcname$ $arg$.")
      (target "$mcname$ hunt$s$ you $arg$.")
      (rest   "$mcname$ hunt$s$ $hcname$ $arg$.")
    )
  )
)

