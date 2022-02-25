/*-- Waffengestell --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local pController;
local last_id;
local last_r, last_heli_r;
local iPat_Dir;
local heli,vis;
local pCrosshair;
local Rad,Ang;
local rot_left,rot_right;
local blinkspeed;
local fExternalAmmo;
local fUsesSpread;
local iSpreadReduction;

public func GetAttWeapon()		{return cur_Attachment;}						//Waffe
public func MaxRotLeft()		{return rot_left+GetDir(heli)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel links
public func MaxRotRight()		{return rot_right+GetDir(heli)*(180-rot_right+180-rot_left)+GetR();}	//Maximaler Winkel rechts
public func ReadyToFire()		{return 1;}								//Allzeit bereit
public func IsAiming()			{return true;}								//Geschütz immer am Zielen
public func IsThreat()			{return pController;}							//Status
public func UpdateCharge()		{return 1;}
public func BonusPointCondition()	{return false;}								//Keine Bonuspunkte (übernimmt der Blackhawk)
public func IsWeaponRack()		{return 1;}
public func IsRepairable()		{return false;}

/* Aufrufe */

public func Set(pTarget, iRad, iAng, iRotLeft, iRotRight, bool fAmmo, bool _fUsesSpread, int _iSpreadReduction)
{
  Rad = iRad;
  Ang = iAng;
  rot_left = iRotLeft;
  rot_right = iRotRight;
  heli = pTarget;
  SetObjectOrder(pTarget);
  blinkspeed = RandomX(27,33);
  SetAction("Floating");
  AddEffect("IntTimer", this, 1, 1, this);
  fExternalAmmo = fAmmo;
  fUsesSpread = _fUsesSpread;
  iSpreadReduction = _iSpreadReduction;
}

public func SetGunner(pObj, fNoAim)
{
  //Zielen & HUD
  if(pController)
    pController->SetHUDTarget(0);
  if(!pObj)
    EndAim();
  else
  {
    SetOwner(GetOwner(pObj));
    if(cur_Attachment)
    {
      SetOwner(GetOwner(pObj), cur_Attachment);
      if(!fNoAim)
      {
        InitAim();
        pObj->SetHUDTarget(GetAttWeapon());
      }
    }
  }

  //Controller setzen und eventuelles Geschützfeuer einstellen
  pController = pObj;
  if(cur_Attachment)
    cur_Attachment->~StopAutoFire();
  iPat_Dir = 0;
}

public func SetCooldown()
{
  var weapon = GetAttWeapon();
  if(!weapon) return;

  //Eventuell bereits vorhandenen Cooldown vorher entfernen
  if(GetEffect("StrikeRecharge", weapon) != 0)
    RemoveEffect("StrikeRecharge", weapon);

  AddEffect("StrikeRecharge", weapon, 1, 1, weapon);
}

public func Arm(id idWeapon, int iObject, int iRotation)
{
  //Waffe existiert?
  if(!idWeapon && !iObject) return;
  if(!GetName(0, idWeapon) && !GetName(0, iObject)) return;

  //Bei ID eine entsprechende Waffe erstellen, ansonsten vorhandenes Objekt ausrüsten
  var pWeapon;
  if(idWeapon)
  {
    pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
    Enter(this,pWeapon);
  }
  else
  {
    pWeapon = Object(iObject);
    Enter(this,pWeapon);
  }

  //Konfiguration
  SetObjectOrder(this, pWeapon, 1);
  if(!iRotation)
    aim_angle = 180;
  else
    aim_angle = iRotation;
  cur_Attachment = pWeapon;
  spread = Max(pWeapon->GetFMData(FM_MinSpread), spread);
  LocalN("controller", pWeapon) = this;

  //Neu erstellte Waffe wird nachladen
  if(idWeapon)
    Reload();

  //Effekte
  AddEffect("ShowWeapon", this, 20, 1, this);
  if(fUsesSpread && !GetEffect("DecaySpread", this))
    AddEffect("DecaySpread", this, 19, 1, this);
}

