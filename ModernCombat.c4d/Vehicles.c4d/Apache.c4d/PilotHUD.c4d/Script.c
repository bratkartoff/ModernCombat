/*-- Piloten-HUD --*/

#strict 2
#include BHUD

static const APCE_Overlay_RocketPod = 7;

local fRocketPod;


/* Initialisierung */

public func Initialize() 
{
  SetVisibility(VIS_None);
  iState = -1;
  fFlares = true;
  fRocketPod = true;
  fExtinguisher = true;
  Schedule("SetVisibility(VIS_Owner)", 1, 0, this);
  SetGraphics("Flares", this, BHUD, BHUD_Overlay_Flares, GFXOV_MODE_Base);
  SetGraphics("Rockets", this, AHUD, APCE_Overlay_RocketPod, GFXOV_MODE_Base);
  SetGraphics("Extinguisher", this, BHUD, BHUD_Overlay_Extinguisher, GFXOV_MODE_Base);
  SetGraphics("ExtinguisherInfo", this, BHUD, BHUD_Overlay_ExtinguisherInfo, GFXOV_MODE_Base);
  SetGraphics("Warning", this, BHUD, BHUD_Overlay_Warning, GFXOV_MODE_Base);
  SetGraphics("Failure", this, BHUD, BHUD_Overlay_Failure, GFXOV_MODE_Base);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Flares);
  SetClrModulation(RGBa(255,255,255,255), this, APCE_Overlay_RocketPod);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Extinguisher);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_ExtinguisherInfo);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Warning);
  SetClrModulation(RGBa(255,255,255,255), this, BHUD_Overlay_Failure);
  SetStandardColor(0,210,255,50);
  SetState(BHUD_Ready);
  return true;
}

public func SetState(int iNewState, bool fKeepSound)
{
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
  if(fRocketPod)
  {
    SetClrModulation(dwArrowColor, this, APCE_Overlay_RocketPod);
  }
  else
  {
    SetClrModulation(RGBa(255,255,255,255), this, APCE_Overlay_RocketPod);
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

protected func Timer()
{
  //Kein Helikopter oder passender Pilot vorhanden: Verschwinden
  if(!pHelicopter || !pHelicopter->GetPilot() || GetOwner(this) != GetOwner(pHelicopter->GetPilot()) || GetOwner() == NO_OWNER)
  {
    RemoveObject();
    return true;
  }
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

  //Flares, Raketenpod und Feuerl?scher
  var fUpdate = false;
  var tFlares = pHelicopter->CanDeployFlares();
  var tRocketPod = pHelicopter->~RocketPodsReady();
  var tExtinguisher = pHelicopter->CanDeployExtinguisher();
  if(fFlares != tFlares)
  {
    fUpdate = true;
    if(!fFlares) Sound("WarningFlaresReloaded.ogg", false, this, 100, GetOwner()+1);
    fFlares = tFlares;
  }
  if(fRocketPod != tRocketPod)
  {
    fUpdate = true;
    if(!fRocketPod) Sound("WarningRocketPodReloaded.ogg", false, this, 100, GetOwner()+1);
    fRocketPod = tRocketPod;
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

  //Standardform setzen
  SetObjDrawTransform(1000,0,0,0,1000,0);

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
      else if(!Random(2))
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
    {
      SetState(BHUD_Warning);
    }
    else
    {
      if(pHelicopter->GetAutopilot())
      {
        SetState(BHUD_Disabled);
      }
      else
      {
        SetState(BHUD_Ready);
      }
    }
  }
  if(iDamageRemaining > 0) iDamageRemaining--;
  if(!iDamageRemaining) fDamage = false;
  return true;
}