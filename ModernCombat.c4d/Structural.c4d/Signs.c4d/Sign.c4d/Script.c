/*-- Schild --*/

#strict 2

local mode;


/* Intialisierung */

func Initialize()
{
  SetAction("Exist");
  Set("Danger");
}

/* Blinken */

func Blink()
{
  AddLightFlash(100,0,0,RGB(255,0,0));
}

/* Einstellungen */

func Set(string szIcon, bool fMode, bool fFloat, bool fFadeIn)
{
  //Icon und Modus
  if(fMode)
  {
    SetGraphics("Metal");
    SetGraphics("Metal",0,GetID(this),2,GFXOV_MODE_Action, szIcon);
    SetAction("Exist2");
    mode = 1;
  }
  else
  {
    SetGraphics("");
    SetGraphics(0,0,GetID(this),2,GFXOV_MODE_Action, szIcon);
    SetAction("Exist");
    mode = 0;
  }

  //Hintergrundobjekt
  if(fFloat)
    SetCategory(C4D_StaticBack);
  else
    SetCategory(C4D_Structure);

  //Einblenden
  if(fFadeIn)
    FadeIn();
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 80)
  {
    //Effekte
    if(mode)
      CastParticles("MetalSplinter", 8, 50, 0,0, 141);
    else
      CastParticles("WoodSplinter", 8, 50, 0,0, 141);
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("CrateDestruct*.ogg");

    RemoveObject();
  }
}