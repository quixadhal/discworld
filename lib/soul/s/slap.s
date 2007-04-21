(slap
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You slap your forehead.")
      (rest "$mcname$ slap$s$ $mposs$ forehead.")
    )
  )
  (targeted
    (no-arguments
      (self   "You slap $hcname$.")
      (target "$mcname$ slap$s$ you.")
      (rest  "$mcname$ slap$s$ $hcname$.")
    )
    (arguments
      (arguments  hard,harder,really hard,incredibly hard,
                  across the belly with a wet trout,with a large haddock,
                  with a frog,with a gauntlet,twice,viciously,carefully,
                  with a scantily clad shark,lovingly)
      (self   "You slap $hcname$ $arg$.")
      (target "$mcname$ slap$s$ you $arg$.")
      (rest  "$mcname$ slap$s$ $hcname$ $arg$.")
    )
  )
)

