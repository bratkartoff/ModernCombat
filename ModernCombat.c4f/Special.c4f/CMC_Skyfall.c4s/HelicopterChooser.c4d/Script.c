/*-- Helikopterwähler --*/

#strict 2

local iClass;
local spawn,helicopters,clonk,selection,oldvisrange,oldvisstate;

public func IsSpawnObject()	{return true;}


/* Initalisierung */

public func Initialize()
{
  SetVisibility(VIS_Owner);
}

global func CreateHelicopterSpawner(object pCrew, int iChoosedClass)
{
  if(!pCrew) pCrew = this;
  if(!pCrew) return false;

  var spawner = CreateObject(HSPW);
  SetPosition(GetX(pCrew),GetY(pCrew),spawner);
  Enter(spawner, pCrew);

  LocalN("iClass", spawner) = iChoosedClass;

  return spawner;
}

protected func Collection2(object pObject)
{
  //Clonk verifizieren
  if(!(GetOCF(pObject) & OCF_CrewMember)) return;
  SetOwner(GetController(pObject));
  clonk = pObject;

  //Bei Initialisierung Werte speichern
  oldvisrange = GetObjPlrViewRange(clonk);
  oldvisstate = GetPlrFogOfWar(GetOwner(clonk));

  SetFoW(true, GetOwner(clonk));

  //Zeitbegrenzung bei AS, LMS und DM
  if(FindObject(GASS) || FindObject(GLMS) || FindObject(GTDM))
    AddEffect("Spawntimer", this, 100, 35, this, GetID(), GetOwner(pObject), pObject, this);

  SpawnMenu();
}

public func ContainedLeft()	{return 1;}
public func ContainedRight()	{return 1;}
public func ContainedDown()	{return 1;}
public func ContainedUp()	{return 1;}
public func ContainedDig()	{return 1;}
public func ContainedThrow()	{return 1;}

public func SpawnMenu()
{
  var obj = Contents();
  if(!obj) return;

  var iTeam = GetPlayerTeam(obj->GetOwner());
  if(!iTeam) return;

  //Menü öffnen
  CloseMenu(obj);
  CreateMenu(HSPW,clonk,this,C4MN_Extra_Info,"$SpawnMenu$",0,C4MN_Style_Dialog,true);

  //Überschrift
  AddMenuItem("<c ffff33>$NewHelicopter$</c>",0,NONE,clonk,0,0," ");

  //Alle Helikoptertypen anbieten
  var def, i;
  while(def = GetDefinition(i++, C4D_Vehicle))
    if(def->~IsHelicopter())
      AddMenuItem(GetName(0, def), Format("SetupVehicle(%d, %i)", iTeam, def), def, obj);

  //Vorhandene, verbündete Helikopter suchen
  var menuentry = false;
  for(var heli in FindObjects(Find_Func("IsHelicopter")))
  {
    //Nicht mehr vorhanden?
    if(!heli)
      continue;
    //Kein Pilot?
    if(!heli->~GetPilot())
      continue;
    //Verfeindet?
    if(Hostile(obj->GetOwner(), heli->GetOwner()))
      continue;
    //Komplett belegt?
    if(heli->~GetPassengerCount() >= heli->~MaxPassengerCount())
      continue;
    if(!menuentry)
    {
      //Leerzeile und Überschrift
      AddMenuItem(" ", 0,NONE,obj,0,0," ");
      AddMenuItem("<c ffff33>$AlliedHelicopter$</c>",0,NONE,clonk,0,0," ");
    }
    menuentry = true;
    var maxdmg = heli->MaxDamage();
    AddMenuItem(Format("%s|%s ({{SM12}} %d%)",GetTaggedPlayerName(heli->GetOwner()),GetName(heli),(maxdmg-heli->GetDamage())*100/maxdmg), Format("SetupVehicle(%d, Object(%d))", iTeam, heli->ObjectNumber()), heli->GetID(), obj);
  }
  if(!menuentry)
  {
    //Leerzeile und Überschrift
    AddMenuItem(" ", 0,NONE,obj,0,0," ");
    AddMenuItem("<c 777777>$AlliedHelicopterUnavailable$</c>",0,NONE,clonk,0,0," ");
  }

  SelectMenuItem(selection,obj);
}

