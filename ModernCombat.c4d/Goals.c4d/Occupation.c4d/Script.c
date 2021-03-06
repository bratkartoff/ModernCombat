/*-- Besitznahme --*/

#strict 2
#include TEAM

local iStartTickets;
local iWarningTickets;
local aTicket;
local aTeamTimers;
local fTicketBleed;

static const GOCC_Horizontal		= 1;
static const GOCC_Vertical		= 2;

private func StartTickets()		{return 30;}		//Standardticketzahl
public func IsConfigurable()		{return true;}
public func GoalExtraValue()					//Spielzielinformationen an Scoreboard weitergeben
{
  if(fTicketBleed)
    return Format("%d, {{SM27}}", iStartTickets);
  else
    return iStartTickets;
}
public func CustomSpawnSystem()		{return true;}
public func RejectChoosedClassInfo()	{return true;}
global func GetOccupationTimerSpeed()	{return 30;}		//Standard-Timer-Geschwindigkeit
public func GoalDescription()
{
  if(fTicketBleed)
    return "$GoalDesc$";
  else
    return "$GoalDesc2$";
}


/* Initialisierung */

protected func Initialize()
{
  aTicket = [];
  //Ticketzahl vorgeben
  iStartTickets = StartTickets();
  fTicketBleed = true;

  return _inherited();
}

