(tease
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You tease yourself.")
      (rest "$mcname$ teases $mobj$self, $mpronoun$ appears to enjoy it.")
    )
  )
  (targeted
    (no-arguments
      (self   "You tease $hcname$.")
      (target "$mcname$ teases you.")
      (rest  "$mcname$ teases $hcname$.")
    )
    (arguments
      (arguments  mercilessly, insistently, annoyingly, lovingly,
                  mischievously, rudely, carefully, quickly, slowly,
                  halfheartedly, idly, obnoxiously, #)
      (self   "You tease $hcname$ $arg$.")
      (target "$mcname$ teases you $arg$.")
      (rest  "$mcname$ teases $hcname$ $arg$.")
    )
  )
)

