/*-- Anfassen verbieten --*/

//Bestimmte Objekte können nicht mehr angefasst werden.

#strict 2
#appendto FLGH
#appendto LTBL


/* Initialisierung */

public func Initialize() 
{
  SetObjectLayer(this());
  return(_inherited());
}