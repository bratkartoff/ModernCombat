/*-- Wrack --*/

#strict 2


/* Initialisierung */

public func Initialize()
{
  SetAction("Destroyed");
}

/* Aufschlag */

protected func Hit()
{
  Sound("WreckHeavyHit*.ogg");
}