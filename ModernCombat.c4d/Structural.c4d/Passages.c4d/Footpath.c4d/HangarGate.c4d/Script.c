/*--- Hangartor ---*/

#strict 2

local direction;

public func IsMachine()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("Stand");
  direction = 0;
}

/* Bewegung nach links oder rechts */

protected func ControlLeft()
{
  if(GetEffect("IntCooldownTime",this)) return;

  SetAction("Travel");
  SetComDir(COMD_Left);
}

public func ControlRight()
{
  if(GetEffect("IntCooldownTime",this)) return;

  SetAction("Travel");
  SetComDir(COMD_Right);
}

public func ControlStop()
{
  Sound("HangarClose.ogg");
  SetAction("Stand");
  SetComDir(COMD_Stop);
  SetXDir();
  SetYDir();

  AddEffect("IntCooldownTime", this, 1, 15, this);

  //Unstuck von Lebewesen
  var xArea, yArea, xOffset, yOffset;
  xArea = GetDefCoreVal("Width", "DefCore")+30;
  yArea = GetDefCoreVal("Height", "DefCore")+30;
  xOffset = (xArea/2)+15;
  yOffset = (yArea/2)+15;
  var obj = FindObjects(Find_InRect(-xOffset,-yOffset,xArea,yArea), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Exclude(this));
  for(var target in obj)
  {
    AutoUnstuck(target);
  }
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(GetEffect("IntCooldownTime",this)) return;

  if(GetAction() == "Stand")
  {
    if(i == 1)
      if(direction == 0)
        return "$Right$";
      else
        return "$Left$";

    if(i == 2)
      if(direction == 1)
        return "$Right$";
      else
        return "$Left$";
  }
  else
  {
    if(i == 1)
      return "$Stop$";

    if(i == 2)
      if(direction == 1)
        return "$Right$";
      else
        return "$Left$";
  }
}

public func ConsoleControlled(int i)
{
  if(GetAction() == "Stand")
  {
    if(i == 1)
      if(direction == 0)
        ControlRight();
      else
        ControlLeft();

    if(i == 2)
      if(direction == 1)
      {
        ControlRight();
        direction = 0;
      }
      else
      {
        ControlLeft();
        direction = 1;
      }
  }
  else
  {
    if(i == 1)
      ControlStop();

    if(i == 2)
      if(direction == 1)
      {
        ControlRight();
        direction = 0;
      }
      else
      {
        ControlLeft();
        direction = 1;
      }
  }
}

/* Aufprallkontakt */

public func SetDirection(int i)	{direction = i;}

protected func ContactLeft()
{
  direction = 0;

  //Effekte
  CastSmoke("Smoke3",8, 5, -80, -5, 100, 200);

  return(ControlStop());
}

protected func ContactRight()
{
  direction = 1;

  //Effekte
  CastSmoke("Smoke3",8, 5, 79, -5, 100, 200);

  return(ControlStop());
}