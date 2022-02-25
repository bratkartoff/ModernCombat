/*-- Grundstruktur --*/

#strict 2

local fDestroyed;
local fRepairing;
local iAutorepairWait;
local iLastAttacker;
local destructability;

public func OnDestruction()		{}				//Bei der Zerstörung des Gebäudes, aber folgenden Reparatur
public func AutoRepairDuration()	{return 36*20;}			//Dauer der Reparatur
public func BonusPointCondition()	{return true;}			//Zusätzlicher Callback, ob Punkte vergeben werden
public func MaxDamage()			{return 100;}			//Maximalschaden
public func AutoRepairWait()		{return iAutorepairWait;}	//Zeit bis zu Autoreparatur

public func IsRepairing()		{return fRepairing;}		//Reparierend
public func IsDestroyed()		{return fDestroyed;}		//Zerstört
public func IsCMCStructure()		{return true;}			//Ist eine CMC Struktur

public func IsRepairable()		{return true;}			//Reparierbar
public func RepairSpeed()		{return 0;}			//Standard-Reparaturgeschwindigkeit
public func BonusPointRepair()		{return 50;}			//Benötigte Reparatur für Punkteausschüttung


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  fRepairing = false;
  iAutorepairWait = 80*20;
  iLastAttacker = -1;
  destructability = true;
  SetAction("Idle");
  return true;
}

/* Reparatur */

local iRejectRepairCounter;

public func RejectRepair()
{
  if(--iRejectRepairCounter < 0)
    iRejectRepairCounter = RepairSpeed();

  return iRejectRepairCounter;
}

public func StartRepair()
{
  ClearScheduleCall(this, "StartRepair");
  if(!fRepairing)
  {
    fRepairing = true;
    SetAction("RepairStart");
  }
}

public func Repair()
{ 
  //Jetzt gepanzert
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,6,this);
}

/* Automatischer Reparatureffekt */

public func FxIntRepairStart(object pTarget, int iEffectNumber, int iTemp)
{
  Sound("Repair.ogg",false,this,50,0,+1); 
  return 1;
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(GetDamage(pTarget) <= 0)
    return -1;

  DoDamage(-1, pTarget);

  if(!Random(2))
  {
    Sparks(2+Random(5), RGB(250,150,0), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
    Sparks(2+Random(5), RGB(100,100,250), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));
  }
}

public func FxIntRepairDamage(object pTarget, int iNr, int iDmgEngy)
{
  //Reparatur darf nicht in die Länge gezogen werden
  if(iDmgEngy > 0)
    return 0;

  return iDmgEngy;
}

public func FxIntRepairStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Sound("Repair.ogg",false,this,0,0,-1); 
  if(!iReason)
    ObjectSetAction(pTarget, "RepairStop");
}

public func StopRepair()
{
  fRepairing = false;
  fDestroyed = false;
  DoDamage(-GetDamage());
  OnRepair();
}

public func AutoRepair()
{
  if(this->AutoRepairWait() > 0) ScheduleCall(this, "StartRepair", this->AutoRepairWait());
}

public func SetAutoRepair(int iAuto)
{
  iAutorepairWait = iAuto;
}

/* Bei Reparaturfertigstellung */

public func OnRepairSound()	{return Sound("StructureConstructed*.ogg");}

public func OnRepair()
{
  var width = this->GetDefWidth();
  var height = this->GetDefHeight();

  //Effekte
  this->OnRepairSound();
  this->AddLightFlash(width+height*2, 0, 0, RGB(250,250,250));
  for(var i = -3; i < 3; i++)
    this->CreateParticle("MaterialSpark", RandomX(-width/2,width/2), RandomX(-height/2,height/2), 0,0, 5*5+Random(5*10));
}

/* Unstuck von Lebewesen */

public func ClearObjects(int x, int y, int xoff, int yoff)
{
  //Angegebenes Zielrechteck verwenden: Ansonsten Objektgröße
  var xArea, yArea, xOffset, yOffset;
  if(xoff)
  {
    xArea = x;
    yArea = y;
    xOffset = xoff;
    yOffset = yoff;
  }
  else
  {
    xArea = GetDefCoreVal("Width", "DefCore")+30;
    yArea = GetDefCoreVal("Height", "DefCore")+30;
    xOffset = (xArea/2)+15;
    yOffset = (yArea/2)+15;
  }

  var obj = FindObjects(Find_InRect(-xOffset,-yOffset,xArea,yArea), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Exclude(this));
  for(var target in obj)
  {
    AutoUnstuck(target);
  }
}

/* Unzerstörbarkeit */

public func SwitchDestructability()
{
  if(destructability)
    destructability = false;
  else
    destructability = true;

  //Objektspezifischer Aufruf
  OnSwitchDestructability();

  return true;
}

public func OnSwitchDestructability()	{return true;}

/* Schaden */

public func Damage(int change)
{
  if(change > 0)
  {
    //Struktur unzerstörbar: Schaden rückgängig machen
    if(!destructability)
      DoDamage(-change);
    //Struktur bereits zerstört: Maximalschaden nicht überschreitbar
    if(IsDestroyed())
    {
      if(GetDamage() > MaxDamage())
        DoDamage(-(GetDamage()-MaxDamage()));
    }
  }

  //Maximalschaden erreicht: Struktur zerstören
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
    Destroyed();
  }

  //Bei beendetem Reparaturvorgang Sonderfunktionen aufrufen
  if(this->~IsCMCStructure() && this->IsDestroyed() && this->GetDamage() == 0 && !this->IsRepairing())
  {
    LocalN("fDestroyed", this) = false;
    ObjectSetAction(this, "RepairStop");
    this->OnRepair();
  }
}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Default
}

public func OnHit(int iDmg, int iType, object pBy)
{
  if(!IsDestroyed())
    iLastAttacker = GetController(pBy);
}

/* Zerstörung */

public func Destroyed()
{
  //Punkte bei Belohnungssystem (Struktur zerstört)
  if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
      DoPlayerPoints(BonusPoints("StructureDestruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);

  //Status setzen
  SetAction("Destroyed");
  fDestroyed = true;

  //Schaden auf Maximalwert setzen
  if(GetDamage() > MaxDamage())
    DoDamage(-(GetDamage()-MaxDamage()));

  //Callback - kann ChangeDef aufrufen
  OnDestruction();

  if(this->~IsCMCStructure())
  {
    //Letzen Angreifer zurücksetzen
    LocalN("iLastAttacker", this) = -1;

    //Reparatur anordnen
    this->AutoRepair();
  }
}

public func OnRepairing()
{
  if(IsDestroyed() && !IsRepairing())
    StartRepair();

  return true;
}

public func IsFullyRepaired()
{
  RemoveEffect("IntRepair", this);
  return true;
}