/*-- Frontlines (größtenteils Besitznahme --*/

#strict 3
#include TEAM

local iStartTickets;
local iWarningTickets;
local aTicket;
local aTeamTimers;
local aFlagGroups; // array of groups (group: array of flags)
local aFlags; // same information, for performance. Only used for reading

static const GOCC_Horizontal		= 1;
static const GOCC_Vertical		= 2;

private func StartTickets()		{return 25;}		//Standardticketzahl
private func TicketsPerFlag()		{return 5;}
public func IsConfigurable()		{return true;}
public func GoalExtraValue()					//Spielzielinformationen an Scoreboard weitergeben
{
  return iStartTickets;
}
public func CustomSpawnSystem()		{return true;}
public func RejectChoosedClassInfo()	{return true;}
public func GoalDescription()
{
  return "$GoalDesc2$";
}


/* Initialisierung */

protected func Initialize()
{
  aTicket = [];
  //Ticketzahl vorgeben
  iStartTickets = StartTickets();

  return _inherited();
}

public func Activate(iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
}


// must be called by the scenario script once all flags have been created
public func SetFlagGroups(flagGroups)
{
  aFlags = [];
  aFlagGroups = flagGroups;
  for(var group in aFlagGroups)
  {
    for(var flag in group)
    {
      flag->InitCaptureableArray();
      // set aFlags (flattened flagGroups
      aFlags[] = flag;
    }
  }

  // "Capture" start flags
  for(var group in aFlagGroups)
  {
    for(var flag in group)
    {
      var startteam = flag->GetFrontlinesTeam();
      if(startteam != nil)
        FlagFrontlinesStatusChange(flag, startteam, true);
    }
  }


}

// helper function for simple maps where all flags are in a line
// (e.g. Island Strike, Oasis, Atlantic Crisis)
public func LinearScenario(flags)
{
  var flagGroups = [];
  for(var flag in flags)
    flagGroups[] = [flag];
  return flagGroups;
}


/* Rundenbeginn */

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);

  //Ticketalarm an Ticketzahl anpassen
  if(iStartTickets < 4)
    iWarningTickets = 0;
  else
    iWarningTickets = Max(iStartTickets/4, 5);

  //Bei Klassenwahl Spawnsystem anpassen
  if(!FindObject(MCSL))
    ScheduleCall(0,"CreateSpawners",1);

  //Tickets verteilen
  for(var i = 0; i < GetTeamCount(); i++)
    DoTickets(GetTeamByIndex(i), iStartTickets);
}

public func CreateSpawners()
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateGOCCSpawner(GetCrew(GetPlayerByIndex(i)));
  }
}

/* Globale Funktionen */

public func GetDirection()
{
  var call = GameCall("OccupationDir");
  if(call) return call;
  return GOCC_Horizontal;
}

// only exists because of the flag chooser
// removed the automatic sorting by position via FindObjects because
// it could change the order if the flags move (unlikely, but possible)
// and because the flags are sorted anyway because the information is extracted from aFlagGroups
// todo: remove this and use an array in OCC as well?
public func GetSortedFlags()
{
  return GetFlags();
}

public func GetFlags()
{
  return aFlags;
}

/* HUD */

public func GetHUDInfo(int player, object hud)
{
  var str;

  if(GetTeamCount() > 0)
  {
    str = Format("<c %x>%d</c>",GetTeamColor(1),GetTickets(1));

    if(GetTeamCount() > 1)
      for(var i = 2; i <= GetTeamCount(); i++)
        Format("%s : <c %x>%d</c>",GetTeamName(i),GetTeamColor(i),GetTickets(i));
  }

  return str;
}

/* Konfiguration */

local iChoosedPlr;

public func ConfigMenu(object pCaller)
{
  iChoosedPlr = GetOwner(pCaller);
  OpenGoalMenu();
  return 1;
}

private func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

