/*-- Flaggenw�hler --*/

//Spieler erhalten bei Besitznahme ein Helikoptermen�.

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