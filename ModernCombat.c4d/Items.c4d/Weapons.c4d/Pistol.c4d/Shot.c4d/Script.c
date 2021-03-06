/*-- Schuss --*/

#strict 2

local iTime,lx,ly,pTrail,iDamage,speed,max_dst,dst,fb,fNoTrail,iMaxHits,iHitReduction;
local shooter,wpnid,iAttachment;			//Sch?tze, Waffe und Waffenaufsatz
local startx, starty;

func NoWarp()			{return true;}
func IsBullet()			{return true;}
public func AllowHitboxCheck()	{return false;}


/* Initialisierung */

public func Initialize()
{
  SetObjectBlitMode(1);
}

func Construction(object byObj)
{
  //Kein Objekt: Verschwinden
  if(!byObj)
    return;

  //ID der Host-Waffe, Sch?tzen und Team ermitteln
  wpnid = GetID(byObj);
  shooter = GetShooter(byObj);
  if(shooter->GetTeam())
    SetTeam(shooter->GetTeam());
}

public func SetAngle(int iAngle, int iAPrec)
{
  if(!iAPrec) iAPrec = 1;
  SetXDir(+Sin(iAngle,speed,iAPrec));
  SetYDir(-Cos(iAngle,speed,iAPrec));
  SetR(iAngle/iAPrec,0);
}

/* Aufprall und Treffer */

private func HitLandscape()
{
  //Aufprall auf Material oder SolidMask
  var x,y;
  x = lx;
  y = ly;
  OnHitLandscape(x,y);
}

public func OnHitLandscape(int iX, int iY)
{
  var tmp = CreateObject(TIM1,iX,iY,-1);	//Sound-Objekt

  //Aufpralleffekte je nach Material
  var mat = GetMaterial(iX,iY);
  if(GBackSolid(iX,iY))
  {
    //Grabbares Material in kleiner Menge entfernen und verschleudern
    if(GetMaterialVal("DigFree", "Material", mat))
    {
      DigFree(GetX()+iX, GetY()+iY, 5);
      CastPXS(MaterialName(mat), 20, 40, iX, iY);

      //Effekte bei grabbaren Material
      Sound("BulletHitSoft*.ogg", 0, tmp);
      Sound("Crumble*.ogg", 0, tmp, RandomX(30,60));
    }
    else
    {
      //Effekte bei festem Material
      Sound("BulletHit*.ogg", 0, tmp);
      Sound("Crumble*.ogg", 0, tmp, RandomX(40,70));
      var iAngle, iSpeed, iSize;
      for(var i = 0; i < 3 * GetCon()/10; ++i)
      {
        iAngle = GetR() - 200 + Random(41);
        iSpeed = Random(speed / 9 + 1);
        iSize = Random(31) + 10;
        CreateParticle("Frazzle", iX + Sin(iAngle, 2), iY - Cos(iAngle, 2), Sin(iAngle, iSpeed), -Cos(iAngle, iSpeed), iSize, GlowColor(iTime));
      }
    }
  }

  //Allgemeine Effekte
  var rgb = 0;
  var rand = Random(3);
  rgb = RGB(GetMaterialColor(mat,rand,0),
  	GetMaterialColor(mat,rand,1),
  	GetMaterialColor(mat,rand,2));
  if(rgb)
    SmokeBurst(BoundBy(10+iDamage,10,25),iX,iY,GetR()-180,0,rgb);

  //Sound-Objekt entfernen
  RemoveObject(tmp);
}

public func HitObject(object pObject)
{
  //Sch?tze kann sich nicht selbst treffen
  if(shooter && pObject)
    if(pObject == shooter)
      return false;

  //Schaden verursachen und treffen
  if(BulletStrike(pObject))
  {
    var x,y;
    x = lx;
    y = ly;
    OnBulletHit(pObject,x,y);
    return true;
  }
}

