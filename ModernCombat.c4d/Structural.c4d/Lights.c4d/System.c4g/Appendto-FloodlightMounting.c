/*-- Scheinwerferhalterung --*/

//Erweiterung für automatische Scheinwerferrotation.

#strict 2
#appendto FLGH

public func Set(bool fAutoRotate, bool fRotationDirection, bool fStatic, int iRotation, iOwnRotation)
{
  pLight->SetAutoRotation(fAutoRotate, fRotationDirection);

  if(iRotation || iOwnRotation)
    SetRotation(iRotation, iOwnRotation);

  if(fStatic)
    SetCategory(C4D_StaticBack);
  else
    SetCategory(C4D_Vehicle);
}