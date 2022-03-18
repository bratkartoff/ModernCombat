/*-- Flaggenwähler --*/

#strict 3

local szFunction, iClass;
local spawn,flagpoles,selection,oldvisrange,oldvisstate;

public func IsSpawnObject()	{return true;}


/* Initalisierung */

public func Initialize()
{
  SetVisibility(VIS_Owner);
  szFunction = "";
}

global func CreateGOCCSpawner(object pCrew, int iChoosedClass)
{
  if(!pCrew) pCrew = this;
  if(!pCrew) return false;

  var spawner = CreateObject(OSPW);
  Enter(spawner, pCrew);

  LocalN("iClass", spawner) = iChoosedClass;

  return spawner;
}

global func FindGOCC()
{
  return FindObject2(Find_Or(Find_ID(GOCC), Find_ID(GFRN)));
}


protected func Collection2(object pObject)
{
  if(!(GetOCF(pObject) & OCF_CrewMember)) return;
  SetOwner(GetController(pObject));

  //Bei Initialisierung Werte speichern
  oldvisrange = GetObjPlrViewRange(pObject);
  oldvisstate = GetPlrFogOfWar(GetOwner(pObject));

  SetFoW(true, GetOwner(pObject));
  SetPlrViewRange(0, pObject);

  if(!flagpoles)
  {
    flagpoles = FindGOCC()->GetSortedFlags();
    if(!flagpoles)
    {
      ErrorLog("Couldn't find any flags");
      GameOver();
    }
  }

  SpawnMenu();
  SelectBestFlag();
}

private func SelectBestFlag()
{
  var flag = GetBestFlag(GetPlayerTeam(GetOwner(Contents())));
  if(flag)
    SetSelected(flag);
}

global func GetFlagDistance(object pFlag)
{
  var flags = [];
  var gocc = FindGOCC();
  if(gocc) flags = gocc->GetFlags();
  var i = 0;
  while(i < GetLength(flags)-1)
  {
    if(pFlag == flags[i])
    {
      break;
    } 
    i++;
  }

  var down = i;
  var up = i;
  var iTeam = pFlag->GetTeam();
  var dist = 0;
  while(down >= 0 || up <= GetLength(flags)-1)
  {
    if(flags[down] && down >= 0)
      if(flags[down]->GetTeam() != iTeam)
      {
        dist = i - down;
        break;
      }
    down--;
    if(flags[up] && up >= 0)
      if(flags[up]->GetTeam() != iTeam)
      {
        dist = up - i;
        break;
      }
    up++;
  }
  return dist;
}

// todo: why is this global (lots of FindObject calls)
global func GetBestFlag(int iTeam)
{
  var capture = 0;
  var best;
  var dist = 0;
  var flags = [];

  var gocc = FindGOCC();
  // todo: frontlines?
  if(gocc) flags = gocc->GetFlags();
  for(var flag in flags)
  {
    if(flag->GetTeam() == iTeam)
    {
      if(flag->GetProcess() > capture || GetFlagDistance(flag) < dist) //Wer bietet mehr?
      {
        dist = GetFlagDistance(flag);
        capture = flag->GetProcess();
        best = flag;
      }
    }
  }

  return best;
}

/* KI-Funktion */

public func AutoSelect()
{
  SelectBestFlag();
  SpawnOk();
}

func SpawnOk()
{
  if(!Contents())
    return RemoveObject();
  if(GetOwner(Contents()) == NO_OWNER)
    return RemoveObject();

  //Sichtdaten zurücksetzen
  SetFoW(oldvisstate,GetOwner(Contents()));
  SetPlrViewRange(oldvisrange,Contents());

  spawn = 1;
  CloseMenu(Contents());
  Spawn();
}

public func ContainedLeft()	{return 1;}
public func ContainedRight()	{return 1;}
public func ContainedDown()	{return 1;}
public func ContainedUp()	{return 1;}
public func ContainedDig()	{return 1;}
public func ContainedThrow()	{return 1;}

public func Spawn()
{
  if(!Contents()) return RemoveObject();

  var obj = Contents();
  Exit(obj);
  //Hinweis: Eigene Spawnmöglichkeiten via GameCall
  if(!GameCall(szFunction,obj))
  {
    var tim = CreateObject(TIM2);
    Enter(tim, obj);
    tim->Spawn();
    RemoveObject();
  }

  return 1;
}