public func OnBulletHit(object pObject, int iX, int iY)
{
  //Getroffenes Objekt vorhanden?
  if(pObject)
  {
    //Allgemeines Trefferger?usch
    var tmp = CreateObject(TIM1, iX, iY, NO_OWNER);	//Sound-Objekt
    Sound("BulletImpact*.ogg", 0, tmp);

    //Bei Feind: Trefferger?usch und Fadenkreuzindikator f?r Sch?tzen
    if(Hostile(GetOwner(), GetOwner(pObject)) && !pObject->~NoHitIndicator())
    {
      Sound("BulletHitTargetIndicator*.ogg", 0, shooter, 0, GetOwner(shooter)+1,-1);
      Sound("BulletHitTargetIndicator*.ogg", 0, shooter, 0, GetOwner(shooter)+1);

      var pPlayerClonk;
      pPlayerClonk = GetCursor(GetOwner());
      pPlayerClonk->SetCrosshairIndicator(255);
    }

    //Bei Lebewesen: Trefferger?usch f?r dessen Besitzer
    if(GetOCF(pObject) & OCF_Living)
    {
      if((GetOCF(pObject) & OCF_CrewMember) && (GetOwner() != NO_OWNER))
        Sound("BulletHitIndicator*.ogg", 0, tmp, 0, GetOwner(pObject)+1);

      RemoveObject(tmp);
      //Keine weiteren Effekte
      return;
    }
    RemoveObject(tmp);
  }
  else
    return;

  //Allgemeine Effekte
  var iAngle, iSpeed, iSize;
  for(var i = 0; i < 3 * GetCon()/10; ++i)
  {
    iAngle = GetR() - 200 + Random(41);
    iSpeed = Random(speed / 9 + 1);
    iSize = Random(31) + 10;
    CreateParticle("Frazzle", iX + Sin(iAngle, 2), iY - Cos(iAngle, 2), Sin(iAngle, iSpeed), -Cos(iAngle, iSpeed), iSize, GlowColor(iTime));
  }
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  CreateParticle("Flare", iX, iY, 0, 0, 100*sizemod/100, GlowColor(iTime));
}

public func BulletStrike(object pObj)
{
  //Schaden f?r getroffenes Objekt errechnen
  if(pObj)
    if(iAttachment != AT_Silencer)
      DoDmg(iDamage, DMG_Projectile, pObj, 0, 0, 0, iAttachment);
    else
    {
      //Schadensreduzierung auf Entfernung bei schallged?mpften Waffen
      var x = 9;
      var stepsize = 75;
      var maxsteps = 6;
      var step = BoundBy((Distance(GetX(), GetY(), GetX(pObj), GetY(pObj)) / stepsize), 1, maxsteps) - 1;
      var newDamage = iDamage - ((iDamage * step) / x);
      //Message(Format("%d red: %d step: %d dist: %d", newDamage, ((iDamage * step) / x), step, Distance(GetX(), GetY(), GetX(pObj), GetY(pObj))), pObj);
      DoDmg(newDamage, DMG_Projectile, pObj, 0, 0, 0, iAttachment);
    }

  return true;
}

public func GlowColor(int iATime)
{
  return RGBa(255,190,0,50);
}

func HitExclude()	{}

public func Remove(int iRemoveTime)
{
  if(pTrail)
  {
    SetPosition(GetX(),GetY(),pTrail);
    pTrail->Remove();
  }

  if(!GetEffect("Remove",this))
  {
    BulletParticle();
    //SetVisibility(VIS_None);
    SetXDir();
    SetYDir();
    SetClrModulation(RGBa(255,255,255,255));
    AddEffect("Remove",this,1,Max(1,iRemoveTime),0,GetID());
  }
}

/* Trefferpr?fung */

