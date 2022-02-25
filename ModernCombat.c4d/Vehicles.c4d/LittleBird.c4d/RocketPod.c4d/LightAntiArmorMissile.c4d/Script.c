/*-- Leichte Panzerabwehrrakete --*/

#strict 2
#include MISL

public func TracerCompatible()	{return false;}

public func StartSpeed()	{return 1;}			//Startgeschwindigkeit
public func Acceleration()	{return 5+!fGuided*2;}		//Beschleunigung
public func MaxSpeed()		{return 120+!fGuided*30;}	//Maximale Geschwindigkeit

public func MaxDamage()		{return 20;}			//Maximalschaden bis Absturz

public func ExplosionDamage()	{return 20;}			//Explosionsschaden
public func ExplosionRadius()	{return 20;}			//Radius

public func MaxTurn()		{return 5;}			//max. Drehung