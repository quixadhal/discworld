(pet
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You pet Goldie.")
      (rest "$mcname$ pet$s$ Goldie.")
    )
  )
  (targeted
    (no-arguments
      (self   "You pet $hcname$.")
      (target "$mcname$ pet$s$ you.")
      (rest   "$mcname$ pet$s$ $hcname$.")
    )
    (arguments
      (arguments  affectionately, soothingly, seductively, carefully, nicely,
                  like a cute kitten, like a moose stuck in a tree, #)
      (self   "You pet $hcname$ $arg$.")
      (target "$mcname$ pet$s$ you $arg$.")
      (rest   "$mcname$ pet$s$ $hcname$ $arg$.")
    )
  )
)