private func HitCheck(int r, int d)
{
  //L?nge berechnen
  var mx = +Sin(r,100),
      my = -Cos(r,100),
      ml = d;
  var sx = GetX(),
      sy = GetY();

  var g = GetGravity();
  SetGravity(0);
  var x = sx, y = sy, xdir = mx, ydir = my, fLandscape;
  fLandscape = SimFlight(x, y, xdir, ydir, 0, 0, d/10);

  var liqX = sx, liqY = sy, liqXdir = mx, liqYdir = my, fLiquid;
  fLiquid = SimFlight(liqX, liqY, liqXdir, liqYdir, 25, 49, d/10);
  SetGravity(g);

  if(x == sx && y == sy)
  {
    x += Sin(r, d);
    y -= Cos(r, d);
  }

  mx = AbsX(x);//+Sin(r,ml);
  my = AbsY(y);//-Cos(r,ml);

  var aExcludes = [2, [5, shooter], [5, this]];

  for(var i = 0; i < iMaxHits; i++)
  {
    var pObj = FindObject2(Find_OnLine(0, 0, mx, my), 
    Find_NoContainer(), 
    Find_Or(Find_Func("IsBulletTarget", GetID(), this, shooter), Find_OCF(OCF_Alive)),
    Find_Func("CheckEnemy", 0, shooter, 0, startx, starty),
    Find_Not(Find_Func("HitExclude", this)),
    aExcludes,
    Find_Or(Find_Not(Find_Func("UseOwnHitbox")), Find_Func("BulletHitboxCheck", sx, sy, x, y)),
    Sort_Func("SHTX_SortDistance", sx, sy, x, y, r));
    if(pObj)
    {
      var stretch;
      if(pObj->~UseOwnHitbox() && (stretch = pObj->BulletHitboxStretch(sx, sy, x, y))[0] > 0 && stretch[1] != 4)
      {
        if(stretch[1] == 1)
        {
          lx = 0;
          ly = 0;
        }
        else
        {
          var dist = Distance(sx, sy, x, y);
          lx = Sin(r, (dist * stretch[0]) / 1000);
          ly = -Cos(r, (dist * stretch[0]) / 1000);
        }
      }
      else
      {
        //Bei eigener Hitbox eigenen Bulletcheck ausf?hren (z.B. bei Helikoptern)
        var ox = GetX(pObj), oy = GetY(pObj);

        //Objektwerte verwenden statt Definitionswerte (bspw. f?r Assaulttargets)
        if(pObj->~BulletCheckObjectHitbox())
        {
          var xLeft = GetObjectVal("Offset", 0, pObj, 0) + ox;
          var xRight = GetObjectVal("Width", 0, pObj) + GetObjectVal("Offset", 0, pObj, 0) + ox;

          var yUp = GetObjectVal("Offset", 0, pObj, 1) + oy;
          var yDown = GetObjectVal("Height", 0, pObj) + GetObjectVal("Offset", 0, pObj, 1) + oy;
        }
        else if(pObj->~UseOwnHitbox())
        {
          var xLeft = -pObj->HitboxWidth()/2 + ox + 25*pObj->~IsSmoking();
          var xRight = pObj->HitboxWidth()/2 + ox - 25*pObj->~IsSmoking();

          var yUp = -pObj->HitboxHeight()/2 + oy + 25*pObj->~IsSmoking();
          var yDown = pObj->HitboxHeight()/2 + oy - 25*pObj->~IsSmoking();
        }
        else
        {
          var xLeft = GetDefCoreVal("Offset", "DefCore", GetID(pObj), 0) + ox;
          var xRight = GetDefCoreVal("Width", "DefCore", GetID(pObj)) + GetDefCoreVal("Offset", "DefCore", GetID(pObj), 0) + ox;

          var yUp = GetDefCoreVal("Offset", "DefCore", GetID(pObj), 1) + oy;
          var yDown = GetDefCoreVal("Height", "DefCore", GetID(pObj)) + GetDefCoreVal("Offset", "DefCore", GetID(pObj), 1) + oy;
        }

        if(!(Inside(sx, xLeft, xRight) && Inside(sy, yUp, yDown)))
        {
          var xOff, yOff;

          if(sx > ox)
            xOff = xRight;
          else
            xOff = xLeft;

          if(sy > oy)
            yOff = yDown;
          else
            yOff = yUp;

          var a = Angle(sx, sy, xOff, yOff);
          if(Inside(sx, Min(ox, xOff), Max(ox, xOff)))
          {
            var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
            lx = Sin(r, temp);
            ly = -Cos(r, temp);
          }
          else if(Inside(sy, Min(oy, yOff), Max(oy, yOff)))
          {
            var temp = (xOff - sx) * 1000 / (Sin(r, 1000));
            lx = Sin(r, temp);
            ly = -Cos(r, temp);
          }
          else
          {
            var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
            var lxOne = Sin(r, temp);
            var lyOne = -Cos(r, temp);

            temp = (xOff - sx) * 1000 / (Sin(r, 1000));
            var lxTwo = Sin(r, temp);
            var lyTwo = -Cos(r, temp);

            //Wenn erster Punkt weiter weg, diesen w?hlen, sonst den anderen
            if(Distance(lxOne, lyOne) > Distance(lxTwo, lyTwo))
            {
              lx = lxOne;
              ly = lyOne;
            }
            else
            {
              lx = lxTwo;
              ly = lyTwo;
            }
          }
        }
      }
      var fAlive = GetOCF(pObj) & OCF_Alive;
      var iObj = GetID(pObj);
      if(HitObject(pObj))
      {
        if(pObj && Hostile(GetOwner(), GetOwner(pObj)))
        {
          //Achievement-Fortschritt (Bullet Hell)
          DoAchievementProgress(1, AC59, GetOwner());

          if(i > 0 && GetID(Contained(pObj)) == FKDT)
            //Achievement-Fortschritt (Didn't see it coming)
            AwardAchievement(AC54, GetOwner());
        }

        var dist = Distance(sx, sy, ox, oy);
        dst += dist;

        if((!fAlive) || i == (iMaxHits - 1) || iDamage == 0)
        {
          if(fLiquid && Distance(sx, sy, GetX()+lx, GetY()+ly) > Distance(sx, sy, liqX, liqY)+5)
            HitLiquid(sx, sy, liqX, liqY);

          Remove();
          return dist;
        }
        else
        {
          if(pObj)
            aExcludes[GetLength(aExcludes)] = [5, pObj];
          //Log("Before %d", iDamage);
          iDamage = (iDamage * (100 - iHitReduction)) / 100;
          //Log("After %d", iDamage);
        }
      }
    }
  }

  lx = mx;
  ly = my;

  if(fLiquid && Distance(sx, sy, GetX()+mx, GetY()+my) > Distance(sx, sy, liqX, liqY))
    HitLiquid(sx, sy, liqX, liqY);

  if(fLandscape)
    HitLandscape(mx,my);

  ml = Distance(sx, sy, mx, my);

  dst += ml;
  return ml;
}

