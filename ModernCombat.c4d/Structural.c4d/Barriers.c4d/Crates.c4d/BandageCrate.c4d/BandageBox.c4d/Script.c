/*-- Verband --*/

#strict 2

public func NoArenaRemove()	{return true;}


/* Aufnahme */

public func RejectEntrance(object pObj)
{
  var pFAP;

  //Clonk heilen wenn nötig, sonst in dessen EHP einfügen sofern vorhanden
  if(pObj->GetAlive() && (GetEnergy(pObj) < GetPhysical("Energy",0,pObj)/1000))
  {
    if(!GetEffect("*Heal*",pObj))
    {
      AddEffect("BandageHeal",pObj,20,1,0,BDGE,HealAmount(),HealRate());
      Sound("FAPK_Healing*.ogg");
      Sound("PackGrab*.ogg");

      RemoveObject();
    }
  }
  else if((pFAP = FindContents(FAPK, pObj)) && pFAP->~GetPackPoints() < pFAP->~MaxPoints() && !GetEffect("FAPHeal", pFAP))
  {
    pFAP->~DoPackPoints(HealAmount());
    PlayerMessage(GetOwner(pObj), "$Refilled$", pObj, FAPK, HealAmount());
    Sound("Merge.ogg",0,pFAP,0,GetOwner(pObj)+1);
    Sound("PackGrab*.ogg");

    RemoveObject();
  }

  return true;
}

/* Verbandseffekt */

func HealRate()		{return 3;}
func HealAmount()	{return 15;}

func FxBandageHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate)
{
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return;

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount;	//Heilung insgesamt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate;	//Frames per HP
}

func FxBandageHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Effekt
  pTarget->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pTarget)+25+Sin(iEffectTime*5,35),RGB(0,230,255),pTarget);
  //Heilen
  if(!(iEffectTime % EffectVar(1, pTarget, iEffectNumber)))
  {
    DoEnergy(1, pTarget);
    EffectVar(0,pTarget,iEffectNumber)--;
  }
  //Schon voll geheilt?
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000)
  {
    return -1;
  }
  //Schon leer?
  if(!EffectVar(0,pTarget,iEffectNumber))
  {
    return -1;
  }
  //FakeDeath?
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT)
  {
    return -1;
  }
  //Bildschirm Effekt
  if(!(iEffectTime % 20))
  {
    ScreenRGB(pTarget,RGBa(0, 230, 255, 190), 80, 3,false, SR4K_LayerMedicament, 200);
  }
}

func FxBandageHealDamage(target, no, dmg, dmgtype)
{
  //Bei Schaden abbrechen
  if(dmg < 0) RemoveEffect(0,target,no);

  return dmg;
}

public func FxBandageHealStop(object pTarget, no, reason, temp)
{
  if(temp) return;
}

/* Schaden */

protected func Damage(int iChange)
{
  if(GetDamage() < 10) return;
  Destruct();
}

protected func Destruct()
{
  //Effekte
  CastParticles("Paper", RandomX(4, 8), 40, 0, 0, 20, 35, RGB(180, 180, 180), RGBa(240, 240, 240, 150));
  Sound("FAPK_Hit*.ogg", false, this);

  //Verschwinden
  RemoveObject();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Aufschlag */

protected func Hit()
{
  Sound("FAPK_Hit*.ogg");
}