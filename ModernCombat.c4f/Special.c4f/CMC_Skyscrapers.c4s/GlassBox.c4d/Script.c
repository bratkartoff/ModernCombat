/*-- Glasbox --*/

#strict 2


/* Aufnahme */

public func RejectEntrance(object pObj)
{
  Open(pObj);
}

/* Öffnen */

public func Activate(object pObj)
{
  Open(pObj);
}

/* Gegenstand erstellen */

public func Open(object pObj)
{
  //Nur öffnen, wenn der Träger ein spielergesteuertes Objekt ist
  if((GetOCF(pObj) & OCF_CrewMember))
  {
    //Zufälligen Gegenstand erstellen
    var weapons = [
    RVLR,	//Revolver
    ATWN,	//APW
    PPGN,	//Schrotflinte
    MNGN	//Maschinengewehr
    ];
    var ids = weapons;

    var grenades = [
    FGRN,	//Feldgranate
    FRAG,	//Splittergranate
    PGRN,	//Phosphorgranate
    SGRN,	//Rauchgranate
    STUN,	//Blendgranate
    SRBL	//Sensorball
    ];

    //Maximal 9 Handgranaten
    if (pObj->GrenadeCount() < 9)
    {
      ids ..= grenades;
    }

    //Nur jeweils ein Defibrillator und Dragnin sowie eine volle Sprengfallentasche
    if(!FindContents(CDBT, pObj)) {ids[GetLength(ids)] = CDBT;}												//Defibrillator
    if(!FindContents(DGNN, pObj)) {ids[GetLength(ids)] = DGNN;}												//Dragnin
    if(!FindContents(BTBG, pObj) || FindContents(BTBG, pObj)->GetPackPoints() < FindContents(BTBG, pObj)->MaxPoints()) {ids[GetLength(ids)] = BTBG;}	//Sprengfallentasche

    var objid = ids[Random(GetLength(ids))];
    var obj = CreateContents(objid, pObj);

    //Laden wenn Waffe
    for(var weaponid in weapons)
    {
      if(objid == weaponid)
      {
        obj->DoAmmo(obj->GetFMData(FM_AmmoID), obj->GetFMData(FM_AmmoLoad));
        break;
      }
    }

    //Effekte
    Sound("GlassBreak*.ogg");
    CastParticles("Glas", 10, 50, 0,0, 20,40, RGBa(200,200,200), RGBa(200,200,200));

    //Verschwinden
    RemoveObject();
  }
}

/* Aufschlag */

public func Hit()
{
  Sound("GlassBreak*.ogg");
}