(tattle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
      (arguments Mommy,Daddy)
      (self "You go home to tell your $arg$ on $hcname$.")
      (target "$mcname$ goes home to tell $mposs$ $arg$ on you.")
      (rest "$mcname$ goes home to tell $mposs$ $arg$ on $hcname$.")
    )
  )
)

/* Hobbes 960922 */