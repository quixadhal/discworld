(sass
  (pattern "<indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (targeted
    (no-arguments
      (self "You sass $hcname$.")
      (target "$mcname$ sasses you.")
      (rest "$mcname$ sass $hcname$.")
    )
    (arguments
      (arguments the hoopy frood, brightly, exuberantly,
                 without a towel, over the moon, mangoly,
                 bananaly, applely, pineapplely, avacadoly)
      (self "You sass $arg$ at $hcname$.")
      (target "$mcname$ sasses $arg$ at you.")
      (rest "$mcname$ sasses $arg$ at $hcname$.")
    )
  )
)