/* Aufprall auf Wasser */

public func HitLiquid(int iStartX, int iStartY, int iLiqX, int iLiqY)
{
  var angle = Angle(iStartX, iStartY, iLiqX, iLiqY);
  var x = AbsX(iLiqX), y = AbsY(iLiqY);
  var temp = CreateObject(TIM1, x, y, NO_OWNER);	//Hilfsobjekt f?r Effekte
  var mat = GetMaterial(x, y+1);

  //Effekte
  Sound("BulletHitWater*.ogg", false, temp, 50);
  CreateParticle("Splash", x, y, +Sin(angle, 500), -Cos(angle, 500), 180, RGB(GetMaterialColor(mat), GetMaterialColor(mat, 0, 1), GetMaterialColor(mat, 0, 2)), temp);
  Splash(x, y+1, 15);
  for(var i = 0; i < 5; i++)
  {
    var bubble = CreateObject(FXU1, x, y+2, NO_OWNER);
    if(!bubble)
      break;

    bubble->SetXDir(+Sin(angle+Random(30)-15, 100));
    bubble->SetYDir(-Cos(angle+Random(30)-15, 100));
  }
}

public func GetKillIcon()
{
  return wpnid;
}

public func BulletParticle()
{
  //Waffe kann Effekte unterdr?cken
  if(fNoTrail) return;

  //Distanz zu Aufprallort ermitteln
  var l = Distance(lx,ly)-32;
  //Bei zu kurzer Entfernung abbrechen
  if(l < 64) return;
  //Position des Effekts ermitteln
  var p = 20+Random(l-70);
  var s = 30+Random(10);
  //Partikel erstellen
  CreateParticle("BulletTail",
  		lx*p/l,
  		ly*p/l,
  		+Sin(GetR(),s)*4,
  		-Cos(GetR(),s)*4,
  		(RandomX(200,300)*GetCon()/100)*5,GlowColor(iTime));
}

global func GetShooter(object weapon)
{
  var shooter;

  if(!weapon) return;

  //Bei Waffen den Sch?tzen ermitteln
  if(weapon->~IsWeapon())
  {
    shooter = weapon->GetUser();
    //Sch?tze ist irgendwo drin
    if(Contained(shooter))
      shooter = Contained(shooter);
    //oder fasst was an
    if(shooter)
      if(GetActionTarget(0, shooter) && (GetProcedure(shooter) == "PUSH"))
        shooter = GetActionTarget();
  }

  //Kein Sch?tze identifiziert: Stattdessen Waffe nehmen
  if(!shooter)
    shooter = weapon;
      return shooter;
}

/* Angepasste Schuss-Funktion */

