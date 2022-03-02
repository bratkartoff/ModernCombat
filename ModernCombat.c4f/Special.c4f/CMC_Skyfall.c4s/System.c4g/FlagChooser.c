/*-- Flaggenwähler --*/

//Spieler erhalten bei Besitznahme ein Helikoptermenü.

#strict 2
#appendto OSPW


public func Spawn()
{
  if(!Contents()) return RemoveObject();

  var obj = Contents();
  Exit(obj);

  CreateHelicopterSpawner(obj,iClass);

  RemoveObject();

  return 1;
}