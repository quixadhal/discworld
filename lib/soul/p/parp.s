(parp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You parp.")
      (rest "$mcname$ parp$s$.")
    )
    (arguments
      (arguments happily,sadly,nervously,wryly,broadly,crookedly,
                 stupidly,idiotically,condescendingly,ironically,
                 patiently,brightly,slightly,nastily,excitedly,
                 strangely,with forced patience,plasticly,carefully,
                 sadistically,lopsidedly,fishily,cattishly,faintly,
                 like a train,angrily,paranoically,innocently,froggily,
                 slyly,weakly,humbly,paradoxically,sarcastically,
                 viciously,garishly,sweetly,innocently,lovingly,
                 dreamily,radiantly,enthusiastically,insolently,
                 quirkily,completely,unconsciously,drunkenly,insanely,
                 cutely,maliciously,smugly,absentmindedly,curtly,
                 disturbingly,unbelievably,quietly,loudly,differently,
                 winningly,in triumph,seductively,tightly,
                 gosh there are lots of arguments,softly,lividly,
                 demonically,warmly,grimly,indolently,ingeniously,
                 wickedly,contentedly,statistically,spasmodically,
                 incandescently,pleasantly,with menace,manically,
                 warily,sardonically,lazily,serenely,disappointedly, sleepily, #)
      (self "You parp $arg$.")
      (rest "$mcname$ parp$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You parp at $hcname$.")
      (target "$mcname$ parp$s$ at you.")
      (rest  "$mcname$ parp$s$ at $hcname$.")
    )
    (arguments
      (arguments happily,sadly,nervously,wryly,broadly,crookedly,
                 stupidly,idiotically,condescendingly,ironically,
                 patiently,brightly,slightly,nastily,excitedly,
                 strangely,with forced patience,plasticly,manically,
                 carefully,sadistically,lopsidedly,fishily,cattishly,
                 faintly,weakly,humbly,paradoxically,sarcastically,
                 viciously,garishly,insolently,quirkily,completely,
                 unconsciously,drunkenly,absentmindedly,curtly,
                 disturbingly,unbelievably,quietly,loudly,differently,
                 winningly,in triumph,seductively,sweetly,innocently,
                 lovingly,dreamily,radiantly,enthusiastically,like a train,
                 angrily,paranoically,innocently,froggily,slyly,
                 macabrely,ruefully,tentatively,like a dog,tightly,
                 gosh there are lots of arguments,softly,insanely,
                 cutely,maliciously,smugly,grimly,lividly,demonically,
                 indolently,ingeniously,wickedly,contentedly,statistically,
                 spasmodically,incandescently,pleasantly,with menace,
                 parpishly,outlandishly,not like Ooeeoo,like a VW Beetle,
                 erratically,sardonically,warily,lazily,serenely,
                 disappointedly, sleepily, #)
      (self   "You parp $arg$ at $hcname$.")
      (target "$mcname$ parp$s$ $arg$ at you.")
      (rest  "$mcname$ parp$s$ $arg$ at $hcname$.")
    )
  )
)

