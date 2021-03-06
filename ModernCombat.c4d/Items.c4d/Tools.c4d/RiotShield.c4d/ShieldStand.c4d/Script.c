/*-- Schildgestell --*/

#strict 2
#include CSTR

local aim_angle;
local pShield;
local fRotRight;

public func MaxDamage()		{return 100;}
public func LimitationCount()	{return 1;}
public func BonusPointRepair()	{return 30;}
public func IsDefusable()	{return true;}


/* Initialisierung */

func Initialize()
{
  //Animation und Effekt setzen
  SetAction("Con");
  AddEffect("IntCon", this(), 1, 1, this());
  destructability = true;

  return true;
}

/* Rotationsbegrenzungen */

public func MaxRotLeft()
{
  if(fRotRight)
    return 10;
  else
  return -150;
}

public func MaxRotRight()
{
  if(fRotRight)
    return 150;
  else
   return -10;
}

public func AimAngle()
{
  return aim_angle;
}

public func WeaponAt(&x, &y, &r)
{
  return true;
}

public func DoHit(int iHit)
{
  return true;
}

/* Schild */

public func GetShield()
{
  //Schild entfernen sofern vorhanden
  if(pShield)
    return(RemoveObject(pShield));

  //Ansonsten Schild erstellen
  pShield = CreateObject(RSLH, 0, 0, GetOwner(this));
  pShield->Set(this, this);
}

public func SetAim(int angle)
{
  aim_angle = angle;

  if(angle > 0)
    fRotRight = true;
  else
    fRotRight = false;
}

public func DirChanged()
{
  if(fRotRight != GetDir())
  aim_angle *= -1;

  fRotRight = GetDir();
}

/* Aufbau */

protected func FxIntConTimer(object target, int number, iEffectTime)
{
  if(iEffectTime == 1)
    SetDir(fRotRight);

  //Keine Arbeiter: wieder zusammenfallen
  if(!FindObject2(Find_ActionTarget(this),Find_Action("Push")) && GetActTime() > 5)
  {
    Collapse();
    return(-1);
  }

  //Aufbauger?usche
  if(!(iEffectTime%30))
  {
    Sound("WPN2_Hit*.ogg",0,0,50);
  }
}

private func ConComplete()
{
  if(GetEffect("IntCon", this))
    RemoveEffect("IntCon", this);

  var pTemp;
  while(pTemp = FindObject2(Find_ActionTarget(this), Find_Action("Push")))
    ObjectSetAction(pTemp, "Walk");

  //Schildgestell konfigurieren
  SetAction("Standing");
  GetShield();

  CheckLimitation();

  Sound("Connect");
  Sound("StructureConstructedStationary*.ogg");
}

private func Collapse()
{
  //Schild erstellen
  CreateObject(RSHL,0,8,GetOwner());

  //Verschwinden
  RemoveObject();
}

/* Abbau */

protected func FxIntDesTimer(object target, int number, iEffectTime)
{
  //Keine Arbeiter: Bestehen bleiben
  if(!FindObject2(Find_ActionTarget(this),Find_Action("Push")) && GetActTime() > 5)
  {
    if(GetEffect("IntDes", this))
      RemoveEffect("IntDes", this);

    //Schild konfigurieren
    SetAction("Standing");
    GetShield();
  }

  //Abbauger?usche
  if(!(iEffectTime%30))
  {
    Sound("WPN2_Hit*.ogg",0,0,50);
  }
}

private func DesComplete()
{
  if(GetEffect("IntDes", this))
    RemoveEffect("IntDes", this);

  var pTemp;
  while(pTemp = FindObject2(Find_ActionTarget(this), Find_Action("Push")))
    ObjectSetAction(pTemp, "Walk");

  //Schild erstellen
  CreateObject(RSHL,0,8,GetOwner());

  Sound("RSHL_Hide.ogg");

  return RemoveObject(this);
}

/* Steuerung */

public func ControlDigDouble(pClonk)
{
  //Schild entfernen
  GetShield();

  //Animation und Effekt setzen
  SetAction("Des");
  AddEffect("IntDes", this(), 1, 1, this());

  Sound("RSHL_Hide.ogg");
}

public func ControlLeft(pByObj)
{
  TurnLeft();

  return true;
}

public func ControlRight(pByObj)
{
  TurnRight();

  return true;
}

public func ControlUp(pByObj)
{
  if(fRotRight)
    TurnLeft();
  else
    TurnRight();
  return true;
}

public func ControlDownSingle(pByObj)
{
  if(!fRotRight)
    TurnLeft();
  else
    TurnRight();
  return true;
}

/* Ausrichtung */

private func TurnLeft()
{
  if(GetEffect("IntCon"))
    return;

  if(aim_angle > MaxRotLeft())
    aim_angle -= 10;
}

private func TurnRight()
{
  if(GetEffect("IntCon"))
    return;

  if(aim_angle < MaxRotRight())
    aim_angle += 10;
}

/* Schaden */

public func Destroyed(bool fNoShield)
{
  //Schild erstellen
  if(!fNoShield)
    CreateObject(RSHL,0,8,GetOwner());

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) 
  {
    CastParticles("MetalSplinter",3,110,0,0,40,100);
    CastParticles("MetalSplinter",1,80,0,0,90,90);
  }
  CastSmoke("Smoke3",8,10,0,5,100,150,RGBa(255,255,255,100),RGBa(255,255,255,130));
  Sound("MISL_ShotDown.ogg");

  //Verschwinden
  RemoveObject();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return 20;	//Explosionen und Druckwellen
  if(iType == DMG_Bio)		return 100;	//S?ure und biologische Schadstoffe

  return 80;
}

/* Entsch?rfung */

public func RTDefuse()
{
  DoDamage(1);
  if(GetDamage() >= 99)
  {
    Destroyed(true);
    return true;
  }

  return false;
}

/* HUD */

public func CustomHUD(int source)
{
  return source == HUD_Grabbed;
}

public func UpdateHUD(object hud, int source)
{
  //Daten ?bergeben
  hud->~Ammo(false, false, GetName(), false, true, false, source, true);
  if(GetAction() == "Con")
    hud->Recharge(GetActTime(), 76);
  else
    if(GetAction() == "Des")
      hud->Recharge(GetActTime(), 68);
}

/* Wasserkontakt */

protected func WaterContact()
{
  if(GetEffect("IntCon", this))
    Collapse();
  else
    Destroyed();
}

/* Aufschlag */ 

protected func Hit3()
{
  DoDamage(20);
}
  
protected func Hit()
{
  Sound("WPN2_Hit*.ogg",0,0,50);
  return 1;
}