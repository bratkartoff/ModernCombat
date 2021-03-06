/*-- Grenze --*/

#strict 2

local x,y,xh,yh;
local fAbyss, fReopenDoors;
local fTeamAllow, iAllowedTeam, iSearchDir;


/* Einstellung */

public func Set(parameter, bool fKeepSpawns, bool fAbyssKill, bool fTeamCheck, int iTeam, bool fLockDoors)
{
  fAbyss = fAbyssKill;

  //Teamspezifisch suchen?
  if(fTeamCheck)
  {
    fTeamAllow = true;
    iAllowedTeam = iTeam;
  }

  //Suchrichtung einstellen
  if(GetType(parameter) == C4V_Array)
  {
    x = parameter[0];
    y = parameter[1];
    xh = parameter[2];
    yh = parameter[3];

    iSearchDir = 4;
  }
  else
  {
    iSearchDir = parameter;

    if(parameter == 0)
    {
      x = -GetX();
      y = -GetY();
      xh = GetX();
      yh = LandscapeHeight();
    }
    if(parameter == 1)
    {
      x = 0;
      y = -GetY();
      xh = LandscapeWidth()-GetX();
      yh = LandscapeHeight();
    }
    if(parameter == 2)
    {
      x = -GetX();
      y = -GetY();
      xh = LandscapeWidth();
      yh = GetY();
    }
    if(parameter == 3)
    {
      x = -GetX();
      y = 0;
      xh = LandscapeWidth();
      yh = LandscapeHeight()-GetY();
    }
  }

  //Spawnpoints entfernen?
  if(!fKeepSpawns)
    for(var obj in FindObjects(Find_InRect(x, y, xh, yh), Find_Func("IsSpawnpoint")))
      RemoveObject(obj);

  //Türen und Luken abschließen?
  if(fLockDoors)
  {
    for(var obj in FindObjects(Find_And(Find_InRect(x, y, xh, yh), Find_Or(Find_ID(GTNG), Find_ID(GDDR), Find_ID(H24K), Find_ID(HA4K)))))
      obj->Lock();
    fReopenDoors = true;
  }
}

/* Grenzüberschreiter suchen */

private func Check()
{
  var id;
  for(var clonk in FindObjects(Find_InRect(x, y, xh, yh), Find_Or(Find_OCF(OCF_CrewMember), Find_Func("IsBorderTarget"))))
    if(!GetEffect("Border", clonk) || fAbyss)
    {
      if((fTeamAllow && GetPlayerTeam(GetOwner(clonk)) == iAllowedTeam) || Contained(clonk) && (id = GetID(Contained(clonk))) && (id == TIM1 || id == TIM2 || id == FKDT))
        continue;
      AddEffect("Border", clonk, 50-fAbyss, 35, this);
    }
  for(var flag in FindObjects(Find_InRect(x, y, xh, yh), Find_ID(FLA2), Find_Action("Lost")))
    RemoveObject(flag);
}

public func IsDangerous(object pFor, int iX, int iY)
{
  if(!pFor || (!(GetOCF(pFor) & OCF_CrewMember) && !pFor->~IsBorderTarget()))
    return false;

  var cursor = pFor;
  if(!(GetOCF(pFor) & OCF_CrewMember))
    cursor = pFor->~GetRealCursor();

  if(fTeamAllow && GetPlayerTeam(GetOwner(cursor)) == iAllowedTeam)
    return false;

  if(!iSearchDir && iX < GetX())
    return true;
  else if(iSearchDir == 1 && iX > GetX())
    return true;
  else if(iSearchDir == 2 && iY < GetY())
    return true;
  else if(iSearchDir == 3 && iY > GetY())
    return true;
  else if(iSearchDir == 4 && Inside(iX, GetX() + x, GetX() + x + xh) && Inside(iY, GetY() + y, GetY() + y + yh))
    return true;

  return false;
}

/* Effekt */