public func SpawnMenu()
{
  var crew = Contents();
  if(!crew) return;

  var team = GetPlayerTeam(GetOwner(crew));
  if(!team) return;

  if(!MenuNeedsUpdate()) return;

  if(GetMenu(crew))
    ClearMenuItems(crew);
  else
    CreateMenu(OFLG,crew,nil,C4MN_Extra_Info,"$SpawnMenu$",0,C4MN_Style_Dialog);

  for(var flag in flagpoles)
  {
    AddMenuItem(GetName(flag),"SelectFlagpole2",GetID(),crew,flag->GetProcess(),ObjectNumber(flag),"",C4MN_Add_ImgIndexedColor, GetIconIndex(flag), GetColorForMenu(flag));
  }

  SelectMenuItem(selection,crew);
}

protected func SelectFlagpole2(id unused,int iObject)
{
  SelectFlagpole(Object(iObject));
}

public func SelectFlagpole(object pObject)
{
  var crew = Contents();
  if(!crew) return;

  if((pObject->GetTeam() != GetPlayerTeam(GetOwner(crew))) || (!pObject->IsFullyCaptured()))
  {
    SetPlrViewRange(0, crew);
    SpawnMenu();
    return Sound("Error", false, crew, 100, GetOwner(crew)+1);
  }

  SetPlrViewRange(Min(200, oldvisrange), crew);//Nicht mehr als maximal 200px sehen.

  var X, Y, szFunc;
  pObject->GetSpawnPoint(X, Y, szFunc, GetOwner(crew));

  SetPosition(GetX(pObject) + X, GetY(pObject) + Y);

  if(szFunc) szFunction = szFunc;

  SpawnOk();

  if(FindObject(MCSL))
    FindObject(MCSL)->SpawnEventInfo(Format("$SpawnAt$", GetName(pObject)), GetOwner(crew), iClass, FindObject(GOCC));

  CloseMenu(crew);
}

protected func Timer()
{
  if(spawn) return;

  var crew = Contents();
  if(!crew) return;

  if(!flagpoles) return;

  selection = GetMenuSelection (crew);

  if(GetSelected())
    ShowFlagpole(GetSelected(), Contents(), this, oldvisrange);

  SpawnMenu();
}

protected func OnMenuSelection(int iSelection)
{
  var crew;
  if(spawn || !(crew = Contents()) || !flagpoles) return;
  selection = iSelection;
}

global func ShowFlagpole(object pObject, object pCrew, object pContainer, int iMaxrange)
{
  if(!pCrew) return;
  if(!pObject) return;
  if(!iMaxrange) iMaxrange = 200;

  if(pObject->GetTeam() != GetPlayerTeam(GetOwner(pCrew)))
  {
    SetPlrViewRange(0,pCrew);
    return;
  }

  SetPlrViewRange(Min(200,iMaxrange), pCrew);

  if(!pContainer) pContainer = pCrew;
  if(!pContainer) return false;
  SetPosition(GetX(pObject), GetY(pObject), pContainer);
  return true;
}

protected func MenuQueryCancel()
{
  return !spawn;
}

private func GetColorForMenu(object pFlagpole)
{
  var color;
  if(!pFlagpole->GetTeam())
    color = RGB(255,255,255);
  else
  {
    if(pFlagpole->GetProcess() >= 100)
      color = GetTeamColor(pFlagpole->GetTeam());
    else
      // todo: use hsl discoloration
      color = SetRGBaValue(GetTeamColor(pFlagpole->GetTeam()), 255/2, 0);
  }

  return color;
}

private func GetIconIndex(object flag)
{
  // trend is -1, 0, or 1, warn is a boolean
  var warn = flag->IsAttacked();
  var trend = flag->GetTrend();
  return 1 + trend + 3 * warn;
}

local last_frame_values;
private func MenuNeedsUpdate()
{
  if (!last_frame_values)
  {
    last_frame_values = {};
    for(var flag in flagpoles)
      last_frame_values[flag] = ChangeDetectionState(flag);
    return true;
  }
  else
  {
    var did_change = false;
    for(var flag in flagpoles)
    {
      var current_state = ChangeDetectionState(flag);
      if (current_state != last_frame_values[flag])
      {
        last_frame_values[flag] = current_state;
        did_change = true;
      }
    }
    return did_change;
  }
}

// MenuNeedsUpdate will compare these array, if any value changes the menu will update
private func ChangeDetectionState(flag)
{
  return [flag->IsAttacked(), flag->GetTrend(), flag->GetProcess(), GetName(flag)];
}


protected func GetSelected()
{
  var crew = Contents();
  if(!crew) return;

  if(selection == -1)
    return;

  if(selection >= -1)
    return flagpoles[selection];
}

protected func SetSelected(object flag)
{
  var crew = Contents();
  if(!crew) return;

  if(!flag) return;

  var i = GetIndexOf(flag, flagpoles);
  if(i == -1) return;

  selection = i;
  SelectMenuItem(selection,crew);
}