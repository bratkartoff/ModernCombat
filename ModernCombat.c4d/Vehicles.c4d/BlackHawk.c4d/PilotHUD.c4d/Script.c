/*-- Piloten-HUD --*/

#strict 2

static const BHUD_Off = -1;
static const BHUD_Ready = 0;
static const BHUD_Warning = 1;
static const BHUD_Error = 2;
static const BHUD_Disabled = 3;

static const BHUD_Overlay_Flares = 1;
static const BHUD_Overlay_SmokeWall = 2;
static const BHUD_Overlay_Warning = 3;
static const BHUD_Overlay_Failure = 4;
static const BHUD_Overlay_Extinguisher = 5;
static const BHUD_Overlay_ExtinguisherInfo = 6;

local pHelicopter;
local iState;
local pRotation, pThrottle, pAltitude, pWind;
local fDamage, iDamageRemaining;
local fFlares, fSmokeWall, fExtinguisher;

local iR, iG, iB, iT;	//Standardfarbe der Hauptgrafik


/* Initialisierung */

public func Initialize()
{
  SetVisibility(VIS_None);
  iState = -1;
  fFlares = true;
  fSmokeWall = true;
  fExtinguisher = true;
  Schedule("SetVisibility(VIS_Owner)", 1, 0, this);
  SetGraphics("Flares", this, GetID(), BHUD_Overlay_Flares, GFXOV_MODE_Base);
  SetGraphics("SmokeWall", this, GetID(), BHUD_Overlay_SmokeWall, GFXOV_MODE_Base);
  SetGraphics("Extinguisher", this, GetID(), BHUD_Overlay_Extinguisher, GFXOV_MODE_Base);
  SetGraphics("ExtinguisherInfo", this, GetID(), BHUD_Overlay_ExtinguisherInfo, GFXOV_MODE_Base);
  SetGraphics("Warning", this, GetID(), BHUD_Overlay_Warning, GFXOV_MODE_Base);
  SetGraphics("Failure", this, GetID(), BHUD_Overlay_Failure, GFXOV_MODE_Base);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_SmokeWall);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Extinguisher);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_ExtinguisherInfo);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
  SetStandardColor(0,210,255,50);
  SetState(BHUD_Ready);
  return true;
}

public func SetStandardColor(int iRn, int iGn, int iBn, int iTn)
{
  iR = iRn;
  iG = iGn;
  iB = iBn;
  iT = iTn;

  SetClrModulation(RGBa(iR,iG,iB,iT));
}

