/*-- Flaggenposten --*/

#strict 3

local team, process, range, flag, bar, attacker, spawnpoints, trend, capt, pAttackers, lastowner, currentIcon, captureradiusmarker, noenemys, nofriends;
local startflagforteam;
local capturableby; // array of booleans (index: team index)

public func GetAttacker()		{return attacker;}
public func GetTeam()			{return team;}
public func GetProcess()		{return process;}
public func GetTrend()			{return trend;}
public func GetRange()			{return range;}
public func IsFullyCaptured()		{return capt;}

public func StandardRange()		{return 100;}		//Standardreichweite
public func StandardSpeed()		{return 2;}		//Standardeinnahmegeschwindigkeit

public func IsFlagpole()		{return true;}		//Ist ein Flaggenposten
public func IsSpawnable()		{return true;}		//Einstiegspunkt

static const BAR_FlagBar = 5;
static const COLOR_WHITE = 16777215; // RGB(255, 255, 255);

static const OFPL_ICON_NORMAL     = SM21;
static const OFPL_ICON_CONQUERING = SM22;
static const OFPL_ICON_EMBATTLED  = SM23;
static const OFPL_ICON_BACKLINE   = SM31; // frontlines only

/* Initalisierung */

public func Initialize()
{
  capt = false;
  spawnpoints = CreateArray();
  pAttackers = CreateArray();

  //Anfangs neutral
  lastowner = 0;
  //Standardwerte setzen
  Set();
  //Flagge erstellen
  if(!flag)
    flag = CreateObject(OFLG);
  //HUD-Anzeige erstmals einrichten
  process = 0;
  UpdateFlag();
}

/* Einstellungen */

public func Set(string szName, int iRange, int iSpeed, int iValue)
{
  //Name setzen
  if(!szName) szName = "Alpha";
  SetName(szName);

  //Reichweite setzen
  if(!iRange) iRange = StandardRange();
  range = iRange;

  //Einnahmegeschwindigkeit setzen
  if(!iSpeed) iSpeed = StandardSpeed();

  //Prüfungseffekt einrichten
  RemoveEffect("IntFlagpole",this);
  AddEffect("IntFlagpole",this,10,iSpeed,this);
}

public func IsSpawnableForTeam(int teamnumber)
{
  return team == teamnumber && IsFullyCaptured();
}

/* Frontlines */
private func IsFrontlines() { return capturableby != nil; }

public func SetStartFlagForTeam(int teamnumber)
{
  startflagforteam = teamnumber;
  Capture(teamnumber, true);
}

public func IsStartFlagForTeam(int teamnumber)
{
  return startflagforteam == teamnumber;
}

// todo: colors
public func InitCapturableArray()
{
  capturableby = CreateArray(GetTeamCount());
  UpdateFlag();
}

public func IsCapturableBy(int teamnumber)
{
  // OCC
  if (!IsFrontlines())
    return true;
  // start flags are always recapturable
  if (IsStartFlagForTeam(teamnumber))
    return true;
  // set by Frontlines goal object via SetCapturableBy
  return capturableby[teamnumber];
}

public func CountCapturableBy()
{
  if (!IsFrontlines())
    return GetTeamCount();
  var teams = 0;
  for(var i = 0; i < GetTeamCount(); i++)
    if(IsCapturableBy(GetTeamByIndex(i)))
      teams++;
  return teams;
}

public func IsBacklineFlag()
{
  var count = CountCapturableBy();
  return !count || count == 1 & IsFullyCaptured();
}

public func SetCapturableBy(int teamnumber, bool capturable)
{
  capturableby[teamnumber] = capturable;
  UpdateFlag();
}

public func IsFrontlinesFullyCaptured()
{
  return IsFullyCaptured();
  //return process == 100;
}

public func IsFrontlinesFullyCapturedBy(int teamnumber)
{
  return teamnumber == team && IsFrontlinesFullyCaptured();
}

// like GetTeam(), but returns the team iff IsFrontlinesFullyCapturedBy(team) == true
public func GetFrontlinesTeam()
{
  if(process == 100)
    return GetTeam();
  return nil;
}

