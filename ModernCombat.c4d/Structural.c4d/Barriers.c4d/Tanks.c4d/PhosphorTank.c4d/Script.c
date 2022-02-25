/*-- Phosphortank --*/

#strict 2
#include XTNK

public func ExplosionTime()	{return 20+Random(100);}


/* Zerstörung */

func BlowUp(iPlr)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",15+Random(5),100,0,0,35,50,RGB(40,20,20));
  CastSmoke("Smoke3",10,30,0,0,350,400,RGBa(100,150,250,130));
  CastParticles("MetalCrateSplinter",15+Random(5),100,0,0,60,100);
  Sound("PhosphorExplosion.ogg");
  Sound("StructureHeavyHit*.ogg");

  //Phosphor verschleudern
  for(var i = 0; i <= 20; i++)
    CreateObject(PSPR, AbsX(GetX()), AbsY(GetY())-GetDefHeight(GetID())/2, GetController())->SetSpeed(RandomX(-50,50), RandomX(-50,50));

  //Explosion
  FakeExplode(20, iPlr+1);
  RemoveObject();
}