/* Hostmenü */

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,nil,0,"",0,1);

  //Ticketanzeige
  AddMenuItem("$Tickets$", "", SM03, pClonk, iStartTickets);

  //Ticketlimits ermitteln
  var uplimit = 100; var downlimit = 5;
  if(GetLeague())
  {
    uplimit = 40; downlimit = 10;
  }

  //Mehr Tickets
  if(iStartTickets < uplimit)
    AddMenuItem("$MoreTickets$", "ChangeStartTickets", GetID(), pClonk, 0, +5, "$MoreTickets$",2,1); else
    AddMenuItem("<c 777777>$MoreTickets$</c>", "ChangeStartTickets", GetID(), pClonk, 0, +5, "$MoreTickets$",2,1);

  //Weniger Tickets
  if(iStartTickets > downlimit)
    AddMenuItem("$LessTickets$", "ChangeStartTickets", GetID(), pClonk, 0, -5, "$LessTickets$",2,2); else
    AddMenuItem("<c 777777>$LessTickets$</c>", "ChangeStartTickets", GetID(), pClonk, 0, -5, "$LessTickets$",2,2);

  //Fertig
  AddMenuItem("$Finished$", "ConfigFinished", GetID(), pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeStartTickets(id dummy, int iChange)
{
  //Sound
  Sound("Grab",1,nil,0,1);

  //Ticketstand verändern
  if(!GetLeague())
    iStartTickets = BoundBy(iStartTickets+iChange,5,100);
  else
    iStartTickets = BoundBy(iStartTickets+iChange,10,40);

  //Menü erneut öffnen
  var iSel = 1;
  if(iChange < 0) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Scoreboard */

static const GOCC_FlagColumn		= 1;
static const GOCC_TimerColumn		= 2;
static const GOCC_ProgressColumn	= 3;

protected func InitScoreboard()
{  
  //Wird noch eingestellt
  if(FindObject(CHOS) || IsFulfilled()) return;

  UpdateHUDs();

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%s",GetName()), SBRD_Caption);

  //Spaltentitel
  SetScoreboardData(SBRD_Caption, GOCC_FlagColumn, "{{IC12}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_ProgressColumn, "{{SM02}}", SBRD_Caption);

  UpdateScoreboard();

  init = true;
}

private func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS) || IsFulfilled()) return;

  var i = 0;
  var data, base;

  //Szenarioausrichtung ermitteln
  if(GetDirection() == GOCC_Horizontal)
    base = LandscapeWidth();
  if(GetDirection() == GOCC_Vertical)
    base = LandscapeHeight();

  //Flaggenposten auflisten
  for(var flag in GetFlags())
  {
    //Teamfarbe und Flaggenzustand ermitteln
    var teamclr = GetTeamColor(flag->GetTeam()),
    prog = flag->GetProcess();
    //Färbung je nach Zustand
    if(!flag->~IsFullyCaptured())
      var nameclr = RGB(255,255,255);
    else
      var nameclr = teamclr;
    var percentclr = RGBa(Interpolate2(255, GetRGBaValue(teamclr, 1), prog, 100),
    Interpolate2(255, GetRGBaValue(teamclr, 2), prog, 100), 
    Interpolate2(255, GetRGBaValue(teamclr, 3), prog, 100));

    //Entsprechend der Ausrichtung des Szenarios sortieren
    if(GetDirection() == GOCC_Horizontal)
      data = GetX(flag);
    if(GetDirection() == GOCC_Vertical)
      data = GetY(flag);

    SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", nameclr, GetName(flag)), data);
    SetScoreboardData(i, GOCC_ProgressColumn, Format("<c %x>%d%</c>", percentclr, flag->GetProcess()));
    i++;
  }

  //Leerzeile und Spaltentitel
  if(i != 1)
  {
    SetScoreboardData(i, GOCC_FlagColumn, " ", base+1);
    SetScoreboardData(i, GOCC_ProgressColumn, " ");
    i++;
    SetScoreboardData(i, GOCC_FlagColumn, "{{SM26}}", base+2);
    SetScoreboardData(i, GOCC_ProgressColumn, "{{SM03}}");
    i++;
  }

  //Teams und deren Timer und Tickets auflisten
  for(var j = 0; j < GetTeamCount(); j++)
  {
    var iTeam = GetTeamByIndex(j);
    if(TeamAlive(iTeam))
    {
      SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", GetTeamColor(iTeam), GetTeamName(iTeam)), base+3+iStartTickets-GetTickets(iTeam));
      SetScoreboardData(i, GOCC_ProgressColumn, Format("<c ffbb00>%d</c>", GetTickets(iTeam)));
    }
    else
    {
      SetScoreboardData(i, GOCC_FlagColumn, "");
      SetScoreboardData(i, GOCC_ProgressColumn, "");
    }
    i++;
  }

  SortScoreboard(GOCC_FlagColumn);
}

/* GameCalls */

public func FlagAttacked(object pFlag, int iTeam)
{
  UpdateScoreboard();
}