public func Activate(iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
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

  if(fTicketBleed)
    Schedule("AddEffect(\"OccupationGame\", this, 100, 1, this, GOCC);", 1, 0, this);
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

public func GetSortedFlags()
{
  if(GetDirection() == GOCC_Horizontal)
    return FindObjects(Find_Func("IsFlagpole"), Sort_Func("Sort_XPosition"));
  else
    return FindObjects(Find_Func("IsFlagpole"), Sort_Func("Sort_YPosition"));
}

global func GetFlags()
{
  return FindObjects(Find_Func("IsFlagpole"));
}

global func CreateFlagpole(int iX, int iY, string szName, int iRange, int iSpeed)
{
  var point = CreateObject(OFPL, iX, iY, NO_OWNER);
  point->Set(szName, iRange, iSpeed);
  return point;
}

global func GetTeamFlags(int iTeam)
{
  var result = [];

  for(flag in GetFlags())
    if(flag->GetTeam() == iTeam)
      result[GetLength(result)] = flag;

  return result;
}

global func GetFlagCount(int iTeam, bool bCountBlankFlags, bool bCountOnlyFullyCaptured)
{
  var count = 0;
  for(var flag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(iTeam)
    {
      if(bCountBlankFlags)
      {
        if(flag->GetTeam() || !flag->~IsFullyCaptured())
          if(flag->GetTeam() != iTeam)
            continue;
      }
      else
      {
        if(flag->GetTeam() != iTeam)
          continue;

        if(bCountOnlyFullyCaptured && !flag->~IsFullyCaptured())
          continue;
      }
    }
    count++;
  }
  return count;
}

global func GetEnemyFlagCount(int iTeam)
{
  var count = 0;

  if(!iTeam)
    return 0;

  for(var flag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(HostileTeam(iTeam, flag->GetTeam()) && flag->~IsFullyCaptured())
      count++;
  }

  return count;
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

/* Hostmen? */

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  //Ticketanzeige
  AddMenuItem("$Tickets$", 0, SM03, pClonk, iStartTickets);

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

  //Ticketabzug umschalten
  if(fTicketBleed)
    AddMenuItem("$TicketBleed$", "ChangeTicketBleed", SM27, pClonk, 0, false, "$TicketBleedDesc$");
  else
    AddMenuItem("<c 777777>$TicketBleed$</c>", "ChangeTicketBleed", SM06, pClonk, 0, true, "$TicketBleedDesc$");

  //Fertig
  AddMenuItem("$Finished$", "ConfigFinished", GetID(), pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeStartTickets(id dummy, int iChange)
{
  //Sound
  Sound("Grab",1,0,0,1);

  //Ticketstand ver?ndern
  if(!GetLeague())
    iStartTickets = BoundBy(iStartTickets+iChange,5,100);
  else
    iStartTickets = BoundBy(iStartTickets+iChange,10,40);

  //Men? erneut ?ffnen
  var iSel = 1;
  if(iChange < 0) iSel = 2;
  OpenGoalMenu(0, iSel);
}

private func ChangeTicketBleed(id dummy, bool fChange)
{
  fTicketBleed = fChange;
  GoalMenuChangeVariable();
  //Men? erneut ?ffnen
  OpenGoalMenu(0, 3);
}




// Hilfsfunktion
private func GoalMenuChangeVariable() {
  //Sound
  Sound("Grab",1,0,0,1);

  //Zeitlimit umschalten und Spielzielbeschreibung aktualisieren
  var chos = FindObject(CHOS);
  if(chos) chos->SetGoalDesc(GetID(this), GoalDescription());
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
  if(fTicketBleed) SetScoreboardData(SBRD_Caption, GOCC_TimerColumn, " ", SBRD_Caption);
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
    //F?rbung je nach Zustand
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
    if(fTicketBleed) SetScoreboardData(i, GOCC_TimerColumn, Format(" "));
    SetScoreboardData(i, GOCC_ProgressColumn, Format("<c %x>%d%</c>", percentclr, flag->GetProcess()));
    i++;
  }

  //Leerzeile und Spaltentitel
  if(i != 1)
  {
    SetScoreboardData(i, GOCC_FlagColumn, " ", base+1);
    if(fTicketBleed) SetScoreboardData(i, GOCC_TimerColumn, " ");
    SetScoreboardData(i, GOCC_ProgressColumn, " ");
    i++;
    SetScoreboardData(i, GOCC_FlagColumn, "{{SM26}}", base+2);
    if(fTicketBleed) SetScoreboardData(i, GOCC_TimerColumn, "{{SM27}}");
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
      if(fTicketBleed) SetScoreboardData(i, GOCC_TimerColumn, Format("<c 777777>%d</c>", GetTeamTimer(iTeam)));
      SetScoreboardData(i, GOCC_ProgressColumn, Format("<c ffbb00>%d</c>", GetTickets(iTeam)));
    }
    else
    {
      SetScoreboardData(i, GOCC_FlagColumn, 0);
      if(fTicketBleed) SetScoreboardData(i, GOCC_TimerColumn, 0);
      SetScoreboardData(i, GOCC_ProgressColumn, 0);
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

public func FlagLost(object pFlag, int iTeam, int iTeamAttacker, array pAttackers)
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
  UpdateScoreboard();
}

public func FlagCaptured(object pFlag, int iTeam, array pAttackers, bool fRegained)
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
  //Eventnachricht: Flaggenposten erobert
  EventInfo4K(0, Format("$MsgFlagCaptured$", GetTeamColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), IC10, 0, GetTeamColor(iTeam), 0, "Info_Objective.ogg");
  UpdateScoreboard();
}

/* Tickets */

public func TicketChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  UpdateScoreboard(iTeam);
}

public func GetTeamTimer(int iTeam)
{
  if(!aTeamTimers)
    return;

  return (aTeamTimers[iTeam-1] / 100);
}

public func GetHighestTeams()
{
  var result = [];
  var flagcounts = [];

  for(var i = 0; i < GetTeamCount(); i++)
    flagcounts[i] = GetFlagCount(i+1, false, true);

  var highest = GetMaxArrayVal(flagcounts, false, true);

  for(var i = 0; i < GetLength(flagcounts); i++)
    if(flagcounts[i] == highest)
      result[GetLength(result)] = i + 1;

  return result;
}

public func GetTickets(int iTeam)
{
  return aTicket[iTeam-1];
}

public func SetTickets(int iTeam, int iTickets)
{
  aTicket[iTeam-1] = Max(iTickets, 0);
  UpdateScoreboard(iTeam);
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
  if(FindObject(CHOS))	return;
  if(fFulfilled)	return true;

  var iWinningTeam = GetWinningTeam();

  //Existiert nurnoch ein Team, gewinnt es
  if(iWinningTeam > 0 || !GetFlags())
  {
    //Teameliminierung
    for(var i = 0; i < GetTeamCount(); i++)
      if(GetTeamByIndex(i) != iWinningTeam) EliminateTeam(GetTeamByIndex(i));

    if(LosersAlive(iWinningTeam)) return;

    //Spielende planen
    Schedule("GameOver()",150);

    //Auswertung
    RewardEvaluation();

    //Nachricht ?ber Gewinner
    Message("@$TeamHasWon$",0 , GetTeamColor(iWinningTeam), GetTeamName(iWinningTeam));

    //Sounds
    Sound("Cheer.ogg", true);

    return fFulfilled = true;
  }

  //Unentschieden
  if(iWinningTeam == -1)
  {
    for(var i = 0; i < GetTeamCount(); i++)
      EliminateTeam(GetTeamByIndex(i));
    if(LosersAlive(0))
      return false;

    //Spielende planen
    Schedule("GameOver()",150);

    //Auswertung
    RewardEvaluation();

    return fFulfilled = true;
  }
}

public func FxOccupationGameStart(object pTarget, int iEffectNumber)
{
  aTeamTimers = [];

  for(var i = 0; i < GetTeamCount(); i++)
    aTeamTimers[i] = 10000;

  return 1;
}

public func FxOccupationGameTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!GetFlags())
    return -1;

  var fcounts = [];

  for(var i = 0; i < GetTeamCount(); i++)
    fcounts[i] = GetFlagCount(i+1, false, true);

  var maxfcount = GetMaxArrayVal(fcounts, false, true);

  for(var i = 0; i < GetTeamCount(); i++)
  {
    if(fcounts[i] < maxfcount)
    {
      var decrease = fcounts[i] - GetEnemyFlagCount(i+1);
      if(decrease < 0)
      {
        decrease *= GetOccupationTimerSpeed();
        decrease = decrease / GetLength(GetFlags());
        aTeamTimers[i] += decrease;
        if(aTeamTimers[i] <= 0)
        {
          aTicket[i] -= 1;
          aTeamTimers[i] = 10000;
          UpdateScoreboard();
        }
      }
    }
  }

  return;
}