private func GetFlagColor()
{
  return flag->GetColorDw();
}

private func Desaturate(int color)
{
  if (color == COLOR_WHITE) // reducing saturation doesn't work for white, reduce lightness instead
    return RGB(127,127,127);
  var hsl = RGB2HSL(GetFlagColor());
  return HSL2RGB(SetRGBaValue(hsl, GetRGBaValue(hsl, 2) / 2, 2));
}

public func GetNameColor()
{
  if(!IsFullyCaptured())
    return COLOR_WHITE;
  return GetFlagColor();
}

public func GetScoreboardPercentColor()
{
  // interpolating first and then desaturating doesn't work because Desaturate does not actually desature
  var color = GetFlagColor();
  var white = COLOR_WHITE;

  if(CountCapturableBy() < 2)
  {
    color = Desaturate(color);
    white = Desaturate(white);
  }
  
  return InterpolateRGBa3(white, color, process, 100);
}


/* Spawnpoint-Konfiguration */

public func AddSpawnPoint(int iX, int iY, string szFunction)
{
  spawnpoints[] = [iX, iY, szFunction];
}

public func ResetSpawnPoint()
{
  SetLength(spawnpoints, 0);
}

/* Flaggenzustände */

public func IsAttacked()
{
  for(clonk in FindObjects(Find_Distance(range),Find_OCF(OCF_Alive)))
  {
    if(Contained(clonk) && !Contained(clonk)->~IsHelicopter()) continue;
    if(GetOwner(clonk) == NO_OWNER) continue;
    var enemyTeam = GetPlayerTeam(GetOwner(clonk));
    if(enemyTeam != team && IsCapturableBy(enemyTeam))
      return true;
  }

  return false;
}

public func IsCaptured(bool pBool)
{
  capt = pBool;
}

protected func ResetAttackers()
{
  pAttackers = CreateArray();
}

/* Prüfungseffekt und -timer */

public func FxIntFlagpoleTimer(object pTarget)
{
  if(!pTarget)
    return(-1);
  pTarget->Timer();
  return;
}

/* Umkreis-Effekt */

protected func ShowCaptureRadius(object pTarget)
{
  //Kreis-Symbol erstellen
  var obj = CreateObject(SM09, 0, 0, -1);
  obj->Set(pTarget);

  //Symbolgröße anpassen
  var wdt = range * 2000 / GetDefWidth(SM09);

  //Symbol konfigurieren
  obj->SetObjDrawTransform(wdt, 0, 0, 0, wdt, 0);
  obj->SetGraphics("Big");

  return obj;
}