protected func SetState(int iNewState, bool fKeepSound, bool fUpdate)
{
  if(iState == iNewState && !fUpdate) return;
  iState = iNewState;
  var dwArrowColor;
  if(iState == BHUD_Off)
  {
    SetClrModulation(RGBa(0,0,0,255));
    dwArrowColor = RGBa(0,0,0,255);
  }
  if(iState == BHUD_Error)
  {
    SetClrModulation(RGBa(255,0,0,50));
    SetClrModulation(RGBa(255,0,0,50), this, BHUD_Overlay_Failure);
    dwArrowColor = RGBa(255,0,0,50);

    if(pHelicopter->GetDamage() >= pHelicopter->MaxDamage()*3/4)
    {
      Sound("WarningDamageCritical.ogg", false, this, 100, GetOwner()+1, +1);
      if(fExtinguisher)
        SetClrModulation(dwArrowColor, this, BHUD_Overlay_ExtinguisherInfo);
      else
        SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_ExtinguisherInfo);
    }
    else
      Sound("WarningDamage.ogg", false, this, 100, GetOwner()+1, +1);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_ExtinguisherInfo);
    if(!fKeepSound)
    {
      Sound("WarningDamageCritical.ogg", false, this, 100, GetOwner()+1, -1);
      Sound("WarningDamage.ogg", false, this, 100, GetOwner()+1, -1);
    }
  }
  if(iState == BHUD_Warning)
  {
    SetClrModulation(RGBa(255,153,0,50));
    SetClrModulation(RGBa(255,153,0,50), this, BHUD_Overlay_Warning);
    dwArrowColor = RGBa(255,153,0,50);
    Sound("WarningLockOn.ogg", false, this, 100, GetOwner()+1, +1);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
    if(!fKeepSound)
    {
      Sound("WarningLockOn.ogg", false, this, 100, GetOwner()+1, -1);
    }
  }
  if(iState == BHUD_Ready)
  {
    SetClrModulation(RGBa(iR,iG,iB,iT));
    dwArrowColor = RGBa(255,204,0,50);
  }
  if(iState == BHUD_Disabled)
  {
    SetClrModulation(RGBa(122,122,122,50));
    dwArrowColor = RGBa(122,122,122,50);
  }
  if(fFlares)
  {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_Flares);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  }
  if(fSmokeWall)
  {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_SmokeWall);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_SmokeWall);
  }
  if(fExtinguisher)
  {
    SetClrModulation(dwArrowColor, this, BHUD_Overlay_Extinguisher);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Extinguisher);
  }
  if(pRotation) pRotation->SetClrModulation(dwArrowColor);
  if(pThrottle) pThrottle->SetClrModulation(dwArrowColor);
  if(pAltitude)
    if(GetY(pHelicopter) < 80)
    {
      if(GetAction(pAltitude) != "Blink")
        pAltitude->~SetAction("Blink");
      pAltitude->~SetClrModulation(RGBa(255,0,0,50));
    }
    else
    {
      pAltitude->~SetAction("Idle");
      pAltitude->~SetClrModulation(dwArrowColor);
    }
  if(pWind) pWind->SetClrModulation(dwArrowColor);
  return true;
}

protected func GetState()
{
  return iState;
}

public func SetHelicopter(object pNewHelicopter)
{
  pHelicopter = pNewHelicopter;
  Align();
  return true;
}

public func DamageReceived()
{
  fDamage = true;
  iDamageRemaining = 10;
  return true;
}

protected func Align()
{
  SetPosition(BoundBy(GetX(pHelicopter), GetDefWidth(GetID())/2+GetDefWidth(BARW)+10, LandscapeWidth()-GetDefWidth(GetID())/2-GetDefWidth(BARW)-10),
    BoundBy(GetY(pHelicopter), GetDefHeight(GetID())/2+GetDefHeight(BARW)+10, LandscapeHeight()-GetDefHeight(GetID())/2-GetDefHeight(BARW)-10));
  return true;
}

