/*--- Head-Up-Display ---*/

#strict 2

static const HUD_Default       = 0;
static const HUD_Contained     = 1;
static const HUD_Grabbed       = 2;
static const HUD_SpecialTarget = 3;

local CharsAmmo,	//Munition (Erste Zahl - Munitionswert)
      CharsSupply,	//Nachschub (Zweite Zahl - Clonk-Munitionswert)
      CharsGrenade,	//Granate (Dritte Zahl - Clonk-Handgranatenwert)
      CharSlash,	//Schrägstrich
      CharInfinity,	//Unendlichkeit
      rechargebar;	//Aktionsbalken

local pLastItem;
local iLastWeaponAmmo;
local iLastClonkAmmo;
local iLastWeaponFM;
local iLastWeaponFT;

public func ColorEmpty()	{return RGB(255, 0, 0);}
public func ColorLow()		{return RGB(255, 150, 0);}


/* Initialisierung */

protected func Initialize()
{
  //Am unteren, linken Bildschirmrand positionieren
  SetPosition(161, -80); 

  //Bestandteile erstellen
  //Munition
  CharsAmmo = [CreateObject(HCHA, -62, 28, GetOwner()), CreateObject(HCHA, -35, 28, GetOwner()), CreateObject(HCHA, -8, 28, GetOwner())];
  ResizeChars(CharsAmmo, 502);
  //Schrägstrich
  CharSlash = CreateObject(HCHA, 15, 35, GetOwner());
  CharSlash->Set(61);
  ResizeChars(CharSlash, 302);
  //Unendlichkeit
  CharInfinity = CreateObject(HCHA, 39, 35, GetOwner());
  CharInfinity->Set(120);
  ResizeChars(CharInfinity, 502);
  SetVisibility(VIS_None, CharInfinity);
  //Nachschub
  CharsSupply = [CreateObject(HCHA, 33, 36, GetOwner()), CreateObject(HCHA, 48, 36, GetOwner()), CreateObject(HCHA, 63, 36, GetOwner())];
  ResizeChars(CharsSupply, 292);
  //Granate
  CharsGrenade = [CreateObject(HCHA, 96, 37, GetOwner()), CreateObject(HCHA, 114, 37, GetOwner())];
  CharsGrenade[0]->Set(1337);
  ResizeChars(CharsGrenade, 262);
  //Aktionsbalken
  rechargebar = CreateObject(RBAR, 0, -12, GetOwner());

  //Werte initialisieren
  iLastWeaponAmmo = -1;
  iLastWeaponFT = -1;
  iLastWeaponFM = -1;

  //Sichtbarkeit nur für Besitzer
  SetVisibility(VIS_Owner);
}

/* Symbolgrößen verändern */

public func ResizeChars(chars, int size)
{
  //Keine Angabe: Normalgröße annehmen
  if(!size)
    size = 1000;

  //Symbol transformieren
  if(GetType(chars) == C4V_Array)
    for(var char in chars)
      SetObjDrawTransform(size, 0, 0, 0, size, 0, char);
  else
    SetObjDrawTransform(size, 0, 0, 0, size, 0, chars);

  return true;
}

/* Timer */

protected func Timer()
{
  //Existenz des Spielers überwachen
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner()) || (!GetAlive(GetCrew(GetOwner()))))
    RemoveObject(this);
}

/* Waffen-Anzeige ein- und ausblenden */

public func HideWeapons(object pClonk, object weapon, int source)
{
  pLastItem = 0;

  if(GetVisibility() == VIS_None)
    return true;

  //Anderer Objekttyp vorhanden: Namen anzeigen
  if(!weapon)
  {
    weapon = Contents(0,pClonk);
    source = HUD_Default;
  }

  if(weapon)
  {
    HUDMessage(GetName(weapon), source, true);
  }
  else
  {
    HUDMessage("", HUD_Default, true);
  }
}

