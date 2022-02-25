/*-- Doppelflinte --*/

#strict 2
#include WPN2

local casings;

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return -500;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 36;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_Bayonet | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;					//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20 + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 40 + (iAttachment == AT_Bayonet)*10;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;					//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;					//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Pellets$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 12;	//Magazingröße
  if(data == FM_AmmoUsage)	return 6;	//Munition pro Schuss

  if(data == FM_SingleReload)	return 80;	//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_PrepareReload)	return 30;	//Zeit bevor das eigentliche Nachladen beginnt
  if(data == FM_FinishReload)	return 45;	//Zeit nach dem Nachladen

  if(data == FM_Reload)		return 75;	//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_Recharge)	return 10;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 35;	//Schadenswert

  if(data == FM_SpreadAdd)	return 200;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 320;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 570;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 150;	//Minimal mögliche Streuung

  if(data == FM_MultiHit)	return 4;	//Anzahl möglicher Treffer pro Kugel
  if(data == FM_MultiHitReduce)	return 35;	//Schadensreduzierung pro Treffer

  return Default(data);
}

/* Kugeln - Streuschuss */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$SpreadShot$";
  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 100;
  if(data == BOT_Power)		return(BOT_Power_3);
  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugeln abfeuern
  var ammo;
  var j = GetFMData(FM_Damage, 1)/6;
  for(var i; i < j; i++)
  {
    angle = user->AimAngle(10,0,true);

    ammo = SALaunchBullet(x,y,GetController(user),angle,250+Random(20),250,10);
  }

  //Effekte
  MuzzleFlash(RandomX(50,65),user,x,y,angle,0, 5);
  Sound("DBSG_Fire*.ogg", 0, ammo);
  Sound("PPGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1,50);
  Echo("DBSG_Echo.ogg");

  //Patronenhülse hinzufügen
  casings++;
}

/* Nachladen */

func OnReload()
{
  Sound("DBSG_ReloadStart.ogg");
}

func OnFinishReloadStart()
{
  Sound("DBSG_ReloadStop.ogg");
}

func OnSingleReloadStart()
{
  if(casings)
    Sound("DBSG_Reload2.ogg");
  Sound("DBSG_Reload.ogg");
}

func OnSingleReloadStop()
{
  if(casings)
  {
    //Patrone auswerfen
    var user = GetUser();
    var dir = GetDir(user)*2-1;
    BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255,0,0));
    casings--;
  }
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func MaxReloadRotation()		{return -20;}

/* Allgemein */

func OnSelect()
{
  Sound("MNGN_Charge.ogg");
}