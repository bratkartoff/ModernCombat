/*-- Monitor --*/

#strict 2

local pLight;


/* Initialisierung */

protected func Initialize()
{
  SetAction("Activity1");

  //Licht erstellen
  pLight = AddLightAmbience(15);
  return 1;
}

public func On()
{
  //Licht erstellen
  if(pLight)
    pLight->TurnOn();

  SetAction("Activity1");
}

public func Off()
{
  //Licht entfernen
  if(pLight)
    pLight->TurnOff();

  SetGraphics(0,0,0,1);
  return(SetAction("Off"));
}

private func ChooseActivity()
{
  //Zuf?llige n?chste Aktion w?hlen
  var iAction = Random(5)+1;
  SetAction(Format("Activity%d", iAction));
}

/* Schaden */

protected func Damage()
{
  if(GetAction() == "Broken") return;

  //Effekte
  CastParticles("Glas",3+Random(3),80,0,0,20,80);
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  Sound("Blast1");

  //Licht entfernen
  if(pLight) RemoveObject(pLight);

  //Umherfliegen
  SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
  SetRDir(GetXDir()*2);

  //Zerst?rt
  SetAction("Broken");

  //Verschwinden
  FadeOut();
}

public func IsBulletTarget(id def)
{
  if(GetAction() == "Broken")	return false;
  if(def->~NoDecoDamage())	return false;
  return true;
}

/* Aufschlag */ 

protected func Hit()
{
  Sound("MetalHit*");
  return 1;
}

protected func Hit3()
{
  Damage();
}