protected func Timer()
{
  //Kein Helikopter oder passender Pilot vorhanden: Verschwinden
  if(!pHelicopter || !pHelicopter->GetPilot() || GetOwner(this) != GetOwner(pHelicopter->GetPilot()) || GetOwner() == NO_OWNER)
  {
    RemoveObject();
    return true;
  }
  if(iState != BHUD_Off)
  {
    //An Helikopter ausrichten
    Align();
    //Rotation anzeigen
    if(!pRotation)
    {
      pRotation = CreateObject(BRTN,0,0,GetOwner());
      pRotation->SetOwner(GetOwner());
      pRotation->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()+1, GetY()+63, pRotation);
    pRotation->SetVisibility(GetVisibility());
    pRotation->SetR(GetR(pHelicopter));

    //Throttle anzeigen
    if(!pThrottle)
    {
      pThrottle = CreateObject(BARW,0,0,GetOwner());
      pThrottle->SetR(180);
      pThrottle->SetOwner(GetOwner());
      pThrottle->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()-145, GetY()+70-BoundBy((((140*1000)/BKHK_MaxThrottle)*pHelicopter->GetThrottle())/1000, 0, 140), pThrottle);
    pThrottle->SetVisibility(GetVisibility());

    //H?he anzeigen
    if(!pAltitude)
    {
      pAltitude = CreateObject(BARW,0,0,GetOwner());
      pAltitude->SetOwner(GetOwner());
      pAltitude->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()+144, GetY()+71-BoundBy(140-((((140*1000)/LandscapeHeight())*GetY(pHelicopter))/1000), 0, 140), pAltitude);
    pAltitude->SetVisibility(GetVisibility());
    if(GetY(pHelicopter) < 80)
    {
      if(GetAction(pAltitude) != "Blink")
        pAltitude->~SetAction("Blink");
      pAltitude->~SetClrModulation(RGBa(255,0,0,50));
    }
    else
    {
      pAltitude->~SetAction("Idle");
      pAltitude->SetClrModulation(RGBa(255,204,0,50));
    }

    //Wind anzeigen
    if(!pWind)
    {
      pWind = CreateObject(BARW,0,0,GetOwner());
      pWind->SetR(-90);
      pWind->SetOwner(GetOwner());
      pWind->SetVisibility(VIS_Owner);
      pWind->SetClrModulation(RGBa(255,204,0,50));
    }
    SetPosition(GetX()+BoundBy((1400*GetWind(AbsX(GetX(pHelicopter)), AbsY(GetY(pHelicopter))))/1000, -70, 70), GetY()-63, pWind);
    pWind->SetVisibility(GetVisibility());

    //Standardform setzen
    SetObjDrawTransform(1000,0,0,0,1000,0);
  }

  //Flares, Rauchwand und Feuerl?scher
  var fUpdate = false;
  var tFlares = pHelicopter->CanDeployFlares();
  var tSmokeWall = pHelicopter->CanDeploySmokeWall();
  var tExtinguisher = pHelicopter->CanDeployExtinguisher();
  if(fFlares != tFlares)
  {
    fUpdate = true;
    if(!fFlares) Sound("WarningFlaresReloaded.ogg", false, this, 100, GetOwner()+1);
    fFlares = tFlares;
  }
  if(fSmokeWall != tSmokeWall)
  {
    fUpdate = true;
    if(!fSmokeWall) Sound("WarningSmokeWallReloaded.ogg", false, this, 100, GetOwner()+1);
    fSmokeWall = tSmokeWall;
  }
  if(fExtinguisher != tExtinguisher)
  {
    fUpdate = true;
    if(!fExtinguisher) Sound("WarningExtinguisherReloaded.ogg", false, this, 100, GetOwner()+1);
    fExtinguisher = tExtinguisher;
  }
  if(fUpdate)
  {
    SetState(GetState(), false, true);
  }
  //Schadensverhalten
  if(fDamage || pHelicopter->GetDamage() >= pHelicopter->MaxDamage()*3/4)
  {
    var fDisable = false;
    if(iDamageRemaining == 0 && Random(5))
    {
      SetState(BHUD_Error);
    }
    else
    {
      if(iDamageRemaining == 0) fDisable = true;
      if(!Random(5) && fDisable)
      {
        SetState(BHUD_Off, true);
      }
      else if(!Random(5))
      {
        SetState(BHUD_Error);
        if(!Random(2))
        {
          var val = RandomX(0,300);
          if(!Random(2)) val *= -1;
          SetObjDrawTransform(RandomX(800, 1200),val,RandomX(-5,5)*1000,val,RandomX(800, 1200),RandomX(-5,5)*1000);
        }
      }
    }
  }
  else
  {
    if(pHelicopter->GetRocket())
      SetState(BHUD_Warning);
    else
      if(pHelicopter->GetAutopilot())
        SetState(BHUD_Disabled);
      else
        SetState(BHUD_Ready);
  }
  if(iDamageRemaining > 0) iDamageRemaining--;
  if(!iDamageRemaining) fDamage = false;
  return true;
}

/* Entfernung */

public func Destruction()
{
  SetState(BHUD_Ready);
  if(pRotation) RemoveObject(pRotation);
  if(pThrottle) RemoveObject(pThrottle);
  if(pAltitude) RemoveObject(pAltitude);
  if(pWind) RemoveObject(pWind);
  return true;
}