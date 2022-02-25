/*-- Wrack --*/

#strict 2


/* Aufschlag */

protected func Hit()
{
  //Effekte
  Sound("VehicleHit*.ogg");
}

protected func Hit3()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1 && !GBackLiquid())
    CastParticles("GunSmoke", 10, 35, 0, 0, 100, 180);
  CastParticles("MetalSplinter", 8, 200, 0, 0, 100, 50, RGB(40, 20, 20));
  CastParticles("StructureSplinter",2,70,RandomX(-10,10),-10,50,100);
  Sound("VehicleHeavyHit*.ogg");

  RemoveObject();
  return;
}