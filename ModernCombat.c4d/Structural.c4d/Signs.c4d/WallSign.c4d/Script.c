/*-- Wandschild --*/

#strict 2


/* Intialisierung */

func Initialize()
{
  Set("Explosives","Grey");
}

/* Einstellungen */

func Set(string szIcon, string szMaterial)
{
  //Icon
  if(szIcon)
    SetGraphics(0,0,GetID(this),2,GFXOV_MODE_Action, szIcon);

  //Material
  if(szMaterial)
    SetAction(szMaterial);
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 80)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,200,200);
    if(GetAction() == "Wood")
    {
      CastParticles("WoodSplinter",4,50,0,0,100,100);
      Sound("WoodHit*.wav");
    }
    else
    {
      CastParticles("StructureSplinter",1,50,0,0,100,100);
      Sound("MetalHit*.wav");
    }

    RemoveObject();
  }
}