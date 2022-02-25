/*-- Little Bird --*/

#strict 2
#include BKHK

static const LBRD_Seat_Pilot = 1;
static const LBRD_Seat_Operator1 = 2;
static const LBRD_Seat_Operator2 = 3;
static const LBRD_PilotLayer = 2;
static const LBRD_OperatorLayer = 3;

static const LBRD_OperatorSpreadReduction = 5;	//Spread-Verringerung: Je höher, desto schneller/genauer

local pTurretStation1, pTurretStation2;
local bombdropreload;

public func MaxPassengerCount()	{return 3;}
public func SoundIdle()		{return "LBRD_IdleSystem.ogg";}


/* Hitbox */

public func HitboxWidth()	{return 140;}	//Breite der Hitbox
public func HitboxHeight()	{return 45;}	//Höhe der Hitbox

/* Initialisierung */

protected func Initialize()
{
  //Keine Helikopter-Regel aktiv: Verschwinden
  if(NoHelicopters())
    return RemoveObject();

  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  SetAction("Stand");

  //Hitbox erstellen
  InitializeHitbox();

  //Pilot
  aSeats = [];

  //Geschütze aufstellen
  pTurretStation1 = CreateObject(WNRK,0,0,GetOwner());
  pTurretStation1 -> Set(this,10,90,90,270,true,true,LBRD_OperatorSpreadReduction);
  pTurretStation2 = CreateObject(WNRK,0,0,GetOwner());
  pTurretStation2 -> Set(this,10,90,90,270,true,true,LBRD_OperatorSpreadReduction);
  pRocketStation = CreateObject(WNRK,0,0,GetOwner());
  pRocketStation -> Set(this,20,10,270,270);
  pRocketStation -> Arm(LBRP);

  //Scheinwerfer einrichten
  pSpotlight = [0];

  //Rotorgeschwindigkeit
  iRotorSpeed = 0;

  //Vertices richtig drehen
  for(var i = 0; i < GetDefCoreVal("Vertices", "DefCore", GetID()); i++)
    SetVertex(i, 0, GetDefCoreVal("VertexX", "DefCore", GetID(), i), this, 2);

  //Eingang erstellen
  pEntrance = CreateObject(ENTR, 0, 0, GetOwner());
  pEntrance->SetHelicopter(this);
  pEntrance->SetOffset(12 * (GetDir()*2-1), 6);

  //Bombenladezeit setzen
  bombdropreload = 35 * 5;

  //Neutrale Fahrzeuge sind weiß
  if(GetOwner() == NO_OWNER)
  {
    SetColorDw(RGB(255,255,255));
  }
  else
  {
    SetColorDw(GetPlrColorDw(GetOwner()));
  }

  AddEffect("VehicleNoOwner", this, 50, 38, this);

  return 1;
}

/* Erfassung */

public func & GetPilot()	{return aSeats[LBRD_Seat_Pilot];}
public func & GetOperator1()	{return aSeats[LBRD_Seat_Operator1];}
public func & GetOperator2()	{return aSeats[LBRD_Seat_Operator2];}

/* Eingangssteuerung */

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    //Freund: Rein. Feind: Raus
    if(!Hostile(GetOwner(this),GetOwner(pObj)) || (GetOwner() == -1) || !GetPassengerCount())
    {
      //Soundschleife übergeben
      SoundPassenger("CockpitRadio.ogg", true, GetOwner(pObj));

      //Freien Sitz belegen
      if(!GetPilot())
        return EnterSeat(LBRD_Seat_Pilot, pObj);
      if(!GetOperator1())
      {
        Sound("StructureEnter*.ogg", true, this, 50, GetOwner(GetPilot()) + 1);
        return EnterSeat(LBRD_Seat_Operator1, pObj);
      }
      if(!GetOperator2())
      {
        Sound("StructureEnter*.ogg", true, this, 50, GetOwner(GetPilot()) + 1);
        return EnterSeat(LBRD_Seat_Operator2, pObj);
      }

      //Kein Platz mehr
      return SetCommand(pObj, "Exit");
    }
    else
      return SetCommand(pObj, "Exit");
  }
}

/* Steuerung */

