(foo
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments
          vaguely, carefully, slowly, dangerously, quietly, silently,
          sarcastically, falsely, manically, evilly, wickedly, insanely,
          mischievously, devilishly, impishly, cheekily, idiotically,
          tentatively, wryly, skeptically, happily, backwards, sideways, #)
      (self "You foo $arg$.")
      (rest "$mcname$ foo$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You foo at $hcname$.")
      (target "$mcname$ foo$s$ at you.")
      (rest  "$mcname$ foo$s$ at $hcname$.")
    )
    (arguments
      (arguments
          vaguely, carefully, slowly, dangerously, quietly, silently,
          sarcastically, falsely, manically, evilly, wickedly, insanely,
          mischievously, devilishly, impishly, cheekily, idiotically,
          tentatively, wryly, skeptically, happily, backwards, sideways, #)
      (self   "You foo $arg$ at $hcname$.")
      (target "$mcname$ foo$s$ $arg$ at you.")
      (rest  "$mcname$ foo$s$ $arg$ at $hcname$.")
    )
  )
)

