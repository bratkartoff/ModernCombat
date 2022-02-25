/*-- Keine Szenario-Events --*/

#strict 2

global func NoScenarioEvents()	{return FindObject(NOSE);}
public func IsChooseable()	{return false;}	//Standardm��ig nicht w�hlbar


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
  //S�ulen suchen und f�rben
  for(var obj in FindObjects(Find_ID(PILR)))
    SetClrModulation(RGBa(100,100,100,5),obj);

  //Sendemasten unzerst�rbar schalten
  for(var obj in FindObjects(Find_ID(AATR)))
    if(obj->~Destructable())
      obj->~SwitchDestructability();
}

/* Entfernung */

protected func Destruction()
{
  //S�ulen suchen und f�rben
  for(var obj in FindObjects(Find_ID(PILR)))
    SetClrModulation(0,obj);

  //Sendemasten zerst�rbar schalten
  for(var obj in FindObjects(Find_ID(AATR)))
    if(!obj->~Destructable())
      obj->~SwitchDestructability();

  return true;
}