protected func ContainedUp(object ByObj)
{
  [$CtrlUp$]

  //Pilot
  if(ByObj == GetPilot())
  {
    if(GetY() < 80)
      return;
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter inaktiv: Hochfahren
    if(!throttle && (GetAction() == "Stand"))
      SetAction("EngineStartUp");
    //Helikopter am herunterfahren: Aktion umkehren und Steuerung sperren
    if(!enginelock)
    {
      if(GetAction() == "EngineShutDown")
      {
        SetAction("EngineStartUp3");
        StartEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineShutDown2")
      {
        SetAction("EngineStartUp2");
        StartEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineShutDown3")
      {
        SetAction("EngineStartUp");
        StartEngine();
        enginelock = 1;
      }
    }

    //Schub erhöhen
    if(!GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())))
    {
      if(GetAction() == "Fly" || GetAction() == "Turn")
        throttle = BoundBy(throttle + BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
    }
    else
    {
      AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), BKHK_ThrottleSpeed);
    }
  }

  //Operator-Speedmenü
  var operator;
  if(ByObj == GetOperator1())
    operator = 1;
  else
    if(ByObj == GetOperator2())
      operator = 2;
  if(operator)
  {
    //Waffe ermitteln
    if(operator == 2)
    {
      var TurretWeapon = Contents(0,pTurretStation2);
      var Turret = pTurretStation2;
    }
    else
    {
      var TurretWeapon = Contents(0,pTurretStation1);
      var Turret = pTurretStation1;
    }

    //Kein Waffe vorhanden: Auswahlmenü
    if(!TurretWeapon)
      MountWeaponMenu(ByObj);
    else
    {
      var ring = CreateSpeedMenu(this, ByObj);

      var overlay;

      //Waffe montieren
      overlay = ring->AddUpItem("$MountWeapon$", "MountWeaponMenu", ByObj, SMIN, 0, ObjectNumber(ByObj));
      SetGraphics("13", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

      //Waffe nachladen
      if(TurretWeapon && TurretWeapon->GetAmmoCount(TurretWeapon->GetSlot()) < TurretWeapon->GetFMData(FM_AmmoLoad))
      {
        overlay = ring->AddThrowItem("$Reload$", "ReloadWeapon", ByObj, SMIN);
        SetGraphics("1", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
      }

      //Waffe abmontieren
      overlay = ring->AddDownItem("$UnmountWeapon$", "UnmountWeapon", ByObj, SMIN, 0, ObjectNumber(ByObj));
      SetGraphics("12", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

      return Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(ByObj) + 1);
    }
  }

  return true;
}

/* Passagierfähigkeiten */

/* Waffe nachladen */

public func ReloadWeapon(object ByObj)
{
  //Waffe nachladen
  if(ByObj == GetOperator1())
    pTurretStation1->~ControlUp(ByObj);
  if(ByObj == GetOperator2())
    pTurretStation2->~ControlUp(ByObj);
}

/* Waffe montieren */

public func MountWeaponMenu(object pObj)
{
  //Passende Waffen ermitteln
  var wpn =  FindObjects(Find_Container(pObj),Find_Func("IsWeapon2"));
  var len = GetLength(wpn);
  if(len == 0)
  {
    //Keine Waffen gefunden
    PlayerMessage(GetOwner(pObj), "$NoWeapon$", pObj);
  }
  else if(len == 1)
  {
    //Nur eine Waffe: Montieren
    MountWeapon(GetID(wpn[0]), pObj);
  }
  else
  {
    //Mehrere Waffen: Auswahlmenü öffnen
    CreateMenu(GetID(), pObj, this, 0, "$MountWeapon$", 0, C4MN_Style_Context);

    for(var obj in wpn)
      AddMenuItem(Format("%s", GetName(obj)), "MountWeapon", GetID(obj), pObj, 0, pObj, 0, C4MN_Add_ForceNoDesc);
  }
}

public func MountWeapon(id idWeapon, object pPassenger)
{
  if(!pPassenger || !FindContents(idWeapon,pPassenger)) return;

  var station;
  if(pPassenger == GetOperator1())
    station = pTurretStation1;
  else
    station = pTurretStation2;

  var wpn = Contents(0,station);
  if(wpn) UnmountWeapon(pPassenger);

  wpn = ObjectNumber(FindContents(idWeapon,pPassenger));
  station->~Arm(0, wpn);
  station->~SetCooldown();
  CreateParticle("PSpark", 0, 0, 0, 0, 150, GetPlrColorDw(Object(wpn)->GetOwner()), Object(wpn));

  //Waffen-HUD aktualisieren
  pPassenger->SetHUDTarget(Object(wpn));
  station->~EndAim();
  station->~InitAim();
  station->~ControlDown();

  //Waffenname einblenden
  PlayerMessage(GetOwner(pPassenger), Format("<c ffff33>%s</c>", GetName(Object(wpn))), station);

  Sound("BKHK_Handle*.ogg", false, station);
  Sound("WNAT_AddAttachement.ogg", false, station);
}

public func UnmountWeapon(object pClonk)
{
  if(!pClonk) return;

  var station, weapon;
  if(pClonk == GetOperator1())
    station = pTurretStation1;
  else
    station = pTurretStation2;
  if(!station) return;
  weapon = Contents(0,station);
  if(!weapon) return;

  station->~Disarm(ObjectNumber(pClonk));
  station->~EndAim();

  //Waffen-HUD aktualisieren
  pClonk->SetHUDTarget(0);

  Sound("BKHK_Handle*.ogg", false, station);
  Sound("WNAT_RemoveAttachement.ogg", false, station);
}

protected func ContainedDown(object ByObj)
{
  [$CtrlDown$]

  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter aktiv: Herunterfahren
    if(!throttle && (GetAction() == "Fly") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Helikopter am hochfahren: Aktion umkehren und Steuerung sperren
    if(!enginelock)
    {
      if(GetAction() == "EngineStartUp")
      {
        SetAction("EngineShutDown3");
        StopEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineStartUp2")
      {
        SetAction("EngineShutDown2");
        StopEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineStartUp3")
      {
        SetAction("EngineShutDown");
        StopEngine();
        enginelock = 1;
      }
    }
    //Schub verringern
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())))
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), -BKHK_ThrottleSpeed);
      else
        throttle = BoundBy(throttle - BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
  }

  //Operator 1
  if(ByObj == GetOperator1())
    //Waffe nachladen
    pTurretStation1->~ControlDown(ByObj);

  //Operator 2
  if(ByObj == GetOperator2())
    //Waffe nachladen
    pTurretStation2->~ControlDown(ByObj);

  return true;
}

