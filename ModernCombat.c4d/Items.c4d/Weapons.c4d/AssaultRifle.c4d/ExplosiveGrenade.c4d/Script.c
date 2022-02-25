/*-- Explosivgranate --*/

#strict 2
#include GREN

local active,sx,sy, start;
local iLastAttacker;
local iAttachment;

public func Color()		{return RGB(255,0,0);}	//Kennfarbe

public func BlastRadius()	{return 30;}		//Explosionsradius
protected func SecureDistance()	{return 75;}		//Sicherungsreichweite
public func ExplodeDelay()	{return 35*3;}		//Verzögerung bis zu automatischer Zündung
public func MaxDamage()		{return 1;}		//Maximalschaden bis Zerstörung

public func IgnoreTracer()	{return true;}		//Nicht markierbar
public func IsRifleGrenade()	{return true;}		//Ist eine Gewehrgranate
public func NoDecoDamage()	{return true;}		//Trifft Dekoobjekte nicht
public func IsSpawnTrap()	{return true;}
public func AllowHitboxCheck()	{return true;}
public func RejectC4Attach()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  iLastAttacker = NO_OWNER;
  return _inherited();
}

/* Start */

func Launch(int xdir, int ydir, int iDmg,a,b,c, int attachment, object pUser)
{
  iAttachment = attachment;
  SetCategory(C4D_Vehicle);
  active = true;
  sx = GetX();
  sy = GetY();
  start = FrameCounter();
  var ffx,ffy;
  pUser->~WeaponEnd(ffx,ffy);
  ffx += GetX(pUser);
  ffy += GetY(pUser);
  AddEffect("HitCheck", this, 1, 1, 0, SHT1,pUser,0,ffx,ffy);
  AddEffect("RifleGrenade", this, 1, 1, this);
  SetSpeed(xdir, ydir);  
  if(!iDmg)
    //Standardexplosion
    iDmg = 30;
  if(Stuck()) Hit();
}

protected func Secure()
{
  if(!active)
    return true;

  if(Distance(GetX(),GetY(),sx,sy) <= SecureDistance() && FrameCounter() < start+70)
    return true;

  return false;
}

/* Treffer */

func Hit()
{
  HitObject();
}

protected func RejectEntrance(pNewContainer)
{
  return 1;
}

func HitObject(object pObj)
{
  if(Secure())
  {
    if(pObj)
    {
      DoDmg(20, DMG_Projectile, pObj, 0, 0, 0, iAttachment);

      if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
      if(GetOCF(pObj) & OCF_Living)
      {
        Sound("SharpnelImpact*.ogg");
      }
      else
      {
        Sound("BulletHitMetal*.ogg");
        Sparks(30,RGB(255,128));
      }
      RemoveObject();
      return;
    }
    else
    {
      Sound("GrenadeHit*.ogg");
      Sparks(30,RGB(255,128));
      if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12, 10, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
      RemoveObject();
      return;
    }
  }
  if(Hostile(iLastAttacker, GetController()) && GetID() != ERND)
    //Punkte bei Belohnungssystem (Projektil abgefangen)
    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);

  Trigger(pObj);
}

func Trigger(object pObj)
{
  Explode(BlastRadius()*2/3);
  DamageObjects(BlastRadius()*3/2,BlastRadius()/2,this, 0, 0, iAttachment);
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  Sound("ShellExplosion*.ogg");
}

/* Timer-Effekt */

func FxRifleGrenadeTimer(object target, int effect, int time)
{
  //Flugzeit aufgebraucht: Explodieren
  if(time > ExplodeDelay()) return HitObject();

  var Xdir = GetXDir();
  var Ydir = GetYDir();
  var vel=Abs(Xdir)+Abs(Ydir);
  var alpha=Max(0,100-vel);

  //Raucheffekt
  if(Color() && !Secure())
  {
    var vel = Abs(Xdir)+Abs(Ydir);
    var alpha = Max(0,60-vel);

    if(!GBackLiquid())
      CreateParticle("Smoke2", -Xdir/6, -Ydir/6, RandomX(-10, 10), -5, vel/3+RandomX(10, 20), SetRGBaValue(Color(),alpha)); 
    else
      CastObjects(FXU1,2,6);
  }

  //In Bewegung: Entsprechende Rotation
  if(Xdir || Ydir)
    SetR(Angle (0,0,Xdir,Ydir));
}

/* Schaden */

func Damage()
{
  if(GetDamage() >= MaxDamage())
  {
    //Effekte
    Sparks(8,RGB(255,128));
    Sound("MISL_ShotDown.ogg");

    if(!Secure() && Hostile(iLastAttacker, GetController()))
    {
      //Punkte bei Belohnungssystem (Projektil abgefangen)
      DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, iLastAttacker, GetCursor(iLastAttacker), IC16);
    }

    Trigger();
  }
}

public func OnHit(a, b, pFrom)
{
  iLastAttacker = GetController(pFrom);
  return;
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 100;	//Feuer
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}