protected func FxBorderStart(pTarget, iNo, iTemp)
{
  if(iTemp)
    return -1;

  if(fAbyss)
  {
    if(GetOCF(pTarget) & OCF_CrewMember)
    {
      //Killverfolgung
      pTarget->~KillIcon(SM10);
      pTarget->~LastDamageType(DMG_Melee);

      if(!FindObject(NOFD) && !FindObject(SICD))
        Sound("FallIntoAbyss*.ogg", 1, 0, 0, GetOwner(pTarget) + 1);

      //Ehrenband-Fortschritt (The End)
      if(Hostile(GetKiller(pTarget), GetOwner(pTarget)))
        AttemptAwardRibbon(RB13, GetKiller(pTarget), GetOwner(pTarget));

      //Standardwartezeit setzen
      GameCall("SetPlayerRespawnTime", GetOwner(pTarget), FKDT_SuicideTime*35);

      //Opfer töten
      Kill(pTarget);
      AddEffect("SilentKill", pTarget, 1);
    }
    else
      pTarget->~BorderDestruction();

    return -1;
  }

  //Countdown
  EffectVar(2, pTarget, iNo) = EffectVar(0, pTarget, iNo) = 10;
  Sound("Info_Alarm.ogg", 0, pTarget, 0, GetOwner(pTarget) + 1);

  //Hinweisnachricht
  if(GetOCF(pTarget) & OCF_CrewMember || pTarget->~GetRealCursor())
    PlayerMessage(GetOwner(pTarget), "$Warning$", pTarget, EffectVar(0, pTarget, iNo));

  //Overlay
  EffectVar(1, pTarget, iNo) = ScreenRGB(pTarget, 1, 1, -10, false, SR4K_LayerBorder, 144);
}

protected func FxBorderTimer(pTarget, iNo, iTime)
{
  //var danger = (GetIndexOf(pTarget, FindObjects(Find_InRect(x, y, xh, yh), Find_OCF(OCF_CrewMember))) != -1);

  //Prüfen, ob Spieler noch im Grenzgebiet ist
  var danger = false;

  for(var pBorder in FindObjects(Find_ID(BRDR)))
    if(danger = pBorder->IsDangerous(pTarget, GetX(pTarget), GetY(pTarget)))
      break;

  //Ziel wieder im Sicheren oder schwerverletzt?
  if(!danger || IsFakeDeath(pTarget))
    return -1;

  if(!EffectVar(0, pTarget, iNo))
  {
    if(GetOCF(pTarget) & OCF_CrewMember)
    {
      //Killverfolgung
      pTarget->~KillIcon(SM15);
      pTarget->~LastDamageType(DMG_Projectile);

      //Standardwartezeit abzüglich Grenzobjekt-Cooldown setzen
      GameCall("SetPlayerRespawnTime", GetOwner(pTarget), (FKDT_SuicideTime-EffectVar(2, pTarget, iNo))*35);

      //Opfer töten
      Kill(pTarget);
      AddEffect("SilentKill", pTarget, 1);
    }
    else
      pTarget->~BorderDestruction();

    Sound("BRDR_Fire.ogg", true, pTarget);
    return -1;
  }

  EffectVar(0, pTarget, iNo)--;

  if(GetOCF(pTarget) & OCF_CrewMember || pTarget->~GetRealCursor())
    PlayerMessage(GetOwner(pTarget), "$Warning$", pTarget, EffectVar(0, pTarget, iNo));
  else
    Message("");

  var obj = EffectVar(1, pTarget, iNo);
  if(obj)
  {
    RemoveEffect("IntRGBFade", obj, 0, true);
    obj->~SetAlpha(128);
  }
}

protected func FxBorderStop(pTarget, iNo, iReason, fTemp)
{
  if(!fTemp)
  {
    PlayerMessage(GetOwner(pTarget), "@", pTarget);
    if(EffectVar(1, pTarget, iNo))
      ScreenRGB(pTarget, RGB(0, 0, 1, 128), 0, 10, 0, SR4K_LayerBorder, 144);
  }
}

protected func Destruction()
{
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
    if(GetEffect("Border", obj, 0, 4) == this)
      RemoveEffect("Border", obj);

  if(fReopenDoors)
    for(var obj in FindObjects(Find_And(Find_InRect(x, y, xh, yh), Find_Or(Find_ID(GTNG), Find_ID(GDDR), Find_ID(H24K), Find_ID(HA4K)))))
      obj->Unlock();
}