/*-- Anfassen verbieten --*/

//Bestimmte Objekte k?nnen nicht mehr angefasst werden.

#strict 2
#appendto LCKR
#appendto LTBL
#appendto FLGH


/* Initialisierung */

public func Initialize() 
{
  SetObjectLayer(this());
  return(_inherited());
}