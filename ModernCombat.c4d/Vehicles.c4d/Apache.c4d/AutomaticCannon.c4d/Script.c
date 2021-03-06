/*-- Maschinenkanone --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 600;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return GRAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 12;	//Magazingr??e

  if(data == FM_Reload)		return 100;	//Zeit f?r Nachladen
  if(data == FM_Recharge)	return 6;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;	//Automatikfeuer

  if(data == FM_Damage)		return 9;	//Schadenswert

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 25;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

/* Kugeln - Automatikfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Auto$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchGrenade(ERND, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;

  return Default(data);
}

/* Granaten - Explosivgranaten */

public func LaunchGrenade(id idg, int speed, int angle, int mode)
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var x,y;
  user->WeaponEnd(x,y);

  //Anpassung des Winkels
  angle = BoundBy(angle/*+GetYDir(user)*/+dir,-360,360);
  //Geschwindigkeit einstellen
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  //Granate abfeuern
  var grenade=CreateObject(idg, x, y, GetController(user));
  if(!Stuck(grenade)) SetPosition(GetX(grenade)+xdir/10,GetY(grenade)+ydir/10,grenade);
  SetController(GetController(user), grenade);
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2), 0, 0, 0, 0, user);

  //Sicht auf Granate wenn der Sch?tze zielt
  if(!(user->~IsMachine()) && user->~IsAiming())
  {
    SetPlrView(GetController(user),grenade);
    SetPlrViewRange(150, grenade);
  }

  //Effekte
  MuzzleFlash(RandomX(35,50),user,x,y,angle,0, 0);
  BulletCasing(x/3,y/3,-dir*Cos(angle-35*dir,20+Random(10)),-dir*Sin(angle-35*dir,20+Random(10)),7);
  Sound("LCAC_Fire*.ogg", 0, grenade);
  Echo("LCAC_Echo.ogg");

  //Klickger?usch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 0, 1))
    Sound("LCAC_Empty.ogg", 0, this, 0, GetOwner(user)+1);
  else
    if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
      Sound("MNGN_Click.ogg", 0, grenade, 0, GetOwner(user)+1);
}

/* Nachladen */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("LCAC_Reload.ogg", false, this);
}