protected func Timer()
{
  var enemys,friends,opposition;

  //Momentanen Zustand speichern
  var iOld = trend;
  trend = 0;

  //Zuvor gespeicherte Clonks in Reichweite auf Aktualität prüfen
  var del;
  var clonks = FindObjects(Find_Distance(range),Find_OCF(OCF_Alive));
  for(var pClonk in pAttackers)
  {
    del = true;
    for(var clonk in clonks)
    {
      if(clonk == pClonk)
      {
        if(Contained(pClonk) && !Contained(pClonk)->~IsHelicopter()) continue;

        //Clonk vorhanden: Eintrag beibehalten
        del = false;
        break;
      }
    }
    //Clonk nicht vorhanden: Eintrag entfernen
    if(del)
      pAttackers[FindInArray4K(pAttackers, pClonk)] = 0;
  }

  //Leere Einträge entfernen
  CleanArray4K(pAttackers);

  var aFriends = CreateArray();
  var aEnemies = CreateArray();

  //Passende Clonks in Reichweite ermitteln
  var clonks = FindObjects(Find_Distance(range),Find_OCF(OCF_Alive));

  //Gefundene Clonks als Feinde oder Verbündete einstufen
  for(clonk in clonks)
  {
    if(Contained(clonk) && !Contained(clonk)->~IsHelicopter()) continue;
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(!GetPlayerName(GetOwner(clonk)) || !GetPlayerTeam(GetOwner(clonk))) continue;
    if(!PathFree4K(GetX(this()),GetY(this())-GetDefHeight(GetID())/2,GetX(clonk),GetY(clonk),4)) continue;
    if(GetPlayerTeam(GetOwner(clonk)) == team)
    {
      if(IsCapturableBy(team)) {
        friends++;
        aFriends[GetLength(aFriends)] = clonk;
      }
    }
    else
    {
      opposition = GetPlayerTeam(GetOwner(clonk));
      if(IsCapturableBy(opposition)) {
        enemys++;
        aEnemies[GetLength(aEnemies)] = clonk;
      }
    }
  }
  attacker = opposition;

  //Zustandsänderung ermitteln
  //Nur Feinde: Flaggenneutralisierung vorrantreiben
  if(enemys && !friends)
    DoProcess(opposition,Min(enemys,3));
  //Nur Verbündete: Flaggeneroberung vorrantreiben
  if(!enemys && friends)
    DoProcess(team,Min(friends,3));

  if(enemys)
  {
    if(!captureradiusmarker && noenemys)
    {
      captureradiusmarker = ShowCaptureRadius(this);
      noenemys = false;
    }
  }
  else
    noenemys = true;

  if(friends)
  {
    if(!captureradiusmarker && nofriends && process < 100)
      captureradiusmarker = ShowCaptureRadius(this);

    nofriends = false;
  }
  else
    nofriends = true;

  if((!enemys) == (!friends))
  {
    if(!friends)
    {
      if(SetIcon(NormalIcon()))
        bar->Update(0, true, true);
    }
    else
    {
      if(SetIcon(OFPL_ICON_EMBATTLED))
      {
        var clr = GetTeamColor(team), plr;
        if( (GetTeamConfig(TEAM_AutoGenerateTeams) && GetTeamPlayerCount(team) <= 1 && (plr = GetTeamMemberByIndex(team, 0)) > -1) || !GetTeamConfig(TEAM_TeamColors))
          clr = GetPlrColorDw(plr);

        bar->SetBarColor(clr);
        bar->Update(process);
      }
    }
  }

  if(trend != iOld)
    ResetAttackers();

  var pClonks = CreateArray();
  if(trend < 0)
    pClonks = aEnemies;
  if(trend > 0)
    pClonks = aFriends;

  for(var clonk in pClonks)
  {
    if(!clonk) continue;
    var new = true;
    //Clonk auffindbar?
    for(var pClonk in pAttackers)
    {
      if(pClonk == clonk) new = false;
      if(!new) break;
    }
    //Neu: Einstellen
    if(new) pAttackers[GetLength(pAttackers)] = clonk;
  }
}


public func Capture(int iTeam, bool bSilent)
{
  process = 100;
  attacker = 0;
  team = iTeam;
  capt = true;
  var fRegained = false;
  if(!bSilent)
  {
    if(lastowner == team) fRegained = true;
    GameCallEx("FlagCaptured", this, team, pAttackers, fRegained);
  }
  lastowner = team;
  ResetAttackers();
  UpdateFlag();
}

protected func Capturing(int iTeam)
{
  attacker = iTeam;
}

public func NoTeam()
{
  team = nil;
  process = 0;
  attacker = 0;
  capt = false;
  UpdateFlag();
}

/* Flaggenkonfiguration */

public func UpdateFlag()
{
  if(!flag) return;

  //Kein Statusbalken vorhanden: Erstellen
  if(!bar)
  {
    bar = CreateObject(SBAR, 0, 0, -1);
    bar->Set(this, RGB(255, 255, 255), BAR_FlagBar, 100, "", NormalIcon(), 0, 0, true, true);
    bar->ChangeDefOffset(GetDefOffset(GetID(), 1)+5);
    SetIcon(NormalIcon());
    bar->Update(0, true, true);
  }

  //Entsprechend dem Besitzer färben
  if(team)
  {
    SetColorDw(RGB(0,0,0), flag);
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) != team) continue;
      flag->SetOwner(GetPlayerByIndex(i));
      break;
    }
  }
  else
  {
    SetOwner(NO_OWNER, flag);
    flag->SetColorDw(RGB(255, 255, 255));
  }

  //Flaggenposition aktualisieren
  SetFlagPos(process);
}

