(arp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You arp.")
      (rest "$mcname$ arp$s$.")
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
                 warily,sardonically,lazily,serenely,disappointedly)
      (self "You arp $arg$.")
      (rest "$mcname$ arp$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You arp at $hcname$.")
      (target "$mcname$ arp$s$ at you.")
      (rest  "$mcname$ arp$s$ at $hcname$.")
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
                 arpishly,outlandishly,sulamoreishly,like a VW Beetle,
                 erratically,sardonically,warily,lazily,serenely,
                 disappointedly)
      (self   "You arp $arg$ at $hcname$.")
      (target "$mcname$ arp$s$ $arg$ at you.")
      (rest  "$mcname$ arp$s$ $arg$ at $hcname$.")
    )
  )
)

