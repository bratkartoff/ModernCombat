/*-- Wrack --*/

#strict 2


/* Helikoptereffekte */

protected func GetWrecked(int iAdditional)
{
  SetAction("Destroyed");
  ScheduleCall(this,"Smoking",5,40);
  SetController(GetOwner());
  Incinerate();
  if(iAdditional)
    Sound("WreckEngineDown*.ogg", false, this, RandomX(25,75));
}

/* Aufschlag */

protected func Hit3()
{
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    if(!GBackLiquid())
      CastParticles("GunSmoke", 15, 35, -20, 0, 300, 500);
    CastParticles("MetalSplinter", 8, 200, 0, 0, 100, 50, RGB(40, 20, 20));
  }
  Sound("VehicleHeavyHit*.ogg");
}

protected func Hit()
{
  Sound("WreckHeavyHit*.ogg");
}

/* Zerstörung */

func DestroyWreck()
{
  //Effekte
  if(!GBackLiquid())
  {
    CastParticles("GunSmoke", 15, 35, -20, 0, 300, 500);
    CastParticles("GunSmoke", 15, 35, 20, 0, 300, 500);
  }
  else
    CastParticles("BlastDirt",8,50,0,0,1000,1200);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 8, 200, 0, 0, 100,50, RGB(40, 20, 20));
  Sound("C4EX_Detonation*.ogg");
  Sound("StructureHit*.ogg");

  //Explosion
  Explode(40);
}

/* Effekte */

func Smoking()
{
  if(OnFire())
    Smoke(0, 0, 30);
  else
    ClearScheduleCall(this,"Smoking");
}