public func Disarm(int iObject)
{
  //Waffe an Anfragenden übergeben
  if(Contents() == cur_Attachment)
    Enter(Object(iObject),Contents());

  //Andere Inhalte entfernen
  while(Contents())
  {
    last_id = GetID(Contents());
    RemoveObject(Contents());
  }

  cur_Attachment = false;

  RemoveEffect("ShowWeapon", this);
  // DecaySpread bleibt bestehen, entfernt sich selbst wenn Spread=0
}

public func StopAutoFire()
{
  //An Waffe weiterleiten
  if(cur_Attachment)
    return cur_Attachment->~StopAutoFire(...);
}

public func SetAimAngle(iAngle, fNoStop)
{
  //Neuen Winkel setzen
  aim_angle = iAngle;
  //Geschützbewegung anhalten
  if(!fNoStop)
    iPat_Dir = 0;

  //Übersteuern verhindern
  if(AimAngle() < MaxRotLeft())
    aim_angle = MaxRotLeft()-GetR();
  else if(AimAngle() > MaxRotRight())
    aim_angle = MaxRotRight()-GetR();
}

/* Streuung */

local spread;

public func HasWeaponSpread()
{
  return fUsesSpread && pController->~HasWeaponSpread();
}

public func DoSpread(int iChange, int iMax, int iMin)
{
  if(!fUsesSpread)
    return;
  if(iMax) iChange = Max(0,BoundBy(spread+iChange, 0, iMax)-spread);
  spread = BoundBy(spread+iChange, iMin, CH_MaxSpread);
}

public func GetSpread()
{
  return spread;
}

public func AimAngle(int iMaxAngle, int iRange, bool bSpread)
{
  var angle = aim_angle+GetR();

  if(bSpread && fUsesSpread)
    angle += GetSpreadAOff();

  return angle;
}

private func GetSpreadAOff()
{
  return RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
}

protected func FxDecaySpreadTimer(object pTarget, int iEffect, int iTime)
{
  var pWeapon = pTarget->GetAttWeapon();

  var unspread, minspread;
  if(pWeapon)
  {
    var unspread = pWeapon->GetFMData(FM_UnSpread);
    var minspread = pWeapon->GetFMData(FM_MinSpread);
  }
  else
  {
    var unspread = 0;
    var minspread = 0;
  }

  pTarget->DoSpread(-(iSpreadReduction+unspread), 0, minspread);

  if(!pWeapon && spread == 0)
    return -1;

  return FX_OK;
}

/* Steuerung */

public func ControlLeft(pByObj)
{
  iPat_Dir = 1;
  return true;
}

public func ControlLeftDouble(pByObj)
{
  iPat_Dir = 2;
  return true;
}

public func ControlLeftReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlRight(pByObj)
{
  iPat_Dir = -1;
  return true;
}

public func ControlRightDouble(pByObj)
{
  iPat_Dir = -2;
  return true;
}

