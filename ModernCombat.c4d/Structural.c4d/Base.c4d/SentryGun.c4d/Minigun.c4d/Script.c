/*-- Minigun --*/

#strict 2

#include WEPN

local fNoSpread;

public func HandSize()		{return 1000;}
public func HandX()		{return 7000;}
public func HandY()		{return 0;}
public func NoWeaponChoice()	{return true;}


/* Kugeln - Dauerfeuer */

public func FMData1(int data)
{
  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 9999;	//Magazingr??e

  if(data == FM_Reload)		return 2;	//Zeit f?r Nachladen
  if(data == FM_Recharge)	return 5;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return 1;	//Automatikfeuer

  if(data == FM_Damage)		return 7;	//Schadenswert

  return Default(data);
}

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle()+(!fNoSpread)*RandomX(-3,+3);
  var dir = GetDir(user)*2-1;
  var iX;
  var iY;
  user->WeaponEnd(iX,iY);

  //Kugel abfeuern
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  ammo->CustomLaunch(angle, 350, 650, 2, GetFMData(FM_Damage) * 10, GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(40+Random(60),user,iX,iY,angle);
  BulletCasing(iX/3,iY/3,-dir*Cos(angle-35*dir,35+Random(10)),-dir*Sin(angle-35*dir,35+Random(10)),7);
  Sound("MISA_Fire.ogg",0,ammo);
  Echo("ACCN_Echo.ogg");
}

/* Allgemein */

func OnAutoStop(int iFM)
{
  Sound("MISA_FireEnd.ogg");
}