protected func ContainedDownDouble(object ByObj)
{
  [$CtrlDownD$]

  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter aktiv: Herunterfahren
    if(throttle == 0 && (GetAction() == "Fly") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Helikopter am hochfahren: Aktion umkehren und Steuerung sperren
    if(!enginelock)
    {
      if(GetAction() == "EngineStartUp")
      {
        SetAction("EngineShutDown3");
        StopEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineStartUp2")
      {
        SetAction("EngineShutDown2");
        StopEngine();
        enginelock = 1;
      }
      if(GetAction() == "EngineStartUp3")
      {
        SetAction("EngineShutDown");
        StopEngine();
        enginelock = 1;
      }
    }
    //Schub verringern
    if(GetAction() == "Fly")
      throttle = BoundBy(throttle - BKHK_ThrottleSpeed*2, 0, 170);
  }

  return true;
}

protected func ContainedLeft(object ByObj)
{
  [$CtrlLeft$]

  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter neigen
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
        rotation = -BKHK_MaxRotation;
      else
        rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Operator 1
  if(ByObj == GetOperator1())
    //Waffe nachladen
    pTurretStation1->~ControlLeft(ByObj);

  //Operator 2
  if(ByObj == GetOperator2())
    //Waffe nachladen
    pTurretStation2->~ControlLeft(ByObj);

  return true;
}

protected func ContainedRight(object ByObj, fRelease)
{
  [$CtrlRight$]
  
  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter neigen
    if(fRelease)
      rotation = GetR();
    else 
      if(GetAction() == "Fly" || GetAction() == "Turn")
        if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
          rotation = BKHK_MaxRotation;
        else
          rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Operator 1
  if(ByObj == GetOperator1())
    //Waffe nachladen
    pTurretStation1->~ControlRight(ByObj);

  //Operator 2
  if(ByObj == GetOperator2())
    //Waffe nachladen
    pTurretStation2->~ControlRight(ByObj);

  return true;
}

protected func ContainedLeftReleased(object ByObj)
{
  if(ByObj == GetPilot())
    rotation = GetR();
  if(ByObj == GetOperator1())
    pTurretStation1->~ControlLeftReleased(ByObj);
  if(ByObj == GetOperator2())
    pTurretStation2->~ControlLeftReleased(ByObj);
}

protected func ContainedRightReleased(object ByObj)
{
  if(ByObj == GetPilot())
    rotation = GetR();
  if(ByObj == GetOperator1())
    pTurretStation1->~ControlRightReleased(ByObj);
  if(ByObj == GetOperator2())
    pTurretStation2->~ControlRightReleased(ByObj);
}

protected func ContainedLeftDouble(object ByObj)
{
  [$CtrlLeftD$]
  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter neigen
    if(GetDir() && GetAction() == "Fly")
      if(GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Operator 1
  if(ByObj == GetOperator1())
    //Waffe nachladen
    pTurretStation1->~ControlLeftDouble(ByObj);

  //Operator 2
  if(ByObj == GetOperator2())
    //Waffe nachladen
    pTurretStation2->~ControlLeftDouble(ByObj);

  return true;
}

protected func ContainedRightDouble(object ByObj)
{
  [$CtrlRightD$]
  
  //Pilot
  if(ByObj == GetPilot())
  {
    //Autopilot stoppen
    ResetAutopilot();
    //Helikopter neigen
    if(!GetDir() && GetAction() == "Fly")
      if(GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Operator 1
  if(ByObj == GetOperator1())
    //Waffe nachladen
    pTurretStation1->~ControlRightDouble(ByObj);

  //Operator 2
  if(ByObj == GetOperator2())
    //Waffe nachladen
    pTurretStation2->~ControlRightDouble(ByObj);

  return true;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if(GetDamage() > MaxDamage())
    return true;
  //Piloten-Speedmenü
  if(ByObj == GetPilot())
  {
    var ring = CreateSpeedMenu(this, ByObj);

    var overlay;

    //Flareabwurf
    //Nur wenn geladen
    if(CanDeployFlares())
    {
      overlay = ring->AddThrowItem("$Flares$", "DeployFlares", ByObj, SMIN);
      SetGraphics("6", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //Raketenwerfer
    if(RocketPodsReady())
    {
      overlay = ring->AddLeftItem("$RocketLauncher$", "FireRockets", ByObj, SMIN);
      SetGraphics("10", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //Feuerlöscher
    //Nur wenn geladen
    if(CanDeployExtinguisher())
    {
      overlay = ring->AddUpItem("$Extinguisher$", "DeployExtinguisher", ByObj, SMIN);
      SetGraphics("11", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //HUD ein- oder ausblenden
    overlay = ring->AddRightItem("$HUD$", "SwitchHUD", ByObj, SMIN);
    SetGraphics("8", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    //Bombenabwurf
    if(BombDropReady())
    {
      overlay = ring->AddDownItem("$BombDrop$", "BombDrop", ByObj, SMIN);
      SetGraphics("14", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    return Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(ByObj) + 1);
  }

  //Operator 1: Feuer eröffnen/einstellen
  if(ByObj == GetOperator1())
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
    {
      var TurretWeapon = pTurretStation1->GetAttWeapon();
      if(TurretWeapon)
      {
        if(GetPilot())
          if(TurretWeapon->GetAmmoCount(TurretWeapon->GetSlot()) == 0)
            pTurretStation1->~ControlUp(ByObj);
          else
            pTurretStation1->~ControlThrow(ByObj);
        else
          RefuseLaunch(ByObj);
      }
      else
        MountWeaponMenu(ByObj);
    }

  //Operator 2: Feuer eröffnen/einstellen
  if(ByObj == GetOperator2())
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
    {
      var TurretWeapon = pTurretStation2->GetAttWeapon();
      if(TurretWeapon)
      {
        if(GetPilot())
          if(TurretWeapon->GetAmmoCount(TurretWeapon->GetSlot()) == 0)
            pTurretStation2->~ControlUp(ByObj);
          else
            pTurretStation2->~ControlThrow(ByObj);
        else
          RefuseLaunch(ByObj);
      }
      else
        MountWeaponMenu(ByObj);
    }

  return true;
}

/* Sitzsteuerung */

protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(), ByObj, this, 0, "$Seats$", 0, 1);

  //Ausstieg
  AddMenuItem("<c ffff33>$Exit$</c>", "ExitClonk", STIN, ByObj, 0, ByObj, "$ExitDesc$");

  //Pilot
  if(GetPilot())
    AddMenuItem("<c 777777>$Pilot$</c>", "SeatOccupied", STIN, ByObj, 0, ByObj, "$SeatOccupied$",2,1);
  else
    AddMenuItem("<c ffffff>$Pilot$</c>", Format("EnterSeat(%d, Object(%d))", LBRD_Seat_Pilot, ObjectNumber(ByObj)), STIN, ByObj, 0, ByObj, "$PilotDesc$",2,1);

  //Operator 1
  var wpn = pTurretStation1->GetAttWeapon();
  if(wpn) var str = Format("%s-",GetName(wpn)); else var str = "";
  if(GetOperator1())
    //AddMenuItem("<c 777777>$Operator$</c>", "SeatOccupied", STIN, ByObj, 0, ByObj, "$SeatOccupied$",2,4);
    AddMenuItem(Format("<c 777777>%s$Operator$</c>",str), "SeatOccupied", STIN, ByObj, 0, ByObj, "$SeatOccupied$",2,4);
  else
    //AddMenuItem("<c ffffff>$Operator$</c>", Format("EnterSeat(%d, Object(%d))", LBRD_Seat_Operator1, ObjectNumber(ByObj)), STIN, ByObj, 0, ByObj, "$OperatorDesc$",2,4);
    AddMenuItem(Format("<c ffffff>%s$Operator$</c>",str), Format("EnterSeat(%d, Object(%d))", LBRD_Seat_Operator1, ObjectNumber(ByObj)), STIN, ByObj, 0, ByObj, "$OperatorDesc$",2,4);

  //Operator 2
  var wpn = pTurretStation2->GetAttWeapon();
  if(wpn) var str = Format("%s-",GetName(wpn)); else var str = "";
  if(GetOperator2())
    AddMenuItem(Format("<c 777777>%s$Operator$</c>",str), "SeatOccupied", STIN, ByObj, 0, ByObj, "$SeatOccupied$",2,4);
  else
    AddMenuItem(Format("<c ffffff>%s$Operator$</c>",str), Format("EnterSeat(%d, Object(%d))", LBRD_Seat_Operator2, ObjectNumber(ByObj)), STIN, ByObj, 0, ByObj, "$OperatorDesc$",2,4);

  //Ausstieg per Seil
  AddMenuItem("<c ffff33>$ExitRope$</c>", "ExitClonkByRope", STIN, ByObj, 0, ByObj, "$ExitRopeDesc$",2,5);

  return 1;
}

public func ContainedUpdate(object ByObj, int comdir, bool dig, bool throw)
{
  if(ByObj == GetOperator1())
  {
    if(throw && !GetPilot())
    {
      pTurretStation1->StopAutoFire();
      return RefuseLaunch(ByObj);
    }

    if(throw)
      return pTurretStation1->ControlThrow(ByObj);
    else
      return pTurretStation1->StopAutoFire();
  }

  if(ByObj == GetOperator2())
  {
    if(throw && !GetPilot())
    {
      pTurretStation2->StopAutoFire();
      return RefuseLaunch(ByObj);
    }

    if(throw)
      return pTurretStation2->ControlThrow(ByObj);
    else
      return pTurretStation2->StopAutoFire();
  }
}

private func DeleteActualSeatPassenger(object pObj)
{
  if(!pObj) return;
  if(GetPilot() == pObj)
  {
    GetPilot() = 0;
    if(pRocketStation) pRocketStation->SetGunner(0);
    if(hud) RemoveObject(hud);
    SetGraphics(0, this, EFMN,LBRD_PilotLayer, GFXOV_MODE_Object, 0, GFX_BLIT_Additive, this);
    UpdateWarnings();
  }
  if(GetOperator1() == pObj)
  {
    UnmountWeapon(pObj);
    GetOperator1() = 0;
    if(pTurretStation1) pTurretStation1->SetGunner(0);
  }
  if(GetOperator2() == pObj)
  {
    UnmountWeapon(pObj);
    GetOperator2() = 0;
    if(pTurretStation2) pTurretStation2->SetGunner(0);
  }

  //Falls keine Passagiere außer Pilot mehr da
  if((!GetPassengerCount() && !GetPilot()) || (GetPassengerCount() == 1 && GetPilot()))
    SetGraphics(0, this, EFMN, LBRD_OperatorLayer, GFXOV_MODE_Object, 0, GFX_BLIT_Additive, this);
  return 1;
}

public func EnterSeat(int iSeat, object pObj)
{
  //Besetzt
  if(aSeats[iSeat])
  {
    SeatOccupied(0, pObj);
    return false;
  }

  //Alten Sitz räumen
  DeleteActualSeatPassenger(pObj);

  Sound("StructureEnter*.ogg", true, this, 100, GetOwner(pObj) + 1);

  //Pilot
  if(iSeat == LBRD_Seat_Pilot)
  {
    if(GetOwner() == -1 || GetPlayerTeam(GetOwner()) != GetPlayerTeam(GetOwner(pObj)))
    {
      //Besitz ergreifen
      SetOwnerFade(GetOwner(pObj));
      SetOwner(GetOwner(pObj), pEntrance);
    }
    SetGraphics("Pilot", this, GetID(), LBRD_PilotLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetPilot() = pObj;
    pRocketStation->SetGunner(GetPilot(),true);
    pObj->~SetHUDTarget(pRocketStation->GetAttWeapon());

    hud = CreateObject(LHUD, 0, 0, GetOwner(pObj));
    hud->~SetHelicopter(this);
    UpdateWarnings();
    return true;
  }

  //Operator 1
  if(iSeat == LBRD_Seat_Operator1)
  {
    SetGraphics("Operator", this, GetID(), LBRD_OperatorLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetOperator1() = pObj;
    pTurretStation1->SetGunner(GetOperator1());
    if(pTurretStation1->GetAttWeapon())
      PlayerMessage(GetOwner(pObj), Format("<c ffff33>%s</c>", GetName(pTurretStation1->GetAttWeapon())), pObj);
    else
      MountWeaponMenu(pObj);

    return true;
  }

  //Operator 2
  if(iSeat == LBRD_Seat_Operator2)
  {
    SetGraphics("Operator", this, GetID(), LBRD_OperatorLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetOperator2() = pObj;
    pTurretStation2->SetGunner(GetOperator2());
    if(pTurretStation2->GetAttWeapon())
      PlayerMessage(GetOwner(pObj), Format("<c ffff33>%s</c>", GetName(pTurretStation2->GetAttWeapon())), pObj);
    else
      MountWeaponMenu(pObj);

    return true;
  }
}

protected func ControlCommand(string szCommand, object Target, int TargetX, int TargetY, object target2, int Data, object ByObj)
{
  if(szCommand == "Exit")
  {
    var rot = GetDir()*180-90 + GetR() + GetDir()*120-60;
    Exit(ByObj, Sin(rot,25), -Cos(rot,25), GetR(), GetXDir(0,1), GetYDir(0,1), GetRDir());
    return true;
  }
  if(szCommand == "MoveTo" || szCommand == "Attack")
  {
    if(ByObj == GetPilot())
      SetAutopilot(Target, TargetX, TargetY);
    else if(ByObj == GetOperator1())
      pTurretStation1->~ControlCommand(szCommand, Target, TargetX, TargetY, target2, Data, ByObj);
    else if(ByObj == GetOperator2())
      pTurretStation2->~ControlCommand(szCommand, Target, TargetX, TargetY, target2, Data, ByObj);
    return true;
  }
}

protected func Ejection(object ByObj)
{
  if(!ByObj)
    return;
  if(!(GetOCF(ByObj) & OCF_CrewMember))
    return;

  if(GetPilot() == ByObj)
    ByObj->~SetHUDTarget(0);

  //Erst mal löschen
  DeleteActualSeatPassenger(ByObj);

  //Soundschleife übergeben
  SoundPassenger("CockpitRadio.ogg", false, GetOwner(ByObj));
  SoundPassenger("WarningNoPilot.ogg", false, GetOwner(ByObj));

  //Nicht bei Schaden
  if(GetDamage() >= MaxDamage()) return;

  SetPosition(GetX(pEntrance), GetY(pEntrance)-1, ByObj);
  AddEffect("HeliEnterCooldown", ByObj, 1, 40);

  var x = GetX(ByObj), y = GetY(ByObj), xdir = GetXDir(ByObj, 100), ydir = GetYDir(ByObj, 100);

  //Prüfen, ob der Clonk den Boden erreichen wird
  var material = SimFlight(x, y, xdir, ydir, 0, 0, 0, 100);
  if(material && Distance(xdir, ydir) < 700)
    return;

  if(!GetEffect("CheckGround",ByObj))
  {
    if(ByObj->~IsArmed())
      ByObj->SetAction("JumpArmed");
    else
      ByObj->SetAction("Jump");

    CreateObject(PARA,GetX(ByObj),GetY(ByObj),GetOwner(ByObj))->Set(ByObj);
    AddEffect("Flying", ByObj, 101, 5);
  }

  return true;
}

/* Pilotenfähigkeiten */

/* Raketenwerfer */

public func FireRockets()
{
  //Feuerbefehl geben
  pRocketStation->~ControlThrow(GetPilot());
}

public func RocketPodsReady()
{
  var weapon;
  if(!pRocketStation || !(weapon = pRocketStation->~GetAttWeapon()))
    return false;

  return (GetAmmo(0, weapon) == weapon->GetFMData(FM_AmmoLoad));
}

/* Schaden */

public func OnDestruction()
{
  //Inhalt auswerfen und töten bzw. zerstören
  for(var obj in FindObjects(Find_Container(this), Find_Not(Find_ID(FKDT))))
  {
    DeleteActualSeatPassenger(obj);
    if(GetOCF(obj) & OCF_Alive && GetID(Contained(obj)) != FKDT)
      DoDmg(200, DMG_Explosion, obj, 0, GetLastAttacker()+1);
    else
      DoDamage(200, obj);
    if(Contained(obj) == this)
      Exit(obj, 0, 0, Random(360), RandomX(-5, 5), RandomX(-4, 8), Random(10));
  }

  //Montierte Waffen auswerfen
  if(pTurretStation1 && pTurretStation1->GetAttWeapon())
    Exit(pTurretStation1->GetAttWeapon(),0,0,0,GetXDir(),GetYDir(),RandomX(0,30));
  if(pTurretStation2 && pTurretStation2->GetAttWeapon())
    Exit(pTurretStation2->GetAttWeapon(),0,0,0,GetXDir(),GetYDir(),RandomX(0,30));

  //Eingang entfernen
  RemoveObject(pEntrance, true);

  //Explosion
  FakeExplode(70, GetLastAttacker() + 1);
  FakeExplode(50, GetLastAttacker() + 1);
  RemoveObject();
  Sound("ExplosionHuge.ogg", false, this);
  Sound("StructureHit*.ogg", false, this);

  //Wrack erstellen
  var obj = CreateObject(LBWK, 0, 20, GetLastAttacker());
  obj->~GetWrecked(throttle);
  SetDir(GetDir(), obj);
  SetR(GetR(), obj);
  SetXDir(GetXDir(), obj);
  SetYDir(GetYDir(), obj);
  SetRDir(GetRDir(), obj);
  return true;
}

protected func TimerCall()
{
  //Nachladezeit reduzieren
  if(bombdropreload) 
    bombdropreload--;

  return _inherited(...);
}

public func BombDropReady()
{
  return !bombdropreload;
}

/* Pilotenfähigkeiten */

/* Bombenabwurf */

public func BombDrop()
{
  //Nicht genug Platz: Abbruch
  if(!PathFree(GetX(),GetY()+25,GetX(),GetY()+60))
    return PlayerMessage(GetOwner(GetPilot()), "$NotAbleToDrop$", GetPilot());

  //Bombe abwerfen
  var grenade = CreateObject(PSHL, 0, 25, GetOwner(GetPilot()));
  SetController(GetOwner(GetPilot()), grenade);
  grenade->~Launch(GetXDir(),GetYDir(),0,0,0,0,0,this);
  grenade->~CreateParticle("NoGravSpark",0,0,0,0,50,GetPlrColorDw(GetOwner(GetPilot())), this);

  //Sound
  Sound("LBRD_DeployBombDrop.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
  Sound("LBRD_BombRelease*.ogg");

  //Bombenladezeit setzen
  bombdropreload = 35 * 15;
}

/* Fahrzeugrotation */

protected func OnChangeDir()
{
  //Eingangsposition
  pEntrance->SetOffset(12 * (GetDir()*2-1), 6);
}