public func FlagLost(object pFlag, int iTeam, int iTeamAttacker, array pAttackers, bool gavetickets)
{
  //Punkte bei Belohnungssystem
  var i = 0;
  for(var pClonk in pAttackers)
  {
    if(!i)
    {
      //Punkte bei Belohnungssystem (Flaggenposten neutralisiert)
      DoPlayerPoints(BonusPoints("OPNeutralization"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC13);
      //Geldbonus: 10 Clunker
      DoWealth(GetOwner(pClonk), 10);
    }
    else
    {
      //Punkte bei Belohnungssystem (Hilfe bei Flaggenpostenneutralisierung)
      DoPlayerPoints(BonusPoints("OPNeutralizationAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC20);
      //Geldbonus: 5 Clunker
      DoWealth(GetOwner(pClonk), 5);
    }
    i++;
  }
  if(iTeam)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      {
        //Eventnachricht: Flaggenposten verloren
        EventInfo4K(GetPlayerByIndex(i)+1, Format("$MsgFlagLost$", GetName(pFlag), GetTeamColor(iTeamAttacker), GetTeamName(iTeamAttacker)), IC13, 0, GetTeamColor(iTeamAttacker), 0, "Info_Event.ogg");
      }
    }
  }
  // subtract tickets
  if(gavetickets)
    DoTickets(iTeam, -TicketsPerFlag());
}

public func FlagCaptured(object pFlag, int iTeam, array pAttackers, bool fRegained, bool gavetickets)
{
  //Punkte bei Belohnungssystem
  if(fRegained)
  {
    for(var pClonk in pAttackers)
    {
      //Punkte bei Belohnungssystem (Flaggenposten verteidigt)
      DoPlayerPoints(BonusPoints("OPDefense"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC12);
      //Geldbonus: 5 Clunker
      DoWealth(GetOwner(pClonk), 5);
    }
  }
  else
  {
    var i = 0;
    for(var pClonk in pAttackers)
    {
      if(!i)
      {
        //Punkte bei Belohnungssystem (Flaggenposten erobert)
        DoPlayerPoints(BonusPoints("OPConquest"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC10);
        //Achievement-Fortschritt (Flagship)
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        //Geldbonus: 15 Clunker
        DoWealth(GetOwner(pClonk), 15);
      }
      else
      {
        //Punkte bei Belohnungssystem (Hilfe bei Flaggenposteneroberung)
        DoPlayerPoints(BonusPoints("OPConquestAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC11);
        //Achievement-Fortschritt (Flagship)
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        //Geldbonus: 10 Clunker
        DoWealth(GetOwner(pClonk), 10);
      }
      i++;
    }

  }

  // add tickets
  if(!gavetickets)
    DoTickets(iTeam, TicketsPerFlag());

  //Eventnachricht: Flaggenposten erobert
  EventInfo4K(0, Format("$MsgFlagCaptured$", GetTeamColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), IC10, 0, GetTeamColor(iTeam), 0, "Info_Objective.ogg");
  UpdateScoreboard();
}


private func FlagFrontlinesStatusChange(object flag, int teamnumber, bool captured) {
  // group states
  var notCaptured = 0;
  var partiallyCaptured = 1;
  var fullyCaptured = 2;

  var group = GetGroup(flag);
  var nCaptured = CountFullyCapturedFlags(group, teamnumber);
  var nFlags = GetLength(group);

  // determine old and new state - I'm not sure if all branches are
  // necessary but this makes the code below easier to read
  var oldState = notCaptured;
  var state = notCaptured;
  if (nCaptured == 0) {
    state = notCaptured;
    if (nFlags == 1)
      oldState = fullyCaptured;
    else
      oldState = partiallyCaptured;
  } else if (nCaptured == nFlags) {
    state = fullyCaptured;
    if (nFlags == 1)
      oldState = notCaptured;
    else
      oldState = partiallyCaptured;
  } else {
    state = partiallyCaptured;
    if (captured && nCaptured == 1)
      oldState == notCaptured;
    else if (!captured && nFlags - 1 == nCaptured)
      oldState == fullyCaptured;
    else
      return; // no state change
  }

  // state != oldState at this point

  // update capturabeby arrays of the flags in the group and the neighboring flags
  if (state == fullyCaptured) {
    for (var neighbor in GetNeighbors(group))
      SetGroupCaptureableBy(neighbor, teamnumber, true);
  } else {
    if (state == partiallyCaptured)
      SetGroupCaptureableBy(group, teamnumber, true);
    else
      SetGroupCaptureableBy(group, teamnumber, AnyNeighborFullyCaptured(group, teamnumber));

    if (oldState == fullyCaptured)
      for (var neighbor in GetNeighbors(group))
        if (!AnyNeighborFullyCaptured(neighbor, teamnumber))
          SetGroupCaptureableBy(neighbor, teamnumber, false);
  }
}

private func GetGroup(object flag)
{
  // todo: use map for performance?
  for(var flagGroup in aFlagGroups) {
    if (ArrayContains(flagGroup, flag))
      return flagGroup;
  }
  Log("misconfiguration: flag %v was not assigned to a flag group", flag);
}

private func CountFullyCapturedFlags(array flagGroup, int teamnumber) {
  var n = 0;
  for(var flag in flagGroup)
    n += flag->IsFrontlinesFullyCapturedBy(teamnumber);
  return n;
}

private func IsFullyCaptured(array flagGroup, int teamnumber) {
  for (var flag in flagGroup)
    if (!flag->IsFrontlinesFullyCapturedBy(teamnumber))
      return false;
  return true;
}

private func AnyNeighborFullyCaptured(array flagGroup, int teamnumber) {
  for (var neighborGroup in GetNeighbors(flagGroup)) {
    if (IsFullyCaptured(neighborGroup, teamnumber)) {
      return true;
    }
  }
  return false;
}

private func GetNeighbors(array flagGroup) {
  // todo: use map for performance?
  var neighbors = [];
  var index = GetIndexOf(flagGroup, aFlagGroups);
  if (index > 0)
    neighbors[] = aFlagGroups[index - 1];
  if (index + 1 < GetLength(aFlagGroups))
    neighbors[] = aFlagGroups[index + 1];
  return neighbors;
}

private func SetGroupCaptureableBy(array flagGroup, int teamnumber, bool captureable) {
  for(var flag in flagGroup)
    flag->SetCaptureableBy(teamnumber, captureable);
}


/* Tickets */
public func GetTeamTimer(int iTeam)
{
  if(!aTeamTimers)
    return 0;

  return (aTeamTimers[iTeam-1] / 100);
}

public func GetTickets(int iTeam)
{
  return aTicket[iTeam-1] || 0;
}

public func SetTickets(int iTeam, int iTickets)
{
  aTicket[iTeam-1] = Max(iTickets, 0);
  UpdateScoreboard();
  return true;
}

public func DoTickets(int iTeam, int iChange, bool fNoWarn)
{
  SetTickets(iTeam, GetTickets(iTeam) + iChange);
  if(!fNoWarn)
  {
    if(iWarningTickets != 0 && iWarningTickets == aTicket[iTeam-1])
    {
      Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d)", aTicket[iTeam-1], iTeam), 1);
    }
  }
  return true;
}

/* Eventnachrichten */

public func TicketsLow(int iRemaining, int iTeam)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      //Eventnachricht: Warnung vor niedrigen Tickets
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgTicketsLow$",iRemaining),SM03,0,0,0,"Info_Event.ogg");
    }
  }
  return true;
}

/* Rundenauswertung */

local fFulfilled;

public func IsFulfilled()
{
  if(FindObject(CHOS))	return false;
  if(fFulfilled)	return true;

  var iWinningTeam = GetWinningTeam();

  //Existiert nurnoch ein Team, gewinnt es
  if(iWinningTeam != nil || !GetFlags())
  {
    //Teameliminierung
    for(var i = 0; i < GetTeamCount(); i++)
      if(GetTeamByIndex(i) != iWinningTeam) EliminateTeam(GetTeamByIndex(i));

    // ???
    if(LosersAlive(iWinningTeam)) return false;

    //Spielende planen
    Schedule("GameOver()",150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("@$TeamHasWon$", nil, GetTeamColor(iWinningTeam), GetTeamName(iWinningTeam));

    //Sounds
    Sound("Cheer.ogg", true);

    return fFulfilled = true;
  }
}

private func TeamAlive(int teamnumber)
{
  // game hasn't started yet
  if(FindObject(CHOS))
    return true;
  return (GetTeamPlayerCount(teamnumber) || 0) > 0 && aTicket[teamnumber-1] > 0;
}

private func GetTeamWithAllFlags()
{
  var flags = GetFlags();
  var first = flags[0]->GetFrontlinesTeam();
  for(var flag in flags)
    if(flag->GetFrontlinesTeam() != first)
      return nil;
  return first;
}

private func GetOnlyTeamAlive()
{
  var winningTeam = nil;
  for(var i = 0; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i);
    if(TeamAlive(team))
    {
      if(winningTeam == nil)
        winningTeam = team;
      else // there are at least 2 teams still alive
        return nil;
    }
  }
  return winningTeam;
}

private func GetWinningTeam()
{
  // win condition 1: a team has all flags
  var winningTeam = GetTeamWithAllFlags();
  if(winningTeam != nil)
    return winningTeam;

  // win condition 2: only one team left
  winningTeam = GetOnlyTeamAlive();
  if(winningTeam != nil)
    return winningTeam;
}

private func EliminateTeam(int iTeam)
{
  for(var i = 0; i < GetPlayerCount() ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      EliminatePlayer(GetPlayerByIndex(i));
    }
}

private func LosersAlive(int iTeam)
{
  for(var i = 0; i < GetPlayerCount() ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) != iTeam)
      return true;

  return false;
}