public func CustomLaunch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int Attachment, int iMultiHit, int iMultiHitReduce)
{
  fNoTrail = (iTrail == -1);
  iAttachment = Attachment;

  //Schaden der Kugel setzen
  if(!iDmg)
    iDamage = 3;
  else
    iDamage = iDmg;

  if(iMultiHit > 1)
    iMaxHits = iMultiHit;
  else
    iMaxHits = 1;

  iHitReduction = iMultiHitReduce;

  //Position setzen
  SetPosition(GetX(),GetY()+GetDefHeight()/2);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(iAngle);
  DoCon(100*iSize/GetDefWidth()-100);

  //M?ndungsposition f?r Schussmechanik speichern
  startx = GetX();
  starty = GetY();

  SetAction("Travel");

  max_dst = iDist;
  speed = iSpeed;
  iTime = 1;
  fb = true;

  HitCheck(iAngle,iDist);

  Remove(iRemoveTime);
}

global func SHTX_SortDistance(int sx, int sy, int x, int y, int r)
{
  //Bestimmung des Aufprallpunktes
  //Helikopter-Hitbox m?glichst mitbeachten; Anschlie?end wird die Distance von Kugelstart bis zum Aufprallpunkt zur?ckgegeben und in so_data[index][1] gespeichert; so_data und so_objects werden auch noch in den lokalen Variablen gespeichert
  var stretch, lx, ly;
  if(this->~UseOwnHitbox() && (stretch = this->BulletHitboxStretch(sx, sy, x, y))[0] > 0 && stretch[1] != 4)
  {
    if(stretch[1] == 1)
    {
      lx = 0;
      ly = 0;
    }
    else
    {
      var dist = Distance(sx, sy, x, y);
      lx = Sin(r, (dist * stretch[0]) / 1000);
      ly = -Cos(r, (dist * stretch[0]) / 1000);
    }
  }
  else
  {
    //Bei eigener Hitbox eigenen Bulletcheck ausf?hren (z.B. bei Helikoptern)
    var ox = GetX(this), oy = GetY(this);

    //Objektwerte verwenden statt Definitionswerte (z.B. bei Assault-Zielen)
    if(this->~BulletCheckObjectHitbox())
    {
      var xLeft = GetObjectVal("Offset", 0, this, 0) + ox;
      var xRight = GetObjectVal("Width", 0, this) + GetObjectVal("Offset", 0, this, 0) + ox;

      var yUp = GetObjectVal("Offset", 0, this, 1) + oy;
      var yDown = GetObjectVal("Height", 0, this) + GetObjectVal("Offset", 0, this, 1) + oy;
    }
    else if(this->~UseOwnHitbox())
    {
      var xLeft = -this->HitboxWidth()/2 + ox + 25*this->~IsSmoking();
      var xRight = this->HitboxWidth()/2 + ox - 25*this->~IsSmoking();

      var yUp = -this->HitboxHeight()/2 + oy + 25*this->~IsSmoking();
      var yDown = this->HitboxHeight()/2 + oy - 25*this->~IsSmoking();
    }
    else
    {
      var xLeft = GetDefCoreVal("Offset", "DefCore", GetID(this), 0) + ox;
      var xRight = GetDefCoreVal("Width", "DefCore", GetID(this)) + GetDefCoreVal("Offset", "DefCore", GetID(this), 0) + ox;

      var yUp = GetDefCoreVal("Offset", "DefCore", GetID(this), 1) + oy;
      var yDown = GetDefCoreVal("Height", "DefCore", GetID(this)) + GetDefCoreVal("Offset", "DefCore", GetID(this), 1) + oy;
    }

    if(!(Inside(sx, xLeft, xRight) && Inside(sy, yUp, yDown)))
    {
      var xOff, yOff;

      if(sx > ox)
        xOff = xRight;
      else
        xOff = xLeft;

      if(sy > oy)
        yOff = yDown;
      else
        yOff = yUp;

      var a = Angle(sx, sy, xOff, yOff);
      if(Inside(sx, Min(ox, xOff), Max(ox, xOff)))
      {
        var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
        lx = Sin(r, temp);
        ly = -Cos(r, temp);
      }
      else if(Inside(sy, Min(oy, yOff), Max(oy, yOff)))
      {
        var temp = (xOff - sx) * 1000 / (Sin(r, 1000));
        lx = Sin(r, temp);
        ly = -Cos(r, temp);
      }
      else
      {
        var temp = (yOff - sy) * 1000 / (-Cos(r, 1000));
        var lxOne = Sin(r, temp);
        var lyOne = -Cos(r, temp);

        temp = (xOff - sx) * 1000 / (Sin(r, 1000));
        var lxTwo = Sin(r, temp);
        var lyTwo = -Cos(r, temp);

        //Wenn erster Punkt weiter weg, diesen w?hlen, sonst den anderen
        if(Distance(lxOne, lyOne) > Distance(lxTwo, lyTwo))
        {
          lx = lxOne;
          ly = lyOne;
        }
        else
        {
          lx = lxTwo;
          ly = lyTwo;
        }
      }
    }
  }
  return Distance(lx, ly);
}