public func ControlRightReleased(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlDown(pByObj)
{
  iPat_Dir = 0;
  return true;
}

public func ControlUp(pByObj)
{
  Reload();
  return true;
}

public func ControlThrow(pByObj)
{
  if(!GetAttWeapon())
    return true;
  SetController(GetController(pByObj), GetAttWeapon());
  if(GetAttWeapon()->IsShooting() && !GetPlrCoreJumpAndRunControl(GetController(pByObj)))
    GetAttWeapon()->StopAutoFire();
  else
    GetAttWeapon()->ControlThrow(this);
  return true;
}

public func ControlDig()
{
  if(GetAttWeapon())
    GetAttWeapon()->~ControlDig(...);
}

protected func FxIntTimerTimer(object pTarget, int iEffect, int iTime)
{
  //Hosthelikopter vorhanden?
  if(!heli)
  {
    OnDestruction();
    return RemoveObject();
  }

  //Rotation des Heli abfragen
  var rot = GetR(heli) + (GetDir(heli) * 2 - 1) * (90 + Ang);

  //Und in die Positionsbestimmung einfließen lassen
  SetPosition(GetX(heli) + Sin(rot, Rad),
              GetY(heli) - Cos(rot, Rad), this());
  SetR(GetR(heli));

  //Waffe vorhanden?
  if(!GetAttWeapon())
    return;

  //Besitzer aktualisieren
  if(pController)
    SetOwner(GetOwner(pController));
  else
    SetOwner(GetOwner(heli));

  cur_Attachment->SetOwner(GetOwner());

  //Transparenz anpassen
  SetClrModulation(GetClrModulation(heli));

  //Alle X Frames
  if(!(iTime % blinkspeed) && pController)
  {
    if(heli->GetTeam())
      var rgb = GetTeamColor(heli->GetTeam());
    else if(GetOwner(heli) != NO_OWNER)
      var rgb = GetPlrColorDw(heli->GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FapLight", 0, 4, 0, 0, 60, LightenColor(rgb), this);
  }

  //Geschützbewegung

  //Rotation
  aim_angle += iPat_Dir;

  //Übersteuern verhindern
  if(AimAngle() < MaxRotLeft())
  {
    aim_angle = MaxRotLeft()-GetR();
    iPat_Dir = 0;
  }
  //Rechts?
  else if(AimAngle() > MaxRotRight())
  {
    aim_angle = MaxRotRight()-GetR();
    iPat_Dir = 0;
  }

  //Crosshair nachziehen
  if(pCrosshair)
  {
    var r = AimAngle()-GetR();
    if(last_r != r || GetR() != last_heli_r)
    {
      pCrosshair->SetAngle(r);
      last_heli_r = GetR();
      last_r = r;
    }
  }

  //An Helikopterrotation anpassen

  //Nur sichtbar, wenn Helikopter nicht drehend
  if(GetAction(heli) == "Turn")
  {
    if(vis)
    {
      SetVisibility(VIS_None, GetAttWeapon());
      SetVisibility(VIS_None);
      vis = false;
    }
  }
  else
    if(!vis)
    {
      SetVisibility(VIS_All, GetAttWeapon());
      SetVisibility(VIS_All);
      vis = true;
    }
}

public func OnEmpty()
{
  Reload();
}

private func Reload()
{
  if(GetAttWeapon()->~IsReloading())
  {
    return false;
  }

  if(!fExternalAmmo || !pController)
  {
    //Munition erzeugen
    Local(0, CreateContents(GetAttWeapon()->~GetFMData(FM_AmmoID))) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  }
  GetAttWeapon()->~Reload();
  return true;
}

public func AmmoStoring() {if(pController && UsesExternalAmmo()) return pController->~AmmoStoring(...);}
public func IsAmmoStorage() {if(pController && UsesExternalAmmo()) return pController->~IsAmmoStorage(...);}

public func UsesExternalAmmo()
{
  return fExternalAmmo;
}

/* Fadenkreuz */

private func InitAim()
{
  if(pCrosshair)
    RemoveObject(pCrosshair);

  //Besitzer wird in Init gesetzt
  pCrosshair = CreateObject(VCRH);
  pCrosshair->Init(this());
  pCrosshair->SetAngle(AimAngle());
}

private func EndAim()
{
  if(pCrosshair)
    RemoveObject(pCrosshair);
}

/* Zerstörung */

public func OnDestruction()
{
  //Waffe entfernen
  Disarm();

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",3,20,0,0,220,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));
}

public func Destruction()
{
  Disarm();
  RemoveEffect("ShowWeapon", this);
}

/* Schaden */

public func Damage()
{
  return;
}

/* Waffenende */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180,7000);
  y = -Cos(GetR()-180,7000);
  r = aim_angle+270+GetR();
  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return;
  return EffectVar(1, this, number);
}