/* Alarmleuchtensteuerung */

public func FxIntWarnStart(object pTarget, int iEffectNumber, int iTemp, object pPoint)
{
  EffectVar(0, pTarget, iEffectNumber) = pPoint;
  return 1;
}

public func FxIntWarnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget)
   return -1;

  var pPoint = EffectVar(0,pTarget,iEffectNumber);
  var iLast  = EffectVar(1,pTarget,iEffectNumber);
  var iNow;
  if(pPoint->GetTrend() || pPoint->GetAttacker())
   iNow = true;
  
  if(iNow != iLast)
  {
     if((pPoint->GetTrend() >= 0) || !pPoint->GetAttacker())
      pTarget->TurnOff();
    else
      pTarget->TurnOn();

    EffectVar(1, pTarget, iEffectNumber) = iNow;
  }

  return;
}

global func AddGOCCWarnEffect(object pTarget, object pPoint, int iRotation)
{
  AddEffect("IntWarn", pTarget, 1, 35, 0, GOCC, pPoint);
  if(iRotation)
    SetR(iRotation, pTarget);
  return true;
}

/* Respawn */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(!init) return;

  if(IsFulfilled()) return EliminatePlayer(iPlr);

  //Verfeindung setzen
  Hostility(iPlr);
  
  RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam, true);
}