/* Ungenutzte Funktionen */

/* Kugel-Abschuss (langsame Kugeln) */

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int iMultiHit, int iMultiHitReduce)
{
  return LaunchFB(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime,iMultiHit,iMultiHitReduce,...);
}

public func LaunchFB(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int iMultiHit, int iMultiHitReduce)
{
  //Schaden der Kugel setzen
  if(!iDmg)
    iDamage = 3;
  else
    iDamage = iDmg;

  if(iMultiHit > 1)
    iMaxHits = iMultiHit;
  else
    iMaxHits = 1;

  iHitReduction = iMultiHitReduce;

  //Position setzen
  SetPosition(GetX(),GetY()+GetDefHeight()/2);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(iAngle);
  DoCon(100*iSize/GetDefWidth()-100);

  //M?ndungsposition f?r Schussmechanik speichern
  startx = GetX();
  starty = GetY();

  SetAction("Travel");

  max_dst = iDist;
  speed = iSpeed;
  iTime = 1;
  fb = true;

  HitCheck(iAngle,iDist);

  Remove(iRemoveTime);
}

private func CreateTrail(int iSize, int iTrail)
{
  if(iTrail < 0)
    return;

  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this);
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Querschl?ger */

public func RicochetAngle()	//maximaler Abprallwinkel
{
  if(GetID() != SHTX) return;
  return 50;
}

public func OffsetX(int iA, int iB)
{
  var off = Offset(iA,iB);
  if(off > 180)
  {
    var da = -iA;
    var db = -iB;

    if(iA > 180)
      da = 360-iA;

    if(iB > 180)
      db = 360-iB;

    iA = 180+db;
    iB = 180+da;

    off = Offset(iA,iB);
  }
  return off;
}

public func Offset(int iA, int iB)
{
  if(iA > iB)
    return iA-iB;
  else
    return iB-iA;
}

public func Flip(int iA, int iB)
{
  var off = OffsetX(iA,iB);
  if(iA > iB)
    return iA+off;
  return iA-off;
}

public func Ricochet(int iX, int iY)
{
  if(GBackSolid()) return;
  if(iDamage <= 1) return;

  var I = Wrap4K(GetR(),0,360);
  var A = SurfaceNormal4K(iX,iY,2);
  var O = Wrap4K(Flip(A,I-180),0,360);
  var H = Wrap4K(OffsetX(O,I),0,360);

  /*Log("In:%d] Surface:%d] Abprallwinkel:%d] Reflektionswinkel:%d]",I,A,H,O);

  DrawParticleLine ("PSpark",0,0,Sin(I,100),-Cos(I,100),4,30,RGB(0,255),RGB(0,255));
  DrawParticleLine ("PSpark",0,0,Sin(A,100),-Cos(A,100),2,20,RGB(255),RGB(255));
  DrawParticleLine ("PSpark",0,0,Sin(O,100),-Cos(O,100),4,30,RGB(0,128,255),RGB(0,128,255));
  DrawParticleLine ("PSpark",0,0,Sin(H,100),-Cos(H,100),2,20,RGB(0,128,255),RGB(255,255,255));*/

  if(H <= RicochetAngle())//Winkel okay?
  {
    var O = A+(A-I)+180;
    iDamage = iDamage-iDamage*(H*50/RicochetAngle())/100;//Maximal 50% abziehen.
    Sound("Ricochet*.ogg");

    HitCheck(O,max_dst-dst);

    return true;
  }
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func TrailColor(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2);
}

private func NotZero(int a)
{
  if(!a) return 1;
  else return a;
}

public func FxRemoveTimer(object target, int effect, int time)
{
  RemoveObject(target);
  return -1;
}

public func CustomBulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor)
{
  return BulletCasing(iX,iY,iXDir,iYDir,iSize,iColor);
}

public func FMMod(int iType,Data)
{
  return Data;
}