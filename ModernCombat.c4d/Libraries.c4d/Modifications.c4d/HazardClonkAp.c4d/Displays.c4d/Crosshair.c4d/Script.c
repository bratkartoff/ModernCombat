/*-- Fadenkreuz --*/

#strict 2

local target;
local spread, iLastSpread;
local angle, iIndicatorTime;

static const CH_Distance = 60;
static const CH_Spread_Prec = 10;

public func NoWarp()	{return true;}


/* Initialisierung */

func Initialize()
{
  angle = 0;
  InitGraphics();

  return 1;
}

public func InitGraphics()
{
  //Linker Strich
  SetGraphics(0,0,GetID(),1,GFXOV_MODE_Action,"Base1");
  SetClrModulation(RGB(255,255,255),0,1);
  SetGraphics(0,0,GetID(),2,GFXOV_MODE_Action,"Status1",GFX_BLIT_Additive);

  //Rechter Strich
  SetGraphics(0,0,GetID(),3,GFXOV_MODE_Action,"Base2");
  SetClrModulation(RGB(255,255,255),0,3);
  SetGraphics(0,0,GetID(),4,GFXOV_MODE_Action,"Status2",GFX_BLIT_Additive);

  //Punkt
  SetGraphics(0,0,GetID(),5,GFXOV_MODE_Action,"Base3");
  SetClrModulation(RGB(255,255,255),0,5);
  SetGraphics(0,0,GetID(),6,GFXOV_MODE_Action,"Status3",GFX_BLIT_Additive);

  //Linker Indikator
  SetGraphics(0,0,GetID(),7,GFXOV_MODE_Action,"Base4");
  SetClrModulation(RGB(255,255,255),0,7);
  SetGraphics(0,0,GetID(),8,GFXOV_MODE_Action,"Status4",GFX_BLIT_Additive);

  //Rechter Indikator
  SetGraphics(0,0,GetID(),9,GFXOV_MODE_Action,"Base5");
  SetClrModulation(RGB(255,255,255),0,9);
  SetGraphics(0,0,GetID(),10,GFXOV_MODE_Action,"Status5",GFX_BLIT_Additive);

  //Linker überblendender Schatten
  SetGraphics(0,0,GetID(),11,GFXOV_MODE_Action,"Base6");
  SetClrModulation(RGB(255,255,255),0,11);

  //Rechter überblendender Schatten
  SetGraphics(0,0,GetID(),12,GFXOV_MODE_Action,"Base7");
  SetClrModulation(RGB(255,255,255),0,12);

  UpdateGraphics();
}

protected func GetUser()
{
  return target;
}

/* Timer */

protected func Check()
{
  //Clonk ermitteln
  if(!target)
    return RemoveObject();
  if(!GetAlive(target))
    return RemoveObject();

  //Keine Waffe oder Handgranate: Ausblenden
  var wpn = Contents(0, target);
  if(!wpn)
  {
    target->HideCH();
    return;
  }
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;

  //Clonk mit unpassender Aktion: Ausblenden
  if(GetProcedure(target) == "PUSH" && GetActionTarget(0, target) && GetActionTarget(0, target)->~CanAim())
    target->HideCH();

  //Indikatoranzeige
  if(iIndicatorTime)
    iIndicatorTime = BoundBy(iIndicatorTime-50, 0, 255);

  //Färbung je nach Waffenstatus
  var ClrR, ClrG, ClrB;
  var ClrT = BoundBy(320-spread,0,255);
  if(wpn->IsReloading() || !wpn->GetCharge())
  {
    ClrR = 255;
    ClrG = 0;
    ClrB = 0;
  }
  else
  {
    ClrR = 0;
    ClrG = 255;
    ClrB = 0;
  }

  //Striche-Schatten
  SetClrModulation(RGB(ClrR,ClrG,ClrB),0,2);
  SetClrModulation(RGB(ClrR,ClrG,ClrB),0,4);

  //Punktfärbung und -schatten
  SetClrModulation(RGBa(ClrR,ClrG,ClrB,ClrT),0,6);
  SetClrModulation(RGBa(ClrR,ClrG,ClrB,ClrT),0,5);

  //Indikatorfärbung und -schatten
  SetClrModulation(RGBa(0,0,0,255-iIndicatorTime),0,7);
  SetClrModulation(RGBa(255,255,255,255-iIndicatorTime),0,8);
  SetClrModulation(RGBa(0,0,0,255-iIndicatorTime),0,9);
  SetClrModulation(RGBa(255,255,255,255-iIndicatorTime),0,10);

  //Überblendender Schatten
  SetClrModulation(RGBa(0,0,0,BoundBy(255-spread,0,255)),0,11);
  SetClrModulation(RGBa(0,0,0,BoundBy(255-spread,0,255)),0,12);

  //Spread bei Bedarf aktualisieren
  if(spread != iLastSpread)
  {
    UpdateGraphics();
    iLastSpread = spread;
  }
}