protected func SetFlagPos(int iPercentage)
{
  if(!flag) return;

  var iMaximum = GetDefHeight() - GetDefHeight(GetID(flag));
  var iHeight = iPercentage * iMaximum / 100 + GetDefHeight(GetID(flag))/2;
  SetPosition(GetX(),GetY() - iHeight, flag);
}

/* Einnahme/Neutralisierung umsetzen */

public func DoProcess(int iTeam, int iAmount)
{
  var old = process;

  //Eventuelle Gegnerflagge abnehmen
  if(team)
  {
    if(iTeam != team && (process != 0))
      iAmount = -iAmount;
  }
  else
    team = iTeam;

  process = BoundBy(process+iAmount,0,100);

  if(old < process)
    trend = +1;

  if(old > process)
    trend = -1;

  //  if((old == 100 && trend < 0))

  if((old == 100 && trend < 0) || (old == 0 && trend > 0))
  {
    GameCallEx("FlagAttacked", this, team, pAttackers);
  }

  //Flagge wird übernommen
  if(process < 100 && trend != 0)
  {
    Capturing(iTeam);
  }

  //Flagge ist fertig übernommen
  if((process >= 100) && (old < 100))
  {
    Capture(iTeam);
    if(IsFrontlines())
      GameCallEx("FlagFrontlinesStatusChange", this, team, true);
  }

  //Neutrale Flagge
  if((process <= 0) && (old > 0))
  {
    capt = false;
    if(team && lastowner != iTeam) {
      GameCallEx("FlagLost", this, team, iTeam, pAttackers);
      if(IsFrontlines())
        GameCallEx("FlagFrontlinesStatusChange", this, team, false);
    }
    attacker = 0;
    team = iTeam;
  }

  UpdateFlag();

  var clr = GetTeamColor(iTeam), plr;
  if( (GetTeamConfig(TEAM_AutoGenerateTeams) && GetTeamPlayerCount(iTeam) <= 1 && (plr = GetTeamMemberByIndex(iTeam, 0)) > -1) || !GetTeamConfig(TEAM_TeamColors))
    clr = GetPlrColorDw(plr);

  bar->SetBarColor(clr);
  if(process >= 100)
  {
    if(SetIcon(NormalIcon()))
      bar->Update(0, true, true);
  }
  else SetIcon(OFPL_ICON_CONQUERING);

  if(currentIcon != OFPL_ICON_NORMAL)
    bar->Update(process);

  return process;
}

private func NormalIcon() {
  if (IsBacklineFlag())
    return OFPL_ICON_BACKLINE;
  return OFPL_ICON_NORMAL;
}

private func SetIcon(id newIcon) {
  if(currentIcon == newIcon)
    return false;
  bar->SetIcon("", newIcon, 0, 0, 32);
  currentIcon = newIcon;
  return true;
}

/* Flaggenposten verschieben */

public func MoveFlagpost(int iX, int iY, string szName, int iRange, bool fNeutral)
{
  //Effekte
  for(var i = -80; i < -20; i += 10)
    CastParticles("MetalSplinter",1,20,0,i,50,80);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("Sandbag", 10, 70, 0,-10, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  Sound("FenceDestruct.ogg");
  Sound("StructureHit*.ogg");
  Sound("StructureDebris*.ogg");

  //Namen ändern
  if(szName)
    SetName(szName);

  //Besitzer neutralisieren
  if(fNeutral)
    NoTeam();

  //Reichweite setzen
  if(iRange) range = iRange;

  //Reichweitenanzeige vorhanden: Entfernen
  if(captureradiusmarker)
    RemoveObject(captureradiusmarker);

  //Verschieben und einblenden
  SetPosition(iX, iY);
  UpdateFlag();
  FadeIn(this,3);
  FadeIn(flag,3);
}
