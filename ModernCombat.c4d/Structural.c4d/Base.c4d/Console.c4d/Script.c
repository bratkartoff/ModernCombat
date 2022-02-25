/*-- Konsole --*/

#strict 2

local target;

public func IsConsole()			{return true;}
public func IsMachine()			{return 1;}
public func GetRealRepairableObject()	{return target;}
public func IsFakeRepairable(int iPlr)
{
  var fAdd = GetDamage(target);
  if(Hostile(iPlr, GetOwner(target)) || HostileTeam(GetPlayerTeam(iPlr), target->~GetTeam()))
    fAdd = true;

  return(target && target->~IsRepairable() && fAdd);
}


/* Timer */

protected func Timer(pClonk)
{
  //Kein Zielobjekt: Selbstzerstörung
  if(!target)
  {
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,50,0,0,40,150);
    CastSmoke("Smoke3",5,10,0,0,50,200, RGBa(255,255,255,0));
    Sound("StructureHit*.ogg");
    return RemoveObject();
  }

  //Blinkeffekt
  AddLightFlash(100,0,0,RGB(0,0,255));
  CreateParticle("NoGravSpark", -8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
  CreateParticle("NoGravSpark", 8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
}

/* Anfassen und Loslassen */

protected func Grabbed(object pClonk, bool fGrab)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  if(fGrab)
  {
    //Sicht auf Zielobjekt zentrieren und Objektnamen anzeigen
    SetPlrView(GetController(pClonk),target);
    PlayerMessage(GetController(pClonk),Format("{{%i}} <c ffff33>%s</c>", GetID(target), GetName(target)),target);

    Sound("Acknowledge.ogg", 0, target, 100, GetOwner(pClonk)+1);
  }
  else
    PlayerMessage(GetController(pClonk),"",target);
}

protected func GrabLost(object pClonk)
{
  Grabbed(pClonk, false);
}

/* Steuerung */

protected func ControlThrow(pClonk)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pClonk),target);

  //Ersten Menüpunkt ausführen
  ControlTarget(1, ObjectNumber(pClonk), ObjectNumber(this));

  //Effekte
  Sound("Switch.ogg", 0, target, 100, GetOwner(pClonk)+1);
  Sound("Switch.ogg");

  return 1;
}

public func ControlUp(object pByObj)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pByObj),target);

  OpenControlMenu(pByObj);
}

protected func ControlDig(object pByObj)
{
  //Keine Aktion wenn kein Ziel
  if(!target) return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pByObj),target);
  OpenControlMenu(pByObj);
}

protected func OpenControlMenu(object pClonk)
{
  //Menü erstellen
  CreateMenu(GetID(), pClonk, this, 0, Format("%s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i, pClonk, this); i++)
    AddMenuItem(desc, Format("ControlTarget(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this)), GetID(target), pClonk);
  if(GetMenuSelection(pClonk) == -1)
  {
    CloseMenu(pClonk);
    return;
  }

  return 1;
}

protected func ControlTarget(int selection, int objn, int objn2)
{
  if(!target)
    return;

  //Befehl an Ziel
  target->~ConsoleControlled(selection, objn, objn2);

  //Effekte
  Sound("Switch.ogg", 0, target, 100, GetOwner(Object(objn))+1);
  Sound("Switch.ogg");
}

/* Ziel setzen */

public func Set(pTarget)
{
  target = pTarget;
}

/* HUD */

public func CustomHUD(int source)
{
  if(target)
    return source == HUD_Grabbed;
  else
    return false;
}

public func UpdateHUD(object hud, int source)
{
  hud->~HUDMessage(Format("%s (%s)", GetName(), GetName(target)), source, true);
}