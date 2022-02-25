/*-- Fahrzeugfadenkreuz --*/

#strict
#include HCRH

local iIndicatorTime;


/* Initialisierung */

func Initialize()
{
  my_r = 0;
  SetVertex(0,1,CH_Distance,0,2);

  //Kontrast
  SetGraphics(0,0,GetID(),1,GFXOV_MODE_Action,"Base1");

  //Färbung
  SetGraphics(0,0,GetID(),2,GFXOV_MODE_Action,"Overlay1");

  //Kontrast Indikator
  SetGraphics(0,0,GetID(),3,GFXOV_MODE_Action,"Base2");
  SetClrModulation(RGB(255,255,255),0,3);

  //Färbung Indikator
  SetGraphics(0,0,GetID(),4,GFXOV_MODE_Action,"Overlay2",GFX_BLIT_Additive);
  SetClrModulation(RGB(255,255,255),0,4);

  return 1;
}

protected func Check()
{
  //Nutzer ermitteln
  var user = GetActionTarget();
  if(!user)
    return RemoveObject();
  if(!user->~IsAiming())
    return RemoveObject();	

  //Waffe ermitteln
  var wpn = Contents(0,user);
  if(!wpn) return;
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;

  //Indikatoranzeige
  if(iIndicatorTime)
    iIndicatorTime = BoundBy(iIndicatorTime-50, 0, 255);

  //Färbung je nach Waffenstatus
  var rgb = RGB(0,255,0);
  if(wpn->IsReloading() || !wpn->GetCharge())
    rgb = RGB(255,0,0);

  SetClrModulation(rgb,0,2);

  SetClrModulation(RGBa(255,255,255,255-iIndicatorTime),0,4);	//Färbung Indikator
  SetClrModulation(RGBa(0,0,0,255-iIndicatorTime),0,3);		//Transparenz Indikator links
}

public func SetIndicator(int iTime)
{
  iIndicatorTime = iTime;
}

protected func Timer()
{
  var angle = GetR();
  //var angle2 = GetActTime()*Global(1);
  
  //var siz = Sin(angle2,200);
  var w = 1000;
  var l = 1000;
  var fsin = -Sin(angle, 1000), fcos = Cos(angle, 1000);
  var width = +fcos*w/1000, height = +fcos*l/1000;
  var xskew = +fsin*l/1000, yskew = -fsin*w/1000;
  
  // set matrix values
  SetObjDrawTransform (
    width, xskew, 0,
    yskew, height, 0,
	0, 1
  );
  SetObjDrawTransform (
    width, xskew, 0,
    yskew, height, 0,
	0, 2
  );
  SetObjDrawTransform (
    width, xskew, 0,
    yskew, height, 0,
	0, 3
  );
  SetObjDrawTransform (
    width, xskew, 0,
    yskew, height, 0,
	0, 4
  );
}