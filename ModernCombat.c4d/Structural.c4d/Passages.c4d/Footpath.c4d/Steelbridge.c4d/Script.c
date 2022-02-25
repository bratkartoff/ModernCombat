/*-- Stahlbrücke --*/

#strict 2
#include CSTR

public func MaxDamage()			{return 150;}
public func BonusPointCondition()	{return false;}

local ruins, norepair;


/* Initialisierung */

public func Initialize()
{
  //Ruinen standardmäßig ausgeschaltet
  ruins = false;

  _inherited();
}

/* Konfiguration */

public func SwitchMode()
{
  ruins = !ruins;
}

public func SetIrreparable()
{
  norepair = true;
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  return;
}

public func OnDestruction()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("ConcreteSplinter",4,150,0,0,40,100);
    CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
  }
  CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  Sound("StructureHit*.ogg");

  //Aktion und Grafik setzen
  ChangeDef(_HBB);
  this->~Set();
}

/* Reparatur */

public func IsRepairable()	{return !norepair;}