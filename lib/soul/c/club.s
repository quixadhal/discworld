(club
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You club yourself over the head.")
      (rest "$mcname$ clubs $mobj$self over the head.")
    )
      (arguments
      (arguments like a harp seal, with a harp seal, like a
                 Canadian sailor, with a paper umbrella,
                 over the head, club-club-clubbity-club-club,
                 extremely painfully, on the noggin, with a
                nail in, with a heavy object, #)
      (self "You club yourself $arg$.")
      (rest "$mcname$ clubs $mobj$self $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You club $hcname$ over the head.")
      (target "$mcname$ clubs you over the head.")
      (rest  "$mcname$ clubs $hcname$ over the head.")
    )
    (arguments
      (arguments like a harp seal, with a harp seal, like a
                 Canadian sailor, with a paper umbrella,
                 over the head, club-club-clubbity-club-club,
                 extremely painfully, on the noggin, with a
                 nail in, with a heavy object, #)
      (self   "You club $hcname$ $arg$.")
      (target "$mcname$ clubs you $arg$.")
      (rest  "$mcname$ clubs $hcname$ $arg$.")
    )
  )
)
      (arguments like a harp seal, with a harp seal, like a
