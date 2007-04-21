(chuck
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You chuck $hcname$ gently under the chin.")
      (target "$mcname$ chucks $hcname$ gently under the chin.")
      (rest   "$mcname$ chucks $hcname$ gently under the chin.")
    )
    (arguments
      (arguments gently,lovingly,happily,smugly,hopefully,ironically,sweetly)
      (self   "You chuck $hcname$ $arg$ under the chin.")
      (target "$mcname$ chucks you $arg$ under the chin.")
      (rest   "$mcname$ chucks $hcname$ $arg$ under the chin.")
    )
  )
)

/* Hobbes, 961027. Suggested by Emily */ 