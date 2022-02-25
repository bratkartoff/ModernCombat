/*-- Benzintank --*/

#strict 2

public func MaxDamage()		{return 100;}
public func ExplosionTime()	{return 20+Random(80);}

/* Entzündung */

func Incineration()
{
  ScheduleCall(this,"BlowUp",ExplosionTime(),0,GetController());
}

func IncinerationEx()
{
  ClearScheduleCall(this, "BlowUp");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() >= MaxDamage())
    Incinerate();
}

func BlowUp(iPlr)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",15+Random(5),100,0,0,35,50,RGB(40,20,20));
  CastParticles("MetalCrateSplinter",15+Random(5),100,0,0,60,100,RGB(250,0,0));
  Sound("StructureHeavyHit*.ogg");

  //Explosion
  FakeExplode(50, iPlr+1);
  RemoveObject();
}