/*-- Kaufstation --*/

#strict 2

local aWare;
local aCount;


/* Initialisierung */

func Initialize()
{
  aWare = CreateArray();
  aCount = CreateArray();

  Sound("WPVM_Scan.ogg",false,this,30,0,+1,0,80);
}

/* Bildschirmeffekt */

func Display()
{
  //Ware vorhanden?
  if(!GetLength(aWare))
    return SetGraphics(0,0,0,1);

  var i = Random(GetLength(aWare));
  SetGraphics(0,this,aWare[i],1,GFXOV_MODE_IngamePicture,0,1);
  if(!aCount[i])
    SetClrModulation(RGB(150,100,100),0,1);

  SetObjDrawTransform(218,0,-16000,0,203,-6000,0,1);
}

public func SortWare()
{
  var length;
  while(Par(length++));

  var ware = CreateArray(), count = CreateArray();

  //Nach Kategorie sortieren
  for(var y = 0; y < length; y++)
  {
    ware[y] = CreateArray();
    count[y] = CreateArray();
    for(var x = 0; x < GetLength(aWare); x++)
    {
      if(DefinitionCall(aWare[x],Par(y)))
      {
        AddToArray(ware[y],aWare[x]);
        AddToArray(count[y],aCount[x]);
      }
    }
  }

  //Nach Preis sortieren
  for(var y = 0; y < length; y++)
  {
    for(var x = 0; x < GetLength(ware[y]); x++)
    {
      for(var z = x+1; z < GetLength(ware[y]); z++)
      {
        if(GetDefValue(ware[y][z]) < GetDefValue(ware[y][x]))
        {
          var tmp = ware[y][x];
          ware[y][x] = ware[y][z];
          ware[y][z] = tmp;
          tmp = count[y][x];
          count[y][x] = count[y][z];
          count[y][z] = tmp;
        }
      }
    }
  }

  //Neue Sortierung speichern
  aWare = aCount = CreateArray();
  var cnt;
  for(var y = 0; y < length; y++)
  {
    for(var x = 0; x < GetLength(ware[y]); x++)
    {
      aWare[cnt] = ware[y][x];
      aCount[cnt] = count[y][x];
      cnt++;
    }
  }
}

private func AddToArray(&a, add)
{
  a[GetLength(a)] = add;
}

/* Steuerung */

func ControlThrowDouble(object pByObj)
{
  //Aktueller Gegenstand im Clonk verkaufen sofern dieser auch angeboten wird
  var pItem = Contents(0, pByObj);
  if(!pItem) return;
  if(!FindWare(GetID(pItem))) return;

  var iPlr = GetController(pByObj);

  //Halben Warenwert ausgeben
  DoWealth(iPlr,GetValue(pItem,0,0,iPlr)/2);
  Sound("UnCash",0,0,0,iPlr+1);
  Sound("ResupplyOut*.ogg");

  //Gegenstand entfernen und dem Kaufmenü hinzufügen
  DoWare(GetID(pItem), +1);
  RemoveObject(pItem);
}

public func ControlUp(object pByObj)
{
  CreateBuyMenu(pByObj);
}

public func ControlDig(object pByObj)
{
  CreateBuyMenu(pByObj);
}

/* Kaufmenü */

protected func CreateBuyMenu(object pClonk)
{
  if(!pClonk || !GetLength(aWare))
    return;
  CreateMenu(WPVM, pClonk, this,0,0,0,0,1);
  for(var x; x < GetLength(aWare); x++)
    AddBuyMenuItem(aWare[x], pClonk, aCount[x]);
}

