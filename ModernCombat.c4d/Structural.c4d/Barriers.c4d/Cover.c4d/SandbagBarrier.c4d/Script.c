/*-- Sandsackbarriere --*/

#strict 2
#include CSTR

local left, ruins;

public func MaxDamage()			{return 30;}
public func BonusPointCondition()	{return fDestroyed;}


/* Initialisierung */

func Initialize()
{
  //Ruinen standardmäßig ausgeschaltet
  ruins = false;

  //Eventuell feststeckende Objekte freimachen
  ClearObjects();

  return inherited();
}

/* Konfiguration */

public func Set(bool fRight, bool fRuins)
{
  if(fRight)
    Right();
  else
    Left();

  if(fRuins)
    ruins = true;
}

public func SwitchMode()
{
  ruins = !ruins;
}

/* Schaden */

public func OnDestruction()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,0,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("Sandbag", 15, 70, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  Sound("FenceDestruct.ogg");

  if(!ruins)
    RemoveObject();
  else
  {
    SetGraphics("Destroyed");
    fDestroyed = true;
  }
}

public func OnDmg(int iDamage, int iType)
{
  if(iType == DMG_Explosion) return 50;
  return 100;
}

/* Konstruktion */

func Construction(object pByObj)
{
  var dir = GetDir(pByObj);

  if(!dir)
    dir = GetDir(Contained(pByObj));

  if(dir)
    Right();
  else
    Left();
}

func Right()
{
  SetGraphics();
  left = 0;
  return this;
}

func Left()
{
  SetGraphics("LEFT");
  left = 1;
  return this;
}

/* Reparatur */

public func StartRepair()	{return true;}
public func IsRepairable()	{return fDestroyed;}
public func RepairSpeed()	{return 1;}
public func OnRepairSound()	{return Sound("StructureConstructedSandbags*.ogg");}

public func OnRepair()
{
  _inherited();

  //Aussehen und SolidMask wiederherstellen
  if(!left)
    Right();
  else
    Left();

  //Bei Bedarf Unstuck von Lebewesen
  ClearObjects();
}