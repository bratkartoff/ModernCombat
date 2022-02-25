/*-- Zerstörte Stahlbrücke --*/

#strict 2
#include CSTR

public func MaxDamage()		{return 150;}

local ruins;


/* Initialisierung */

public func Initialize()
{
  //Eigenständig erstellte zerstörte Stahlbrücken sind standardmäßig Ruinen
  ruins = true;
  fDestroyed = true;
  iLastAttacker = -1;
  destructability = true;

  SetGraphics("Destroyed");
  SetSolidMask(0,0,72,24);
  DoDamage(MaxDamage());
}

/* Einstellung */

public func Set()
{
  //Wrack oder Ruine
  if(ruins)
  {
    SetGraphics("Destroyed");
    SetSolidMask(0,0,72,24);
  }
  else
  {
    SetSolidMask();
    FadeOut();
  }
}

/* Aufschlag */

protected func Hit3()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",25,15,0,0,50,200, RGBa(255,255,255,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,40,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
  Sound("StructureHit*.ogg");
}

/* Reparatur */

public func StartRepair()	{return true;}
public func IsRepairable()	{return ruins;}

public func OnRepair()
{
  _inherited();

  //Aussehen und SolidMask wiederherstellen
  ChangeDef(_HBR);
  SetGraphics();
  SetSolidMask(0,0,72,24);

  //Bei Bedarf Unstuck von Lebewesen
  ClearObjects();
}