public func ShowWeapons()
{
  if(GetVisibility() == VIS_Owner)
    return true;

  //Munition, Nachschub und Schrägstrich sichtbar machen
  for(var char in CharsAmmo)
    SetVisibility(VIS_Owner, char);
  for(var char in CharsSupply)
    SetVisibility(VIS_Owner, char);
  SetVisibility(VIS_Owner, CharSlash);
}

/* HUD-Aktualisierung */

public func HUDMessage(string message, int source, bool hideNumbers)
{
  if(hideNumbers)
  {
    //Munition, Nachschub, Schrägstrich, Unendlichkeit und Aktionsbalken unsichtbar machen
    for(var char in CharsAmmo)
    {
      SetVisibility(VIS_None, char);
    }

    for(var char in CharsSupply)
    {
      SetVisibility(VIS_None, char);
    }

    SetVisibility(VIS_None, CharSlash);
    SetVisibility(VIS_None, CharInfinity);
    SetVisibility(VIS_None, rechargebar);
  }

  var icon = CustomIconForSource(source);
  var strlimit = 29;
  if(icon)
  {
    message = Format("@{{%i}} %s", icon, message);
    strlimit = 22;
  }
  else
  {
    message = Format("@%s", message);
  }

  LimitString(message, strlimit + 15);

  return CustomMessage(message, this, NO_OWNER, 0, 72, 0, 0, 0, MSG_NoLinebreak);
}

private func CustomIconForSource(int source)
{
  if(source == HUD_Grabbed)
  {
    return SM04;
  }
}

