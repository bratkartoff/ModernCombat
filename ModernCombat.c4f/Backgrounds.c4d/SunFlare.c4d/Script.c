/*-- Sonnenlicht --*/

#strict 2

public func IsDeco()	{return true;}


/* Initialisierung */

public func Initialize()
{
  SetCategory(GetCategory() | C4D_Parallax | C4D_Background | C4D_MouseIgnore);
  Local()=100; Local(1)=100;
}