private func AddBuyMenuItem(id id, object pClonk, int iCount)
{
  if(!id || !pClonk)
    return;

  //Keine Munition-Regel vorhanden: Munition nicht anzeigen
  if(DefinitionCall(id,"IsAmmoPacket") && NoAmmo())
    return;

  var name = GetName(0,id),
  	obj = CreateObject(id),
  	wealth = GetDefValue(id),
  	iPlayer = GetOwner(pClonk),
  	bMoney = false;

  //Ware zu teuer oder aufgebraucht: Text ausgrauen
  if(!BuyAble(id, iPlayer))
  {
    name = Format("<c 646464>%s</c>",name);
    SetClrModulation(RGB(100,100,100), obj);
    bMoney = true;
  }

  //Zusatzeffekte abrufen
  this->~AdaptMenuItem(obj, iPlayer);
  var func = "BuyItem";
  if(obj->~IsPack())
    func = "BuyPack";

  //Menüpunkt hinzufügen
  if(!iCount)
    AddMenuItem(Format("%3d: %s",wealth,name),func,id,pClonk,0,pClonk,GetDesc(0,id),4|C4MN_Add_ForceCount, obj);
  else if(bMoney)
    AddMenuItem(Format("%3d: %s",wealth,name),func,id,pClonk,0,pClonk,GetDesc(0,id),4, obj);
  else if(iCount < 0)
    AddMenuItem(Format("%3d: %s",wealth,name),func,id,pClonk,0,pClonk,GetDesc(0,id),4, obj);
  else
    AddMenuItem(Format("%3d: %s",wealth,name),func,id,pClonk,iCount,pClonk,GetDesc(0,id),4, obj);

  RemoveObject(obj);
}

/* Warenkauf */

private func BuyItem(id Item, object pClonk)
{
  var iPlr = GetOwner(pClonk);

  //Kaufbar?
  if(!BuyAble(Item,iPlr))
    return Sound("BKHK_SwitchFail.ogg", 0, 0, 0, iPlr + 1);

  //Ware erstellen
  var tmp = CreateObject(Item, 0, -GetDefOffset(GetID(), 1), iPlr);

  //Munition: Direkt in Munitionsgürtel
  if(tmp->~IsAmmoPacket())
    if(tmp->~MayTransfer(pClonk))
      tmp->~TransferAmmo(pClonk);
    else
    {
      RemoveObject(tmp);
      return Sound("BKHK_SwitchFail.ogg", 0, 0, 0, iPlr + 1);
    }
  else
    //Handgranate: Direkt in Granatgürtel
    if(tmp->~IsGrenade())
      if(!pClonk->~StoreGrenade(tmp))
      {
        RemoveObject(tmp);
        return Sound("BKHK_SwitchFail.ogg", 0, 0, 0, iPlr + 1);
      }
      else
      {
        //Aufnahme erfolgreich
      }
    else
      //Ansonsten normale Aufnahme versuchen
      if(pClonk->~RejectCollect(Item, tmp))
      {
        RemoveObject(tmp);
        return Sound("BKHK_SwitchFail.ogg", 0, 0, 0, iPlr + 1);
      }
      else
      {
        //Wenn Waffe: Nachladen
        if(tmp->~IsWeapon())
          tmp->DoAmmo(tmp->GetFMData(FM_AmmoID), tmp->GetFMData(FM_AmmoLoad));
        Enter(pClonk,tmp);
      }

  //Geld und Ware abziehen
  DoWealth(iPlr, -GetDefValue(Item));
  DoWare(Item, -1);

  //Effekte
  Sound("Cash", 0, 0, 0, iPlr + 1);
  Sound("WPVM_Buy*.ogg", 0, pClonk, 0, 0, 0, 1);
  CastParticles("BulletCasing", RandomX(2,4), 30, 15, 10, 22, 35);

  //Kaufmenü erneut öffnen
  var sel = GetMenuSelection(pClonk);
  CloseMenu(pClonk);
  CreateBuyMenu(pClonk);
  SelectMenuItem(sel,pClonk);
}

protected func BuyPack(id idPack, object pClonk)
{
  var iPlr = GetOwner(pClonk);
  if(!BuyAble(idPack, iPlr))
    return Sound("BKHK_SwitchFail.ogg", 0, 0, 0, iPlr + 1);

  //Pack erstellen und einsammeln lassen
  var obj = CreateObject(idPack, 0, -GetDefOffset(GetID(), 1), iPlr);
  Collect(obj, pClonk);

  //Geld und Ware abziehen
  DoWealth(iPlr, -Value(idPack));
  DoWare(idPack, -1);

  //Effekte
  Sound("Cash", 0, 0, 0, iPlr + 1);
  Sound("WPVM_Buy*.ogg", 0, pClonk, 0, 0, 0, 1);
  CastParticles("BulletCasing", RandomX(2,4), 30, 15, 10, 22, 35);

  //Kaufmenü erneut öffnen
  var sel = GetMenuSelection(pClonk);
  CloseMenu(pClonk);
  CreateBuyMenu(pClonk);
  SelectMenuItem(sel, pClonk);
}