public func SetupVehicle(int iPlr, spawn)
{
  //Clonk verifizieren
  if(!clonk)				return RemoveObject();
  if(GetOwner(clonk) == NO_OWNER)	return RemoveObject();

  //Kein Spawnort: Zurück zum Auswahlmenü
  if(!spawn)
    return SpawnMenu();
  else
  {
    //Menü schließen
    CloseMenu(clonk);

    var helicopter = spawn;
    //Bei ID einen entsprechenden Helikopter erstellen, ansonsten in vorhandenen Helikopter einsteigen lassen
    if(GetType(spawn) == C4V_C4ID)
    {
      //Spawnposition ermitteln
      var iX, iY, aSpawnpoints;
      var iTeam = GetPlayerTeam(iPlr);
      aSpawnpoints = GameCall("RelaunchPosition",iX, iY, iTeam);
      if(GetType(aSpawnpoints) == C4V_Array)
        GetBestSpawnpoint(aSpawnpoints, iPlr, iX, iY);
      if(Contained(clonk))
      {
        Exit(clonk);
        SetPosition(iX, iY, clonk);
        //SetPosition(iX, iY, Contained(clonk));
      }
      else
        SetPosition(iX, iY, clonk);

      helicopter = CreateObject(spawn,0,GetDefCoreVal("Height", "DefCore", spawn),GetOwner(clonk));
      SetController(GetOwner(clonk), helicopter);
      if(GetX() < LandscapeWidth()/2)
        helicopter->ChangeDir();
      helicopter->SetAction("Fly");
      helicopter->EngineStarted();
      LocalN("throttle", helicopter) = 100;
      Enter(helicopter, clonk);
    }
    else
    {
      //Kein Sitzplatz frei: Zurück zum Auswahlmenü
      if(spawn->~GetPassengerCount() >= spawn->~MaxPassengerCount())
        return SpawnMenu();
      //Pilot informieren
      var pilot = helicopter->GetPilot();
      PlayerMessage(GetOwner(pilot), Format("$PlayerSpawned$",GetPlayerName(GetOwner(clonk))), pilot);
      //Clonk hineinsetzen
      Enter(helicopter, clonk);
    }

    //Effekt entfernen
    for(var i = 0; i < GetEffectCount("Spawntimer", this); i++)
      if(EffectVar(0, this, GetEffect("Spawntimer", this, i)) == GetOwner(clonk))
        RemoveEffect("Spawntimer", this, i);
    PlayerMessage(GetOwner(clonk), "@");

    //Sichtdaten zurücksetzen
    SetFoW(oldvisstate,GetOwner(clonk));
    SetPlrViewRange(oldvisrange,clonk);

    return RemoveObject();
  }
}

protected func Timer()
{
  if(spawn) return;

  var crew = Contents();
  if(!crew) return RemoveObject();

  selection = GetMenuSelection(crew); 

  SpawnMenu();
}

protected func OnMenuSelection(int iSelection)
{
  var crew;
  if(spawn || !(crew = Contents())) return;
  selection = iSelection;
}

protected func MenuQueryCancel()
{
  if(spawn)
    return false;
  else
    return true;
}

/* Spawntimer */

public func FxSpawntimerStart(pTarget, iNo, iTemp, iPlr, pClonk, cont)
{
  if(iTemp)
    return -1;
  if(iPlr < 0)
    return -1;

  //EffectVars
  EffectVar(0, pTarget, iNo) = iPlr;	//Spieler
  EffectVar(1, pTarget, iNo) = 10;	//Zeit
  EffectVar(2, pTarget, iNo) = pClonk;	//Clonk
  EffectVar(3, pTarget, iNo) = cont;	//Container

  CustomMessage(Format("@$TimeTillRespawn$", EffectVar(1, pTarget, iNo)), FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr, 0, 70, 0, 0, 0, MSG_NoLinebreak);
}

public func FxSpawntimerTimer(pTarget, iNo, iTime)
{
  EffectVar(1, pTarget, iNo)--;
  var iPlr = EffectVar(0, pTarget, iNo);
  CustomMessage(Format("@$TimeTillRespawn$", EffectVar(1, pTarget, iNo)), FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr, 0, 70, 0, 0, 0, MSG_NoLinebreak);

  //Verschwinden wenn Clonk/Behälter weg oder Clonk nicht im Behälter
  if (!EffectVar(2, pTarget, iNo) || !EffectVar(3, pTarget, iNo) || Contained(EffectVar(2, pTarget, iNo)) != EffectVar(3, pTarget, iNo))
  {
    //Verschwinden wenn Behälter noch vorhanden und TIM1
    if(GetID(EffectVar(3, pTarget, iNo)) == TIM1)
      RemoveObject(EffectVar(3, pTarget, iNo));
    return -1;
  }

  if(EffectVar(1, pTarget, iNo) <= 0)
  {
    CustomMessage("", FindObject2(Find_ID(1HUD), Find_Owner(iPlr)), iPlr);
    SetupVehicle(iPlr, APCE);
    return -1;
  }
}

public func FxSpawntimerStop(pTarget, iNo, iReason, fTemp)
{
  if(!fTemp)
    PlayerMessage(EffectVar(0, pTarget, iNo), "@");
}