(shine
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shine brightly.")
      (rest "$mcname$ shine$s$ brightly.")
    )
  )
  (targeted
      (arguments
      (arguments brightly,lovely,happily,mischievously,cheekily,idiotically,wryly,sceptically,
                 derisively,annoyingly,absentmindedly,whilst you work,distractedly,pointlessly,
                 silently,loudly,rudely,with laughter,vaguely,obviously,like a sun,like Magpie,
                 blindingly,innocently,proudly,charmingly,seductively,#)
      (self   "You shine $arg$ at $hcname$.")
      (target "$mcname$ shine$s$ $arg$ at you.")
      (rest  "$mcname$ shine$s$ $arg$ at $hcname$.")
    )
  )
)

