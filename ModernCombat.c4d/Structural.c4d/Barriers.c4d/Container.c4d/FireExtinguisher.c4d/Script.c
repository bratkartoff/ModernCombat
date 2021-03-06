/*-- Feuerlöscher --*/

#strict 2
#include GSBL

local damaged;

public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return false;
  return !Random(6);
}

public func IsMeleeTarget(object obj)
{
  if(damaged) return;
  if(obj) if(GetID(obj) == BWTH) return 1;
  return;
}


/* Schaden */

func Damage(int iChange)
{
  if(damaged || !this || GetDamage() < 50) return;
  InstaExplode();
}

/* Zerstörung */

func InstaExplode()
{
  if(damaged) return;
  damaged = true;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CastParticles("MetalSplinter", 3+Random(3), 80, 0,0, 30,80,RGB(250,0,0));
    CastSmoke("Smoke3",4,30,0,0,100,300,RGBa(255,255,255,100),RGBa(255,255,255,130));
  }
  AddEffect("GSBL_Smoke",this,251,1,this);
  Sound("BarrelImpact*.ogg");
  Sound("Fuse.wav");

  //Kategorie wechseln
  SetCategory(C4D_Vehicle);

  //Nach oben schleudern
  SetRDir(RandomX(-50,50));
  SetXDir(Sin(GetR(),60));
  SetYDir(-Cos(GetR(),60));

  //Explosion
  ScheduleCall(this, "BlowUp", 25);
}

func BlowUp()
{
  //Rauch erzeugen
  CastObjects(SM4K,4,20);

  //Sound
  Sound("SGRN_Fused.ogg");

  //Verschwinden lassen
  DecoExplode(10);
}