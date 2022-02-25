/*-- Keine Szenario-Events --*/

#strict 2

global func NoScenarioEvents()	{return FindObject(NOSE);}
public func IsChooseable()	{return false;}	//Standardmäßig nicht wählbar


/* Initialisierung */

protected func Initialize()
{
  Execute();
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

public func ChooserFinished()
{
  Execute();
}

public func Execute()
{
  //Säulen suchen und färben
  for(var obj in FindObjects(Find_ID(PILR)))
    SetClrModulation(RGBa(100,100,100,5),obj);

  //Sendemasten unzerstörbar schalten
  for(var obj in FindObjects(Find_ID(AATR)))
    if(obj->~Destructable())
      obj->~SwitchDestructability();
}

/* Entfernung */

protected func Destruction()
{
  //Säulen suchen und färben
  for(var obj in FindObjects(Find_ID(PILR)))
    SetClrModulation(0,obj);

  //Sendemasten zerstörbar schalten
  for(var obj in FindObjects(Find_ID(AATR)))
    if(!obj->~Destructable())
      obj->~SwitchDestructability();

  return true;
}