protected func UpdateHUD(object weapon, object pClonk, bool fForceUpdate, int source)
{
  if(!pClonk) return true;

  //Granatenanzeige aktualisieren
  var grenades = Format("%d", pClonk->~GrenadeCount());
  CharsGrenade[1]->Set(GetChar(grenades));

  //Keine Waffe: HUD unsichtbar machen
  if(!weapon) return HideWeapons(pClonk);

  //Waffe gewechselt: Balken unsichtbar machen
  if(pLastItem != weapon)
  {
    SetVisibility(VIS_None, rechargebar);
    pLastItem = weapon;
    iLastWeaponAmmo = -1;
    iLastWeaponFM = -1;
    iLastWeaponFT = -1;
  }

  //Munition und Nachschub bei Waffen einblenden, ansonsten ausblenden
  if(weapon && !(weapon->~IsWeapon()) && !(weapon->~CustomHUD(source)))
    HideWeapons(pClonk, weapon, source);
  else if(GetVisibility() == VIS_None)
    ShowWeapons();

  //Ausrüstung mit eigener Anzeige
  if(weapon->~CustomHUD(source))
  {
    //Unendlichkeitsicon ausblenden
    if(GetVisibility(CharInfinity) == VIS_Owner)
      SetVisibility(VIS_None, CharInfinity);

    //Schrägstrich einblenden
    if(GetVisibility(CharSlash) == VIS_None)
      SetVisibility(VIS_Owner, CharSlash);

    //Aktionsbalken bei Bedarf ausblenden
    if(GetVisibility(rechargebar) != VIS_None && weapon->~IsWeapon())
      if(!weapon->IsRecharging() && !weapon->IsReloading())
        SetVisibility(VIS_None, rechargebar);

    ShowSelectProcess(weapon);
    return weapon->~UpdateHUD(this, source);
  }

  //Waffe
  if(weapon->~IsWeapon())
  {
    //Munition in Waffe ermitteln
    var weaponAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), weapon);
    var clonkAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), pClonk);
    if(fForceUpdate || iLastWeaponAmmo != weaponAmmo || iLastClonkAmmo != clonkAmmo|| iLastWeaponFM != weapon->~GetFireMode() || iLastWeaponFT != weapon->~GetFireTec())
    {
      iLastWeaponAmmo = weaponAmmo;
      iLastClonkAmmo = clonkAmmo;
      iLastWeaponFM = weapon->~GetFireMode();
      iLastWeaponFT = weapon->~GetFireTec();
      var maxAmmo = weapon->~GetFMData(FM_AmmoLoad);
      var wAmmo = Format("%03d", weaponAmmo);

      //Textfarbe ermitteln
      var clr = ColorEmpty()*(!weaponAmmo);
      if(Inside(weaponAmmo, 1, maxAmmo/3))
        clr = ColorLow();

      var i = 0;
      if(weapon)
      for(var char in CharsAmmo)
      {
        char->Set(GetChar(wAmmo, i), clr);
        i++;
      }

      i = 0;
      //Nachschub darstellen wenn Keine Munition-Regel nicht vorhanden, der Nutzer ein Clonk ist oder das Objekt extern Munition bezieht
      var user = weapon->GetUser();
      if(!NoAmmo() && user && (user == pClonk || user->~UsesExternalAmmo()))
      {
        //Unendlichkeitsicon ausblenden
        if(GetVisibility(CharInfinity) == VIS_Owner)
          SetVisibility(VIS_None, CharInfinity);

        //Nachschub aktualisieren
        var cAmmo = Format("%03d", clonkAmmo);
        if(clonkAmmo > 0)
        {
          for(var char in CharsSupply)
          {
            char->Set(GetChar(cAmmo, i), RGB(255, 255, 255));
            i++;
          }
        }
        else
          for(var char in CharsSupply)
            char->Set(48, ColorEmpty());

        //Schrägstrich einblenden
        if(GetVisibility(CharSlash) == VIS_None)
          SetVisibility(VIS_Owner, CharSlash);
      }
      else
      //Ansonsten Unendlichkeit und Schrägstrich einblenden und Nachschub ausblenden
      {
        //Unendlichkeitsicon einfügen
        if(GetVisibility(CharInfinity) == VIS_None)
          SetVisibility(VIS_Owner, CharInfinity);

        //Schrägstrich einblenden
        if(GetVisibility(CharSlash) == VIS_None)
          SetVisibility(VIS_Owner, CharSlash);

        //Nachschub ausblenden
        for(var char in CharsSupply)
          SetVisibility(VIS_None, char);
      }

      //Textanzeige: Feuermodus und Feuertechnik
      var ammoName = weapon->~GetFMData(FM_Name);
      if(weapon->~IsWeapon2() && weapon->GetFMData(FT_Name))
        var firemode = weapon->~GetFMData(FT_Name);
      else
        var firemode = weapon->~GetFMData(FM_Name);

      var str = Format("<c ffbb00>%s</c> - %s", ammoName, firemode);
      HUDMessage(str, source);
    }
    //Aktionsbalken-Position bei Bedarf repositionieren
    if(weapon->IsRecharging())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var recharge = weapon->GetRecharge();
      var x = -122+(244*recharge/100);
      SetPosition(GetX()+x, GetY()-12, rechargebar);
    }
    else if(weapon->IsReloading())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var charge = (weapon->GetCharge()/10);
      var x = -122+(244*charge/100);
      SetPosition(GetX()+x, GetY()-12, rechargebar);
    }
    else if(ShowSelectProcess(weapon))
    {
      //!
    }
    else if(GetVisibility(rechargebar) != VIS_None)
    {
      SetVisibility(VIS_None, rechargebar);
    }
  }
  return true;
}

public func Update(object weapon, object ammobag, object who, bool fForceUpdate, int source)
{
  UpdateHUD(weapon, who, fForceUpdate, source);
}

private func ShowSelectProcess(object item)
{
  var effect = GetEffect("SelectItem",item,0,6);
  if(effect)
    Recharge(effect,item->~SelectionTime());

  return effect;
}

global func GetHUD(object obj, id theID)
{
  if(!obj)
    if(!(obj = this))
      return;
  if(!theID)
    theID = 1HUD;
  var owner = GetOwner(obj);
  if(owner <= -1) return;
  var bar = FindObjectOwner(theID,owner);
  if(!bar)
    bar = CreateObject(theID, 0,0, owner);
  return bar;
}