/* Konfiguration */

public func Init(object pTarget)
{
  Set(pTarget);
}

public func Set(object pTarget)
{
  SetOwner(GetOwner(pTarget));
  SetAction("Attach",pTarget);
  target = pTarget;
  SetVisibility(VIS_Owner);
  Check();
}

public func Reset(object pTarget)
{
  SetAction("Attach",pTarget);
  SetVisibility(VIS_Owner);
  Check();
}

public func UnSet()
{
  SetAction("Idle");
  SetActionTargets();
  SetVisibility(VIS_None);
  SetPosition();
}

public func SetAngle(int iAngle)
{
  angle = Normalize(iAngle);
  UpdateAngle();
}

public func GetAngle()
{
  return GetR();
}

public func SetIndicator(int iTime)
{
  iIndicatorTime = iTime;
  UpdateAngle();
}

public func ChangeDir()
{
  angle = Normalize(360-angle);
  UpdateAngle();
}

public func DoAngle(int iChange)
{
  angle = Normalize(angle+iChange);
  UpdateAngle();
}

public func UpdateAngle()
{
  if(target)
    if(Contents(0,target))
      if(Contents(0,target)->~IsGrenade())
        if(!target->~IsAiming())
          angle = 65*(GetDir(target)*2-1);
  SetR(angle/*+GetR(target)*/);
  UpdateGraphics();
}

public func SetSpread(int iSpread)
{
  spread = iSpread;
  if(spread != iLastSpread)
  {
    UpdateGraphics();
    iLastSpread = spread;
  }
}

public func GetSpread()
{
  return spread;
}

/* Fadenkreuz zeichnen */

public func UpdateGraphics()
{
  var iLeft = -GetR()*CH_Spread_Prec-(spread/2);
  var iRight = -GetR()*CH_Spread_Prec+(spread/2);

  Draw(iLeft,CH_Distance,CH_Spread_Prec,1);
  Draw(iLeft,CH_Distance,CH_Spread_Prec,2);

  Draw(iRight,CH_Distance,CH_Spread_Prec,3);
  Draw(iRight,CH_Distance,CH_Spread_Prec,4);

  Draw(-GetR()*CH_Spread_Prec,CH_Distance,CH_Spread_Prec,5);
  Draw(-GetR()*CH_Spread_Prec,CH_Distance,CH_Spread_Prec,6);

  Draw(iLeft,CH_Distance,CH_Spread_Prec,11);
  Draw(iRight,CH_Distance,CH_Spread_Prec,12);

  if(iIndicatorTime)
  {
    Draw(iLeft-120,CH_Distance,CH_Spread_Prec,7);
    Draw(iLeft-120,CH_Distance,CH_Spread_Prec,8);

    Draw(iRight+120,CH_Distance,CH_Spread_Prec,9);
    Draw(iRight+120,CH_Distance,CH_Spread_Prec,10);
  }
}

private func Draw(int r, int off, int prec, int overlay)
{
  var xoff = -Sin(r,off,prec);
  var yoff = -Cos(r,off,prec);

  var fsin = Sin(r, 1000,prec), fcos=Cos(r, 1000,prec);

  SetObjDrawTransform
  (
    +fcos, +fsin, xoff*1000,
    -fsin, +fcos, yoff*1000,
    0, overlay
  );
}