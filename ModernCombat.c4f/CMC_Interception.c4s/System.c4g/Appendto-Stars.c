/*-- Sternenhimmel --*/

//Sterne werden nur ab einer bestimmten Höhe erzeugt.

#strict
#appendto STRS

private func Dusk()
{
  //Alle Sterne plaziert
  if(GetActTime() > MaxCount)
    return(SetAction("Night"));
  //Stern hinzufügen
  CreateObject(STAR, Random(LandscapeWidth()), RandomX(0,RandomX(300,600)), -1);
  return(1);
}