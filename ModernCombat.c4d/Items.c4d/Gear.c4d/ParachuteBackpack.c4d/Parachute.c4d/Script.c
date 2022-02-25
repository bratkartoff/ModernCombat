/*-- Fallschirm --*/

#strict 2

local iCtrlInfluence;


/* Initialisierung */

protected func Initialize()
{
  //Soundschleife übergeben
  Sound("ParachuteFly.ogg", false, 0, 25, 0, +1);
}

/* Festlegung */

public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("ParachuteOpen*.ogg");
  SetAction("Open", pObj);
}

/* Timer */

protected func Fly()
{
  //Freiflug
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    SetActionTargets(0, 0, this);

    //Baldiger Boden- oder Wasserkontakt: Zusammenfallen
    if(GBackSolid(0, 20) || GetContact(0,-1, CNAT_Bottom) || InLiquid())
      return Close();
    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) / 8);
    SetYDir(30, 0, 10);
  }
  else
  {
    //Anhang ermitteln
    var targ = GetActionTarget();
    if(!targ)
      return StartFlyFree();

    //Baldiger Boden- oder Wasserkontakt: Zusammenfallen
    if(GBackSolid(0, 20) || GetContact(0,-1, CNAT_Bottom))
    {
      //Zusatzeffekte bei Anhang
      if(targ)
      {
        Sound("ParachuteLand.ogg");
        CastSmoke("Smoke3",8,10,0,20,20,150);
      }
      return Close();
    }
    if(InLiquid() || InLiquid(targ))
      return Close();

    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY())/8+iCtrlInfluence, targ);
    //Fall verlangsamen
    var y = Interpolate2(GetYDir(targ, 100), 300, 1, 5);
    SetYDir(y, targ, 100);

    //[Doppelstop] des Anhangs: Manuell losmachen
    if(GetPlrDownDouble(GetController(targ)))
    {
      if(GBackSolid(0, 20))
        return Close();
      else
        return StartFlyFree();
    }

    //Schwerverletzte losmachen
    if(GetCategory(targ) & C4D_Living)
      if(GetID(Contained(targ)) == FKDT)
      {
        SetPosition(GetX(), GetY()+22);
        return StartFlyFree();
      }
      else
        if(GetID(targ) != FKDT && GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT")
          return Close();

    //Verschachtelten Fallschirm zurück ins Freie
    if(Contained())
    {
      Exit();
      SetAction("Fly", targ, 0, true);
    }
  }
}

public func StartFlyFree()
{
  if(GetAction() == "Open")
    Schedule("SetAction(\"StartFlyFree\")", 25-GetActTime(), 0, this);
  else
    SetAction("StartFlyFree");

  return true;
}

/* Aufklappen */

protected func Opening()
{
  var targ = GetActionTarget();
  if(!targ || GetID(Contained(targ)) == FKDT || (GetOCF(targ) & OCF_Living && GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT"))
  {
    RemoveEffect("Flying", targ);
    return Close();
  }

  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this);
    SetAction("Fly", GetActionTarget());
    return;
  }
  var w = Sin(36 * GetActTime() / 10, 1000);
  var h = 40 * GetActTime();
  SetObjDrawTransform(w, 0, 0, 0, h, InvertA1(500 * GetObjHeight() * h / 1000, 1000 * GetObjHeight()) - 500 * GetObjHeight(), this);
}

public func Close()
{
  if(WildcardMatch(GetAction(),"*Free*"))
    SetAction("FoldFree");
  else
    SetAction("Fold");

  SetActionTargets(0, 0);
  Sound("ParachuteClose.ogg");
  //Soundschleife beenden
  Sound("ParachuteFly.ogg", false, 0, 25, 0, -1);
}

private func Folded()
{
  //Verschwinden
  FadeOut();

  //Soundschleife beenden
  Sound("ParachuteFly.ogg", false, 0, 25, 0, -1);
}

/* Aufnahme verhindern */

public func RejectEntrance()
{
  return true;
}

/* Verlust des Anhangs */

public func AttachTargetLost()
{
  if(ActIdle())
    StartFlyFree();
  return;
}

/* Schaden */

public func Damage()
{
  //Schaden: Anhang losmachen
  if(GetDamage() > 30 && !WildcardMatch(GetAction(), "*Free*") && !WildcardMatch(GetAction(), "*Fold*") && !WildcardMatch(GetAction(), "Lie*"))
  {
    if(GBackSolid(0, 20))
      Close();
    else
      StartFlyFree();

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter", 4, 40, 0,0, 20, 50, RGBa(250,250,250,200));
    Sound("LineBreak");
  }

  return true;
}

/* Steuerung */

public func ControlLeft(object caller)
{
  var effect;
  if(effect = GetEffect("ControlInfluence", this))
    return EffectVar(0, this, effect) = -1;

  return AddEffect("ControlInfluence", this, 101, 3, this, 0, -1);
}

public func ControlRight(object caller)
{
  var effect;
  if(effect = GetEffect("ControlInfluence", this))
    return EffectVar(0, this, effect) = +1;

  return AddEffect("ControlInfluence", this, 101, 3, this, 0, +1);
}

public func ControlDown(object caller)
{
  return RemoveEffect("ControlInfluence", this);
}

public func ControlUpdate(object pByObj, int comdir, bool dig, bool throw)
{
  if(comdir == COMD_Left)
    iCtrlInfluence = BoundBy(iCtrlInfluence-1, -15, 15);
  else if(comdir == COMD_Right)
    iCtrlInfluence = BoundBy(iCtrlInfluence-1, -15, 15);

  return true;
}

public func FxControlInfluenceStart(object pTarget, int iNr, temp, int iChange)
{
  EffectVar(0, pTarget, iNr) = iChange;
}

public func FxControlInfluenceTimer(object pTarget, int iNr)
{
  //Kein Anhang: Kontrolleffekt entfernen
  if(!GetActionTarget())
    return -1;
  else
    iCtrlInfluence = BoundBy(iCtrlInfluence+EffectVar(0, pTarget, iNr), -15, 15);

  return true;
}