private func BuyAble(id Item, int Plr)
{
  //Kaufbarkeit: Genug Geld und Ware
  if(GetWealth(Plr) < GetDefValue(Item))
    return false;
  if(GetWareCount(Item) == 0)
    return false;
  return true;
}

/* Waren-Funktionen */

//Warenbestand ändern
public func DoWare(id ID, int iChange)
{
  //Ware nicht im Angebot?
  if(!FindWare(ID))
    return false;
  if(!ID)
  {
    for(var ware in aWare)
      ChangeWare(ware,iChange);
  }
  else
    ChangeWare(GetWare(ID),iChange);
}

//Interne Funktion, die die Ware dann wirklich ändert
private func ChangeWare(int iIndex, int iChange)
{
  //Ware ist unendlich oft verfügbar?
  if(aCount[iIndex] < 0)
    return ;

  aCount[iIndex] += iChange;
  //Anzahl kleiner als 0?
  if(aCount[iIndex] < 0)
    //Anzahl auf 0 setzen
    aCount[iIndex] = 0;
}

//Ware hinzufügen
public func AddWare(id ID, int iCount)
{
  //Keine ID oder bereits vorhanden: Abbruch
  if(!ID || FindWare(ID))
    return false;

  //Negative Anzahl: Unendlich Ware
  if(iCount < -1)
    iCount = -1;

  //Ware hinzufügen
  AddToArray(aWare, ID);
  AddToArray(aCount,iCount);
}

//Alle Waren eines Typs hinzufügen
public func AddWares(string szCallback, int iCount)
{
  var id, defz;
  //alle Definitionen durchgehn
  while(id = GetDefinition(defz++))
  {
    //Objekt kann nicht gewählt werden?
    if(DefinitionCall(id,"NoWeaponChoice"))
      continue; //Weiter
    //Objekt ist vom richtigen Typ?
    if(DefinitionCall(id,szCallback))
      AddWare(id,iCount); //Hinzufügen
  }
}

//Ware entfernen
public func RemoveWare(id ID)
{
  //Wenn nichts angegeben, alles entfernen
  if(!ID)
  {
    aWare = aCount = CreateArray();
    return true;
  }

  if(!FindWare(ID))
    return false;

  var x = GetWare(ID);
  aWare[x] = 0;
  aCount[x] = 0;
  return true;
}

//Warenindex abfragen
private func GetWare(id ID)
{
  for(var x; x < GetLength(aWare); x++)
    if(aWare[x] == ID)
      return x;
  return -1;
}

//Anzahl einer Ware abfragen
public func GetWareCount(id ID)
{
  for(var x; x < GetLength(aWare); x++)
    if(aWare[x] == ID)
      return aCount[x];
}

//Warenliste abfragen
public func GetWareList()
{
  return aWare;
}

//Waren nach Kategorie abfragten
public func GetWares(string szCallback)
{
  var ret = CreateArray();
  for(var x; x < GetLength(aWare); x++)
    if(DefinitionCall(aWare[x], szCallback))
      ret[GetLength(ret)] = aWare[x];
  return ret;
}

//Bestimmte Ware auf bestimmten Wert setzen
public func SetWare(id id, int iCount)
{
  for(var x; x < GetLength(aWare); x++)
    if(!id || id == aWare[x])
      aCount[x] = iCount;
}

private func FindWare(id ID)
{
  for(var x in aWare)
    if(x == ID)
      return true;

  return false;
}

//Bei Aufsammlung zur Warenliste hinzufügen
func RejectCollect(id id, object pObj)
{
  if(FindWare(id))
    return RemoveObject(pObj);
  AddWare(id, 0);
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if(GetLength(aWare))
  {
    extra[GetLength(extra)] = Format("LocalN(\"aWare\")=%v", aWare);
    extra[GetLength(extra)] = Format("LocalN(\"aCount\")=%v", aCount);
  }
}

/* Ungenutzte Funktionen */

public func GetBuyableWaresFor(string szCallback, int player)	{}
public func DoBuyItem(id Item, int iPlr, object pClonk)		{}