private func TeamAlive(int iTeam)
{
  var alive = [], poles = [];
  var i = iTeam;
  
  //Regelw?hler vorhanden: Teamanzahl zur?ckgeben
  if(FindObject(CHOS))
  {
    return GetTeamPlayerCount(iTeam);
  }
  //Ticketzahl 0: Team eliminieren
  if(aTicket[i-1] <= 0)
  {
    EliminateTeam(i);
    return false;
  }

  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  poles[i] = 0;
  for(var pole in FindObjects(Find_ID(OFPL)))
    if(pole->GetTeam() == i && pole->IsFullyCaptured())
      poles[i]++;
  //Keine Flaggen?
  if(poles[i] == 0)
  {
    alive[i] = 0;
    for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)))
      if(GetPlayerTeam(GetOwner(clonk)) == i)
      {
        if(Contained(clonk))
        {
          if(IsFakeDeath(clonk) || (GetID(Contained(clonk)) == OSPW && GetAction(Contained(clonk)) != "Counter") || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
            continue;
          alive[i]++;
          break;
        }
        alive[i]++;
      }
  }
  else
    //Keine Spieler in einem Team?
    for(var j = 0; j < GetPlayerCount(); j++)
      if(GetPlayerTeam(GetPlayerByIndex(j)) == i)
        alive[i]++;
  if(alive[i] > 0) return true;
  return false;
}

private func GetWinningTeam()
{
  var alive = [];

  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  for(var i = 0; i < GetTeamCount(); i++)
  {
    alive[GetTeamByIndex(i)] = TeamAlive(GetTeamByIndex(i));
  }

  //Wie viele Teams existent?
  var teamA = 0;
  for(var i = 0; i < GetLength(alive); i++)
    if(alive[i])
    {
      if(teamA) //Zwei oder mehr Teams lebendig?
        return 0;
      else
        teamA = i; //Ein lebendiges Team gefunden?
    }
  if(teamA < 1) //Falls keine Teams lebendig (wieso auch immer)
    return 0;
  else
    return teamA; //Ansonsten das einzig lebendige Team
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

  if(GetWinningTeam() > 0 && GetWinningTeam() != iTeam)
  {
    if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
    return;
  }

  DoTickets(iTeam,-1);

  //Spieler wartet noch auf Respawn
  if(GameCall("GetPlayerRespawnTime", iPlr))
    return;

  if(!FindObject(CHOS) && !FindObject(MCSL)) //Regelw?hler oder Klassenwahl?
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

public func GetSpawnPoint(object selectedFlag, int &iX, int &iY, string &szFunction, object clonk)
{
  szFunction = GetBestSpawnpoint(selectedFlag["spawnpoints"], GetOwner(clonk), iX, iY, clonk)[2];
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