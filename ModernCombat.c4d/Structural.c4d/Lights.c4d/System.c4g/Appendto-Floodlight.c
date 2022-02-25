/*-- Scheinwerfer --*/

//Kompatibilität zum Lichtsystem.

#strict 2
#appendto FLHH


/* Keine Zerstörung */

public func IsBulletTarget(id def)
{
  return false;
}

func Damage()
{
  return;
}

/* Automatische Rotation */

local fAutoRotation;

public func SetAutoRotation(bool fAutoRotate, bool fRotation)
{
  fAutoRotation = fAutoRotate;

  if(fAutoRotation)
    if(!fRotation)
      TurnLeft();
    else
      TurnRight();
}

protected func Turning()
{
  if(EMPShocked()) return;
  var anglediff = Normalize(GetR()-GetR(GetActionTarget()),-180);

  if(fAutoRotation)
  {
    if(turn == -1 && anglediff < -90)
      turn = +1;
    else
      if(turn == +1 && anglediff > 90)
      turn = -1;
  }
  else
    if(turn == -1 && anglediff < -90 || turn == +1 && anglediff > 90)
      return(SetAction("Attach",GetActionTarget()));

  SetR(GetR()+turn);
  RotateLight();
}