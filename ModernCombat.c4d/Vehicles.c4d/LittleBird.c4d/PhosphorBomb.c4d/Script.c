/*-- Phosphorbombe --*/

#strict 2
#include ESHL

public func Color()		{return RGB(130,130,255);}	//Kennfarbe
public func ExplodeDelay()	{return 120;}			//Verzögerung bis zu automatischer Zündung
public func MaxDamage()		{return 40;}			//Maximalschaden bis Zerstörung


/* Initialisierung */

public func Initialize()
{
  _inherited();

  //Fluggeräusch
  EchoLoop("ArtilleryEcho*.ogg",1);
}

/* Treffer */

func Hit3()
{
  //Effekte
  Sparks(30,RGB(255,128));
}

func Hit()
{
  //Effekte
  EchoLoop("ArtilleryEcho*.ogg",-1);
  Sound("PhosphorShell_Hit*.ogg");

  SetSpeed();
  SetRDir();
}

func Trigger(object pObj)
{
  if(pObj)
  {
    //Explosion
    Explode(BlastRadius()*2/3);
    DamageObjects(BlastRadius()*3/2,BlastRadius()/2,this, 0, 0, iAttachment);
  }
  else
  {
    //Explosion
    Explode(BlastRadius()*2/4);
    DamageObjects(BlastRadius()*3/3,BlastRadius()/3,this, 0, 0, iAttachment);

    //Phosphor verschleudern
    for(var i = 0; i < 3; i++)
    {
      var pPhosphor = CreateObject(PSPR, 0, 0, GetOwner());
      SetController(GetController(), pPhosphor);
      SetXDir(RandomX(-30,30), pPhosphor);
      SetYDir(RandomX(-30,30), pPhosphor);
    }
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("MetalSplinter",10,150,0,0,25,50);
    CastSmoke("Smoke3",20,20,0,0,100,200,RGBa(100,150,250,100));
  }
  CreateParticle("ShockWave",0,0,0,0,12*BlastRadius(),RGB(0,255,255));
  Sound("PhosphorShell_Hit*.ogg");

  //Verschwinden
  RemoveObject();
}