/*-- Selbstmord --*/

#strict 2

public func IsChooseable()		{return 1;}	//Kann mittels des Spielzielauswählers ausgewählt werden
public func RejectRespawnTimer()	{return true;}	//Keine Spawnverzögerung

/* Initialisiert */

private func Initialized()
{
  //Verschwinden wenn Keine Schwerverletzten-Regel vorhanden
  if(FindObject(NOFD))
    RemoveObject();
}

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}