protected func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch)
{
  if(iPlr == -1 || !GetPlayerName(iPlr)) return;

  //Schauen wir mal ob noch geht
  IsFulfilled();

  //Kein Team?
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);

  //Kills
  aDeath[iPlr]++;
  //Tode
  if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
  {
    aKill[iMurdererPlr]++;
  }

  //Kopfgeld
  Money(iPlr, pCrew, iMurdererPlr);

  var winningTeam = GetWinningTeam();
  if(winningTeam != nil && winningTeam != iTeam)
  {
    if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
    return;
  }

  DoTickets(iTeam,-1);

  //Spieler wartet noch auf Respawn
  if(GameCall("GetPlayerRespawnTime", iPlr))
    return;

  if(!FindObject(CHOS) && !FindObject(MCSL)) //Regelwähler oder Klassenwahl?
    CreateGOCCSpawner(pCrew);

  //Flagge anfokussieren
  DoFlag(iTeam, iPlr);
}

public func OnClassSelection(object pClonk, int iClass)
{
  if(FindObject(CHOS))
    return;

  CreateGOCCSpawner(pClonk, iClass);
}

public func DoFlag(int iTeam, int iPlr)
{
  var pCrew = GetCrew(iPlr);
  if(!pCrew) return Schedule(Format("DoFlag(%d, %d)", iTeam, iPlr), 1);
  var pObject = Contained(pCrew);

  if(!ShowFlagpole(GetBestFlag(iTeam), pCrew, pObject))
  {
    SetPlrViewRange(0, pCrew);
  }

  return true;
}

private func RemovePlayer(int iPlr)
{
  if(iPlr == -1) return;

  //Auswertungsdialog
  DoEvaluation(iPlr);

  UpdateHUDs();
  aDeath[iPlr] = 0;
  aKill[iPlr] = 0;
}

/* Ungenutzte Funktionen */

private func InitMultiplayerTeam(int iTeam)	{}
private func RemoveMultiplayerTeam(int iTeam)	{}
private func InitSingleplayerTeam(int iPlr)	{}
private func RemoveSingleplayerTeam(int iPlr)	{}
private func InitPlayer(int iPlr)		{}
private func RemoveScoreboardPlayer(int iPlr)	{}
public func WinScoreChange(int iNewScore)	{}
private func SortTeamScoreboard()		{}