public func Charge(int part, int max)
{
  //Recharge(part, max);
  return 1;
}

public func Recharge(int part, int max)
{
  if(part != max && part != 0)
    SetVisibility(VIS_Owner, rechargebar);
  else
    SetVisibility(VIS_None, rechargebar);

  var x = -122+(244*part/max);
  SetPosition(GetX()+x, GetY()-12, rechargebar);
  return 1;
}

/* Benutzerdefiniertes HUD (Ausrüstung) */

public func Ammo(int iAmmoCount, int iAmmoLoad, string szName, bool fShow, int dwColorW, int dwColorM, int source, bool fHide)
{
  var wAmmo = Format("%03d", iAmmoCount);
  var i = 0;

  //Munition aktualisieren
  if(!fHide)
  {
    if(!dwColorW) dwColorW = ColorEmpty()*(!iAmmoCount);
    for(var char in CharsAmmo)
    {
      char->Set(GetChar(wAmmo, i), dwColorW);
      i++;
    }
  }
  else
    for(var char in CharsAmmo)
    {
      if(GetVisibility(char) == VIS_Owner)
        SetVisibility(VIS_None, char);
    }

  //Nachschub aktualisieren
  var mAmmo = Format("%03d", iAmmoLoad);
  var i = 0;
  for(var char in CharsSupply)
  {
    char->Set(GetChar(mAmmo, i), dwColorM);
    i++;
  }

  //Wenn angegeben, Nachschub und Schrägstrich ausblenden
  if(!fShow)
  {
    if(GetVisibility(CharsSupply[0]) == VIS_Owner)
    {
      for(var char in CharsSupply)
        SetVisibility(VIS_None, char);

      if(GetVisibility(CharSlash) == VIS_Owner)
        SetVisibility(VIS_None, CharSlash);
    }
  }

  //Text einblenden falls vorhanden
  if(szName)
    HUDMessage(szName, source);

  return 1;
}

/* HUD-Nachschubinfo (Munitionsaufnahme) */

public func ShowResupplyInfo(id SupplyType, int SupplyAmount, int iPos)
{
  //Nachschubhinweis einblenden
  var ix,iy, iz;
  if(!iPos) {ix=-49; iy=33;}
  else {ix=-12; iy=33; iz=1;}

  //Munition oder Handgranate
  if(SupplyType->~IsAmmo() && !NoAmmo())
    CustomMessage(Format("<c ffbb00>+%d</c>{{%i}}",SupplyAmount,SupplyType), CharsGrenade[iz], NO_OWNER, ix, iy, 0, 0, 0, MSG_NoLinebreak);
  else
    if(SupplyType->~IsGrenade())
      CustomMessage(Format("<c ffbb00>+</c>{{%i}}",SupplyType), CharsGrenade[iz], NO_OWNER, ix, iy, 0, 0, 0, MSG_NoLinebreak);
    else
      return;

  return 1;
}

global func ResupplyInfo(object user, id SupplyType, int SupplyAmount, int iPos)
{
  //Spieler existiert und aufrufender Clonk wird gerade gesteuert und nicht innerhalb eines Spawnobjekts
  if(user && GetCursor(GetOwner(user)) == user)
    if(!Contained(user) || !Contained(user)->~IsSpawnObject())
      GetHUD(user)->~ShowResupplyInfo(SupplyType,SupplyAmount,iPos);
}

/* Bei Entfernung alle Zeichen mitlöschen */

public func Destruction()
{
  for(var char in CharsAmmo)
  {
    RemoveObject(char);
  }
  for(var char in CharsSupply)
  {
    RemoveObject(char);
  }
  for(var char in CharsGrenade)
  {
    RemoveObject(char);
  }
  if(CharSlash)
    RemoveObject(CharSlash);
  if(CharInfinity)
    RemoveObject(CharInfinity);
  if(rechargebar)
    RemoveObject(rechargebar);
}