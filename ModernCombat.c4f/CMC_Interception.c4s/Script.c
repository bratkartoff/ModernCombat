/*-- Interception --*/

#strict 2
#include CSTD

static aFlag,aDoorWay,aFlagPosition;

func RecommendedGoals()	{return [GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Flaggen
  aFlag = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  if(C4X_VerBuild < 331) {SetNextMission("ModernCombat.c4f\\CMC_Interception.c4s", "$RepeatButton$", "$RepeatButtonDesc$");}
  return 1; 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-PlateYellow", 1935, 660, 1960, 700, 1935, 740, 1910, 700, 1);

  DrawMaterialQuad("Wall-PlateYellow", 1935, 750, 1960, 700, 2000, 710, 1970, 735, 1);
  DrawMaterialQuad("Wall-PlateYellow", 1935, 750, 1910, 700, 1870, 710, 1900, 735, 1);

  DrawMaterialQuad("Wall-PlateYellow", 1935, 755, 1970, 740, 1975, 780, 1975, 780, 1);
  DrawMaterialQuad("Wall-PlateYellow", 1935, 755, 1900, 740, 1895, 780, 1895, 780, 1);


  DrawMaterialQuad("Wall-Stripes", 790, 980, 800, 970, 830, 970, 840, 980, 1);
  DrawMaterialQuad("Wall-Stripes", 970, 1180, 960, 1180, 950, 1170, 960, 1170, 1);
  DrawMaterialQuad("Wall-Stripes", 1020, 1180, 1030, 1180, 1060, 1150, 1060, 1140, 1);
  DrawMaterialQuad("Wall-Plate", 1160, 700, 1210, 680, 1250, 690, 1200, 710, 1);
  DrawMaterialQuad("Wall-Plate", 1160, 770, 1156, 770, 1140, 750, 1160, 750, 1);
  DrawMaterialQuad("Wall-Unknown1", 1170, 800, 1210, 840, 1200, 850, 1170, 820, 1);
  DrawMaterialQuad("BackWall-Laboratory1", 1170, 1140, 1300, 1140, 1250, 1160, 1220, 1160, 1);

  DrawMaterialQuad("Wall-Unknown1", 1550, 850, 1560, 840, 1600, 850, 1570, 860, 1);
  DrawMaterialQuad("Wall-Concrete2", 1550, 1140, 1700, 1200, 1670, 1200, 1550, 1160, 1);
  DrawMaterialQuad("Wall-Unknown1", 1600, 550, 1620, 570, 1550, 570, 1570, 550, 1);
  DrawMaterialQuad("Wall-Stripes", 1810, 250, 1825, 261, 1810, 261, 1810, 250, 1);
  DrawMaterialQuad("Wall-Stripes", 1870, 1110, 1850, 1130, 1860, 1140, 1880, 1120, 1);

  DrawMaterialQuad("BackWall-Tiles1", 1820, 300, 1850, 300, 1660, 480, 1660, 450, 1);
  DrawMaterialQuad("Wall-Unknown1", 1790, 1210, 1850, 1160, 2020, 1160, 2080, 1210, 1);
  DrawMaterialQuad("BackWall-Laboratory1", 1810, 271, 1850, 266, 2020, 266, 2060, 271, 1);
  DrawMaterialQuad("Wall-Concrete2", 2320, 1140, 2170, 1200, 2200, 1200, 2320, 1160, 1);
  DrawMaterialQuad("BackWall-Tiles1", 2050, 300, 2020, 300, 2210, 480, 2210, 450, 1);

  DrawMaterialQuad("Wall-Stripes", 2000, 1110, 2020, 1130, 2010, 1140, 1990, 1120, 1);
  DrawMaterialQuad("Wall-Stripes", 2060, 250, 2045, 261, 2060, 261, 2060, 250, 1);
  DrawMaterialQuad("Wall-Unknown1", 2270, 550, 2250, 570, 2320, 570, 2300, 550, 1);
  DrawMaterialQuad("Wall-Unknown1", 2320, 850, 2310, 840, 2270, 850, 2300, 860, 1);

  DrawMaterialQuad("BackWall-Laboratory1", 2570, 1140, 2700, 1140, 2650, 1160, 2620, 1160, 1);
  DrawMaterialQuad("Wall-Unknown1", 2700, 800, 2660, 840, 2670, 850, 2700, 820, 1);
  DrawMaterialQuad("Wall-Plate", 2710, 770, 2714, 770, 2730, 750, 2710, 750, 1);
  DrawMaterialQuad("Wall-Plate", 2710, 700, 2660, 680, 2620, 690, 2670, 710, 1);
  DrawMaterialQuad("Wall-Stripes", 2850, 1180, 2840, 1180, 2810, 1150, 2810, 1140, 1);
  DrawMaterialQuad("Wall-Stripes", 2900, 1180, 2910, 1180, 2920, 1170, 2910, 1170, 1);
  DrawMaterialQuad("Wall-Stripes", 3080, 980, 3070, 970, 3040, 970, 3030, 980, 1);

  //Glasscheiben
  CreateObject(PAN2, 895, 1146, -1);
  CreateObject(PAN2, 925, 1146, -1);

  CreateObject(PANE, 1162, 851, -1);
  CreateObject(PANE, 1162, 880, -1);

  CreateObject(PANE, 1308, 851, -1);
  CreateObject(PANE, 1308, 880, -1);

  CreateObject(PANE, 1512, 542, -1);
  CreateObject(PANE, 1512, 571, -1);
  CreateObject(PANE, 1512, 600, -1);
  CreateObject(PANE, 1512, 920, -1);

  CreateObject(PANE, 1658, 542, -1);
  CreateObject(PANE, 1658, 571, -1);
  CreateObject(PANE, 1658, 600, -1);
  CreateObject(PANE, 1658, 920, -1);

  CreateObject(PANE, 2212, 542, -1);
  CreateObject(PANE, 2212, 571, -1);
  CreateObject(PANE, 2212, 600, -1);
  CreateObject(PANE, 2212, 920, -1);

  CreateObject(PANE, 2358, 542, -1);
  CreateObject(PANE, 2358, 571, -1);
  CreateObject(PANE, 2358, 600, -1);
  CreateObject(PANE, 2358, 920, -1);

  CreateObject(PANE, 2562, 851, -1);
  CreateObject(PANE, 2562, 880, -1);

  CreateObject(PANE, 2708, 851, -1);
  CreateObject(PANE, 2708, 880, -1);

  CreateObject(PAN2, 2945, 1146, -1);
  CreateObject(PAN2, 2975, 1146, -1);

  //Leitern
  CreateObject(LADR, 970, 1044, -1)->Set(7);
  CreateObject(LADR, 1100, 788, -1)->Set(5);
  CreateObject(LADR, 1135, 1133, -1)->Set(7);
  CreateObject(LADR, 1140, 952, -1)->Set(18);

  CreateObject(LADR, 1235, 787, -1)->Set(5);
  CreateObject(LADR, 1340, 1110, -1)->Set(4);
  CreateObject(LADR, 1360, 1068, -1)->Set(10);

  CreateObject(LADR, 1530, 544, -1)->Set(7);
  CreateObject(LADR, 1640, 544, -1)->Set(7);

  CreateObject(LADR, 1802, 1149, -1)->Set(17);
  CreateObject(LADR, 2230, 544, -1)->Set(7);
  CreateObject(LADR, 2340, 544, -1)->Set(7);
  CreateObject(LADR, 2068, 1149, -1)->Set(17);

  CreateObject(LADR, 2510, 1068, -1)->Set(10);
  CreateObject(LADR, 2530, 1110, -1)->Set(4);
  CreateObject(LADR, 2635, 787, -1)->Set(5);

  CreateObject(LADR, 2730, 952, -1)->Set(18);
  CreateObject(LADR, 2735, 1133, -1)->Set(7);
  CreateObject(LADR, 2770, 788, -1)->Set(5);
  CreateObject(LADR, 2900, 1044, -1)->Set(7);

  //Bodenluken
  CreateObject(HA4K, 970, 983, -1);
  CreateObject(HA4K, 1100, 743, -1);
  CreateObject(HA4K, 1140, 803, -1);
  CreateObject(HA4K, 1340, 1073, -1);
  CreateObject(HA4K, 1360, 983, -1);

  CreateObject(HA4K, 1530, 483, -1);
  CreateObject(HA4K, 1640, 483, -1);

  CreateObject(HA4K, 1860, 263, -1);
  CreateObject(HA4K, 2010, 263, -1);

  CreateObject(HA4K, 2230, 483, -1);
  CreateObject(HA4K, 2340, 483, -1);

  CreateObject(HA4K, 2510, 983, -1);
  CreateObject(HA4K, 2530, 1073, -1);
  CreateObject(HA4K, 2730, 803, -1);
  CreateObject(HA4K, 2770, 743, -1);
  CreateObject(HA4K, 2900, 983, -1);

  //Große Bodenluken
  CreateObject(H24K, 1135, 1078, -1);
  CreateObject(H24K, 1185, 988, -1);
  CreateObject(H24K, 1235, 748, -1);
  CreateObject(H24K, 1585, 608, -1);
  CreateObject(H24K, 1585, 808, -1);
  CreateObject(H24K, 1585, 888, -1);
  CreateObject(H24K, 2285, 608, -1);
  CreateObject(H24K, 2285, 808, -1);
  CreateObject(H24K, 2285, 888, -1);
  CreateObject(H24K, 2635, 748, -1);
  CreateObject(H24K, 2685, 988, -1);
  CreateObject(H24K, 2735, 1078, -1);

  //Gitter
  CreateObject(GTNG, 1413, 1140, -1);
  CreateObject(GTNG, 1548, 1140, -1);
  CreateObject(GTNG, 1853, 1160, -1);
  CreateObject(GTNG, 2018, 1160, -1);
  CreateObject(GTNG, 2323, 1140, -1);
  CreateObject(GTNG, 2458, 1140, -1);

  //Stahlbrücken
  //Neu
  //CreateObject(_HBB, 1235, 992, -1);
  CreateObject(_HBR, 1235, 992, -1);
  CreateObject(_HBR, 1695, 492, -1)->SwitchMode();

  CreateObject(_HBR, 1855, 302, -1)->SwitchMode();
  CreateObject(_HBR, 1935, 302, -1)->SwitchMode();
  CreateObject(_HBR, 2015, 302, -1)->SwitchMode();

  CreateObject(_HBR, 2175, 492, -1)->SwitchMode();
  //Neu
  //CreateObject(_HBB, 2635, 992, -1);
  CreateObject(_HBR, 2635, 992, -1);

  //Steine
  CreateObject(STNE, 1410, 1270, -1)->Set(1);
  CreateObject(STNE, 2435, 1320, -1)->Set(4);
  CreateObject(STNE, 2860, 1280, -1)->Set(2);

  //Lampen
  CreateObject(CLGH, 995, 1123, -1);
  CreateObject(CLGH, 1235, 1123, -1);
  CreateObject(LLGH, 1275, 845, -1);
  CreateObject(LLGH, 2595, 845, -1);
  CreateObject(CLGH, 2635, 1123, -1);
  CreateObject(CLGH, 2875, 1123, -1);

  //Paletten
  CreateObject(PLLT, 850, 1070, -1);
  CreateObject(PLLT, 3020, 1070, -1);

  //Gerüste
  CreateObject(SFFG, 680, 980, -1)->Set(4);
  CreateObject(SFFG, 750, 980, -1)->Set(4);
  CreateObject(SFFG, 880, 980, -1)->Set(4);
  CreateObject(SFFG, 950, 980, -1)->Set(4);

  CreateObject(SFFG, 2920, 980, -1)->Set(4);
  CreateObject(SFFG, 2990, 980, -1)->Set(4);
  CreateObject(SFFG, 3120, 980, -1)->Set(4);
  CreateObject(SFFG, 3190, 980, -1)->Set(4);

  //Metallkisten
  CreateObject(MWCR, 930, 980, -1)->AutoRespawn();
  CreateObject(MWCR, 2940, 980, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1710, 1210, -1)->AutoRespawn();
  CreateObject(BECR, 2160, 1210, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1360, 1140, -1)->AutoRespawn();
  CreateObject(PBRL, 2510, 1140, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1525, 880, -1)->AutoRespawn();
  CreateObject(PBRL, 1740, 1169, -1)->SetR(-90);
  CreateObject(PBRL, 2130, 1169, -1)->SetR(90);
  CreateObject(XBRL, 2345, 880, -1)->AutoRespawn();

  //Giftfässer
  CreateObject(TBRL, 940, 1170, -1)->AutoRespawn();
  CreateObject(TBRL, 2930, 1170, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1150, 1140, -1)->AutoRespawn();
  CreateObject(GSBL, 1300, 740, -1)->AutoRespawn();
  CreateObject(GSBL, 2570, 740, -1)->AutoRespawn();
  CreateObject(GSBL, 2720, 1140, -1)->AutoRespawn();

  //Schutztüren
  CreateObject(GDDR, 1165, 740, -1);
  CreateObject(GDDR, 1161, 800, -1);
  CreateObject(GDDR, 1275, 740, -1);

  CreateObject(GDDR, 1590, 480, -1);
  CreateObject(GDDR, 1467, 1110, -1);
  CreateObject(GDDR, 2403, 1110, -1);
  CreateObject(GDDR, 2280, 480, -1);

  CreateObject(GDDR, 2595, 740, -1);
  CreateObject(GDDR, 2705, 740, -1);
  CreateObject(GDDR, 2709, 800, -1);

  //Rohre
  CreateObject(PIPL, 715, 1150, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid();
  CreateObject(PIPL, 1120, 1120, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down();

  CreateObject(PIPL, 1785, 1239, -1)->Solid()->Right()->Solid()->Right()->Solid()->Up()->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid()->Right()->Solid();

  CreateObject(PIPL, 2750, 1120, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down();
  CreateObject(PIPL, 3155, 1150, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();

  //Sandsackbarrieren
  CreateObject(SBBA, 1000, 980, -1)->Set(1,1);

  CreateObject(SBBA, 1640, 1140, -1)->Set(0,1);
  CreateObject(SBBA, 1700, 1140, -1)->Set(1,1);

  CreateObject(SBBA, 2171, 1140, -1)->Set(0,1);
  CreateObject(SBBA, 2230, 1140, -1)->Set(1,1);

  CreateObject(SBBA, 2870, 980, -1)->Set(0,1);

  //Container
  CreateObject(CON1, 1010, 1071, -1)->SetGraphics("5");
  CreateObject(CON1, 1030, 1043, -1)->SetGraphics("5");
  CreateObject(CON1, 1050, 1071, -1)->SetGraphics("5");

  CreateObject(CON1, 1180, 1071, -1)->SetGraphics("5");
  CreateObject(CON1, 1200, 1043, -1)->SetGraphics("5");
  CreateObject(CON1, 1215, 1071, -1)->SetGraphics("5");

  CreateObject(CON1, 2655, 1071, -1)->SetGraphics("5");
  CreateObject(CON1, 2670, 1043, -1)->SetGraphics("5");
  CreateObject(CON1, 2690, 1071, -1)->SetGraphics("5");

  CreateObject(CON1, 2820, 1071, -1)->SetGraphics("5");
  CreateObject(CON1, 2840, 1043, -1)->SetGraphics("5");
  CreateObject(CON1, 2860, 1071, -1)->SetGraphics("5");

  //Phosphortanks
  CreateObject(PTNK, 1315, 740, -1)->AutoRespawn();
  CreateObject(PTNK, 1620, 420, -1)->AutoRespawn();
  CreateObject(PTNK, 1935, 1160, -1)->AutoRespawn();
  CreateObject(PTNK, 2250, 420, -1)->AutoRespawn();
  CreateObject(PTNK, 2555, 740, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 1380, 1140, -1)->AutoRespawn();
  CreateObject(XTNK, 910, 1070, -1)->AutoRespawn();
  CreateObject(XTNK, 2960, 1070, -1)->AutoRespawn();
  CreateObject(XTNK, 2490, 1140, -1)->AutoRespawn();

  //Autos
  CreateObject(CCAR, 100+Random(50), 980, -1);
  CreateObject(CCAR, 200+Random(50), 980, -1);
  CreateObject(CCAR, 300+Random(50), 980, -1);
  CreateObject(CCAR, 400+Random(50), 980, -1);
  CreateObject(CCAR, 500+Random(50), 980, -1);
  CreateObject(CCAR, 600, 980, -1);

  CreateObject(CCAR, 100+Random(50), 1070, -1);
  CreateObject(CCAR, 200+Random(50), 1070, -1);
  CreateObject(CCAR, 300+Random(50), 1070, -1);
  CreateObject(CCAR, 400+Random(50), 1070, -1);
  CreateObject(CCAR, 500+Random(50), 1070, -1);
  CreateObject(CCAR, 600, 1070, -1);

  CreateObject(CCAR, 1430, 1070, -1);
  CreateObject(CCAR, 2440, 1070, -1);

  CreateObject(CCAR, 3270, 980, -1);
  CreateObject(CCAR, 3370+Random(50), 980, -1);
  CreateObject(CCAR, 3470+Random(50), 980, -1);
  CreateObject(CCAR, 3570+Random(50), 980, -1);
  CreateObject(CCAR, 3670+Random(50), 980, -1);
  CreateObject(CCAR, 3770+Random(50), 980, -1);

  CreateObject(CCAR, 3270, 1070, -1);
  CreateObject(CCAR, 3370+Random(50), 1070, -1);
  CreateObject(CCAR, 3470+Random(50), 1070, -1);
  CreateObject(CCAR, 3570+Random(50), 1070, -1);
  CreateObject(CCAR, 3670+Random(50), 1070, -1);
  CreateObject(CCAR, 3770+Random(50), 1070, -1);

  //Grenzen
  CreateObject(BRDR, 600, 0, -1)->Set(0);
  CreateObject(BRDR, 3270, 0, -1)->Set(1);

  //Seilrutschen
  CreateZipLine(1230, 605, 1520, 750);
  CreateZipLine(1300, 900, 1520, 1020);
  CreateZipLine(1630, 310, 1890, 150);

  CreateZipLine(1630, 370, 2220, 700);
  CreateZipLine(1650, 750, 2230, 1020);
  CreateZipLine(2240, 370, 1650, 700);
  CreateZipLine(2220, 750, 1640, 1020);

  CreateZipLine(2240, 310, 1980, 150);
  CreateZipLine(2570, 900, 2350, 1020);
  CreateZipLine(2640, 605, 2350, 750);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 1090, 1140, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Right);
  door = CreateObject(SLDR, 2780, 1140, -1);
  door->Lock();
  door->SetMaxDamage(-1);
  door->SetSwitchLock(DIR_Left);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT1, 1195, 795, -1);
  aDoorWay[01] = CreateObject(ROOM, 1235, 1160, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);

  aDoorWay[02] = CreateObject(GAT1, 1885, 245, -1);
  aDoorWay[03] = CreateObject(ROOM, 1895, 1160, -1);
  aDoorWay[02]->Connect(aDoorWay[03]);

  aDoorWay[02] = CreateObject(GAT1, 1985, 245, -1);
  aDoorWay[03] = CreateObject(ROOM, 1975, 1160, -1);
  aDoorWay[02]->Connect(aDoorWay[03]);

  aDoorWay[04] = CreateObject(GAT1, 2675, 795, -1);
  aDoorWay[05] = CreateObject(ROOM, 2635, 1160, -1);
  aDoorWay[04]->Connect(aDoorWay[05]);

  aDoorWay[06] = CreateObject(ROOM, 995, 1180, -1);
  aDoorWay[07] = CreateObject(ROOM, 2875, 1180, -1);
  aDoorWay[06]->Connect(aDoorWay[07]);

  //Sonne umplatzieren
  FindObject(LENS)->SetPosition(1935,590);

  //Soundkulisse

  //Rush Hour
  CreateObject(SE4K, 1000, 850, -1)->Set("Traffic*.ogg",300,100, 25,75);
  CreateObject(SE4K, 2870, 850, -1)->Set("Traffic*.ogg",300,100, 25,75);

  //Sirenen
  CreateObject(SE4K, 900, 850, -1)->Set("Siren*.ogg",2000,200, 75);
  CreateObject(SE4K, 2970, 1000, -1)->Set("Siren*.ogg",2000,200, 75);

  //Windböen
  CreateObject(SE4K, 400, 400, -1)->Set("WindGust*.ogg",800,300);
  CreateObject(SE4K, 1935, 50, -1)->Set("WindGust*.ogg",800,300);
  CreateObject(SE4K, 3470, 400, -1)->Set("WindGust*.ogg",800,300);

  //Hallen
  CreateObject(SE4K, 830, 1200, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 1350, 1220, -1)->Set("InteriorMetal*.ogg",700,100, 75);
  CreateObject(SE4K, 1935, 960, -1)->Set("InteriorStress*.ogg",1000,300, 75);
  CreateObject(SE4K, 2520, 1220, -1)->Set("InteriorMetal*.ogg",700,100, 75);
  CreateObject(SE4K, 3040, 1200, -1)->Set("Interior*.ogg",700,100);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 1275, 1070, -1)->Set(ABOX);
  CreateObject(AMCT, 2595, 1070, -1)->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  CreateObject(AMCT, 1620, 920, -1)->Set(GBOX);
  CreateObject(AMCT, 2250, 920, -1)->Set(GBOX);

  //Versorgungskisten (Dragnin)
  CreateObject(AMCT, 860, 980, -1)->Set(DGNN);
  CreateObject(AMCT, 1480, 1140, -1)->Set(DGNN);
  CreateObject(AMCT, 2390, 1140, -1)->Set(DGNN);
  CreateObject(AMCT, 3020, 980, -1)->Set(DGNN);

  //Versorgungskiste (Doppelflinte)
  CreateObject(AMCT, 1935, 260, -1)->Set(DBSG,0,0,0,1);

  //Versorgungskisten (Railgun)
  CreateObject(AMCT, 1550, 920, -1)->Set(RLGN,0,0,0,1);
  CreateObject(AMCT, 2320, 920, -1)->Set(RLGN,0,0,0,1);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 1530, 1110, -1)->Set(ATWN);
  CreateObject(AMCT, 2340, 1110, -1)->Set(ATWN);

  //Raketen
  PlaceSpawnpoint(MBOX, 1110, 1065);
  PlaceSpawnpoint(MBOX, 2760, 1065);

  //Geschützstellungen
  CreateObject(GNET, 1030, 1015, -1)->Set(SATW,90);
  CreateObject(GNET, 1300, 800, -1)->Set(SATW,90);

  CreateObject(GNET, 1650, 880, -1)->Set(LCAC,90);
  CreateObject(GNET, 1670, 1140, -1)->Set(SATW);
  CreateObject(GNET, 2200, 1140, -1)->Set(SATW);
  CreateObject(GNET, 2220, 880, -1)->Set(LCAC,-90);

  CreateObject(GNET, 2570, 800, -1)->Set(SATW,-90);
  CreateObject(GNET, 2840, 1015, -1)->Set(SATW,-90);

  //MAV-Stationen
  CreateObject(MVSN, 890, 980, -1)->Set(890,930,1);
  CreateObject(MVSN, 2990, 980, -1)->Set(2990,930,1);

  //Artilleriebatterien
  CreateObject(ATBY,1245,680,-1)->SetRotation(20);
  CreateObject(ATBY,2625,680,-1)->SetRotation(-20);

  //Sprungschanzen
  CreateObject(JMPD, 815, 970, -1)->Set(195, 15);
  CreateObject(JMPD, 3055, 970, -1)->Set(195, -15);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Sonnenschirme
  CreateObject(SNSE, 1185, 690, -1);
  CreateObject(SNSE, 1490, 480, -1);
  CreateObject(SNSE, 1550, 480, -1);
  CreateObject(SNSE, 2320, 480, -1);
  CreateObject(SNSE, 2380, 480, -1);
  CreateObject(SNSE, 2685, 690, -1);

  //Orientierungslichter
  CreateObject(OLGH, 815, 970, -1)->Set(1, 13, 1, 1, 50);
  CreateObject(OLGH, 1235, 890, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 1580, 610, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 1580, 930, -1)->Set(7, 10, 1, 1, 10);

  CreateObject(OLGH, 2280, 610, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 2280, 930, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 2630, 890, -1)->Set(7, 10, 1, 1, 10);
  CreateObject(OLGH, 3055, 970, -1)->Set(1, 13, 1, 1, 50);

  //Scheinwerfer
  CreateObject(FLGH, 1080, 741, -1)->Set(1,1,0,30);
  CreateObject(FLGH, 1590, 420, -1)->Set(1,1,0,30);
  CreateObject(FLGH, 1840, 980, -1)->Set(1,1,0,30);
  CreateObject(FLGH, 2030, 980, -1)->Set(1,0,0,-30);
  CreateObject(FLGH, 2280, 420, -1)->Set(1,0,0,-30);
  CreateObject(FLGH, 2790, 741, -1)->Set(1,0,0,-30);

  //Flaschen
  CreateObject(BOTL, 1645, 600, -1);
  CreateObject(BOTL, 2225, 600, -1);

  //Papierstapel
  CreateObject(PPSK, 1200, 880, -1);
  CreateObject(PPSK, 1210, 880, -1);
  CreateObject(PPSK, 1620, 480, -1);
  CreateObject(PPSK, 2250, 480, -1);
  CreateObject(PPSK, 2660, 880, -1);
  CreateObject(PPSK, 2670, 880, -1);

  //Topfpflanzen
  CreateObject(PLT3, 1255, 740, -1);
  CreateObject(PLT3, 1635, 600, -1);
  CreateObject(PLT3, 1740, 480, -1);
  CreateObject(PLT3, 2130, 480, -1);
  CreateObject(PLT3, 2235, 600, -1);
  CreateObject(PLT3, 2615, 740, -1);

  //Apparaturen
  CreateObject(GADG, 855, 1140, -1)->Set(1);
  CreateObject(GADG, 865, 1140, -1)->Set(1);
  CreateObject(GADG, 875, 1140, -1)->Set(1);
  CreateObject(GADG, 2995, 1140, -1)->Set(1);
  CreateObject(GADG, 3005, 1140, -1)->Set(1);
  CreateObject(GADG, 3015, 1140, -1)->Set(1);

  //Zäune
  CreateObject(FENC, 1205, 980, -1)->Set(2);
  CreateObject(FENC, 1265, 980, -1)->Set(2);

  CreateObject(FENC, 1520, 1070, -1);
  CreateObject(FENC, 1580, 1070, -1);
  CreateObject(FENC, 1640, 1070, -1);

  CreateObject(FENC, 2230, 1070, -1);
  CreateObject(FENC, 2290, 1070, -1);
  CreateObject(FENC, 2350, 1070, -1);

  CreateObject(FENC, 2605, 980, -1)->Set(2);
  CreateObject(FENC, 2665, 980, -1)->Set(2);

  //Leitkegel
  CreateObject(TCCN, 1130, 980, -1);
  CreateObject(TCCN, 1145, 980, -1);
  CreateObject(TCCN, 1160, 980, -1);
  CreateObject(TCCN, 1310, 980, -1);
  CreateObject(TCCN, 1325, 980, -1);
  CreateObject(TCCN, 1340, 980, -1);

  CreateObject(TCCN, 2530, 980, -1);
  CreateObject(TCCN, 2545, 980, -1);
  CreateObject(TCCN, 2560, 980, -1);
  CreateObject(TCCN, 2710, 980, -1);
  CreateObject(TCCN, 2725, 980, -1);
  CreateObject(TCCN, 2740, 980, -1);

  //Bildschirme
  CreateObject(SCR3, 150, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 190, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 230, 945, -1)->SetClrModulation(RGB(250,10,10));

  CreateObject(SCR3, 990, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 1030, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 1070, 945, -1)->SetClrModulation(RGB(250,10,10));

  var screen = CreateObject(SCA2, 1235, 980, -1);
  screen->SetClips([12]);
  screen->DoCon(100);

  screen = CreateObject(SCA2, 1235, 1070, -1);
  screen->SetClips([8,9,10,11]);
  screen->DoCon(100);

  screen = CreateObject(SCA2, 1585, 705, -1);
  screen->SetClips([8,9,10,11]);
  screen->DoCon(100);
  CreateObject(SCA2, 1585, 785, -1)->DoCon(100);

  CreateObject(SCA2, 2285, 705, -1)->DoCon(100);
  screen = CreateObject(SCA2, 2285, 785, -1);
  screen->SetClips([8,9,10,11]);
  screen->DoCon(100);

  screen = CreateObject(SCA2, 2635, 980, -1);
  screen->SetClips([12]);
  screen->DoCon(100);

  screen = CreateObject(SCA2, 2635, 1070, -1);
  screen->SetClips([8,9,10,11]);
  screen->DoCon(100);

  CreateObject(SCR3, 2800, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 2840, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 2880, 945, -1)->SetClrModulation(RGB(250,10,10));

  CreateObject(SCR3, 3640, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 3680, 945, -1)->SetClrModulation(RGB(250,10,10));
  CreateObject(SCR3, 3720, 945, -1)->SetClrModulation(RGB(250,10,10));

  //Rahmen entfernen
  RemoveAll(FRME);

  //Spinde
  CreateObject(LCKR, 1210, 740, -1);
  CreateObject(LCKR, 1610, 880, -1);
  CreateObject(LCKR, 1625, 880, -1);
  CreateObject(LCKR, 2245, 880, -1);
  CreateObject(LCKR, 2260, 880, -1);
  CreateObject(LCKR, 2660, 740, -1);

  //Straßenlichter
  CreateObject(SLGH, 410, 980, -1);
  CreateObject(SLGH, 550, 980, -1);
  CreateObject(SLGH, 690, 980, -1);
  CreateObject(SLGH, 1340, 980, -1);

  CreateObject(SLGH, 130, 1070, -1);
  CreateObject(SLGH, 270, 1070, -1);
  CreateObject(SLGH, 410, 1070, -1);
  CreateObject(SLGH, 550, 1070, -1);
  CreateObject(SLGH, 690, 1070, -1);
  CreateObject(SLGH, 830, 1070, -1);
  CreateObject(SLGH, 1110, 1070, -1);
  CreateObject(SLGH, 1250, 1070, -1);
  CreateObject(SLGH, 1390, 1070, -1);
  CreateObject(SLGH, 1530, 1070, -1);
  CreateObject(SLGH, 1670, 1070, -1);

  CreateObject(SLGH, 2200, 1070, -1);
  CreateObject(SLGH, 2340, 1070, -1);
  CreateObject(SLGH, 2480, 1070, -1);
  CreateObject(SLGH, 2620, 1070, -1);
  CreateObject(SLGH, 2760, 1070, -1);
  CreateObject(SLGH, 3040, 1070, -1);
  CreateObject(SLGH, 3180, 1070, -1);
  CreateObject(SLGH, 3320, 1070, -1);
  CreateObject(SLGH, 3460, 1070, -1);
  CreateObject(SLGH, 3600, 1070, -1);
  CreateObject(SLGH, 3740, 1070, -1);

  CreateObject(SLGH, 2530, 980, -1);
  CreateObject(SLGH, 3180, 980, -1);
  CreateObject(SLGH, 3320, 980, -1);
  CreateObject(SLGH, 3460, 980, -1);

  //Verkehrsampeln
  CreateObject(TLGH, 965, 980, -1)->SetAction("Green");
  CreateObject(TLGH, 1095, 980, -1)->SetAction("Green");
  CreateObject(TLGH, 2775, 980, -1)->SetAction("Green");
  CreateObject(TLGH, 2905, 980, -1)->SetAction("Green");

  //Regale
  CreateObject(FRAM, 1185, 770, -1);
  CreateObject(FRAM, 1205, 770, -1);

  CreateObject(FRAM, 1265, 770, -1);
  CreateObject(FRAM, 1285, 770, -1);

  CreateObject(FRAM, 2585, 770, -1);
  CreateObject(FRAM, 2605, 770, -1);

  CreateObject(FRAM, 2665, 770, -1);
  CreateObject(FRAM, 2685, 770, -1);

  //Notausgangslichter
  CreateObject(ETLT, 1178, 852, -1);
  CreateObject(ETLT, 1585, 520, -1);
  CreateObject(ETLT, 2285, 520, -1);
  CreateObject(ETLT, 2692, 852, -1);

  //Anlagen
  CreateObject(CCP2, 1265, 800, -1);
  CreateObject(CCP1, 2600, 800, -1);

  //Büsche
  CreateObject(BSH2, 750, 1070, -1);
  CreateObject(BSH2, 770, 1070, -1);
  CreateObject(BSH2, 1830, 1020, -1);
  CreateObject(BSH2, 1850, 1020, -1);
  CreateObject(BSH2, 2020, 1020, -1);
  CreateObject(BSH2, 2040, 1020, -1);
  CreateObject(BSH2, 3100, 1070, -1);
  CreateObject(BSH2, 3120, 1070, -1);

  //Glasröhren
  CreateObject(GLST, 905, 1200, -1);
  CreateObject(GLST, 2965, 1200, -1);

  //Markierungen
  CreateObject(MSGN, 1355, 980, -1);
  CreateObject(MSGN, 1365, 980, -1);
  CreateObject(MSGN, 1375, 980, -1);

  CreateObject(MSGN, 1445, 1070, -1);
  CreateObject(MSGN, 1455, 1070, -1);
  CreateObject(MSGN, 1465, 1070, -1);

  CreateObject(MSGN, 2405, 1070, -1);
  CreateObject(MSGN, 2415, 1070, -1);
  CreateObject(MSGN, 2425, 1070, -1);

  CreateObject(MSGN, 2495, 980, -1);
  CreateObject(MSGN, 2505, 980, -1);
  CreateObject(MSGN, 2515, 980, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1065, 1135, -1);
  CreateObject(FIE2, 1220, 875, -1);
  CreateObject(FIE2, 1535, 595, -1);
  CreateObject(FIE2, 1535, 875, -1);
  CreateObject(FIE2, 2335, 595, -1);
  CreateObject(FIE2, 2335, 875, -1);
  CreateObject(FIE2, 2650, 875, -1);
  CreateObject(FIE2, 2805, 1135, -1);

  //Dekoschleusen
  CreateObject(GAT2, 950, 1140, -1);
  CreateObject(GAT2, 1020, 1140, -1);
  CreateObject(GAT2, 1235, 1105, -1);
  CreateObject(GAT2, 2635, 1105, -1);
  CreateObject(GAT2, 2850, 1140, -1);
  CreateObject(GAT2, 2920, 1140, -1);

  //Geländer
  CreateObject(RAI1, 928, 1170, -1);
  CreateObject(RAI1, 1075, 740, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 1075, 800, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 1215, 680, -1)->SetRail([2,2,2]);
  CreateObject(RAI1, 1285, 740, -1)->SetRail([2,2,2]);
  CreateObject(BART, 1302, 1070, -1)->SetRail([1,0,1,0,1]);

  CreateObject(RAI1, 1470, 480, -1)->SetRail([2,2,2,2,2]);
  CreateObject(RAI1, 1560, 840, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1580, 420, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 1859, 1160, -1)->SetRail([2,2,2,2,2,2,2,2]);
  CreateObject(RAI1, 2210, 420, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 2268, 840, -1)->SetRail([1,1]);
  CreateObject(RAI1, 2302, 480, -1)->SetRail([2,2,2,2,2]);

  CreateObject(BART, 2492, 1070, -1)->SetRail([1,0,1,0,1]);
  CreateObject(RAI1, 2525, 740, -1)->SetRail([2,2,2]);
  CreateObject(RAI1, 2595, 680, -1)->SetRail([2,2,2]);
  CreateObject(RAI1, 2718, 740, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 2718, 800, -1)->SetRail([2,2,2,2]);
  CreateObject(RAI1, 2918, 1170, -1);

  //Automaten
  CreateObject(VGMN, 1615, 480, -1);
  CreateObject(VGMN, 2255, 480, -1)->Set(2);

  //Poster
  CreateObject(PSTR, 1275, 880, -1)->Set(2,3);
  CreateObject(PSTR, 1625, 850, -1)->Set(2,1);
  CreateObject(PSTR, 2245, 850, -1)->Set(2,2);
  CreateObject(PSTR, 2595, 880, -1)->Set(2,4);

  //Radare
  CreateObject(RADR, 690, 930, -1);
  CreateObject(RADR, 3180, 930, -1);

  //Satellitenschüsseln
  CreateObject(SADH, 1850, 260, -1);
  CreateObject(SADH, 2040, 260, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1825, 1110, -1);
  CreateObject(ENGT, 2045, 1110, -1);

  //Ventillatoren
  CreateObject(MVNT, 1585, 920, -1)->SetCon(50);
  CreateObject(VENT, 1585, 1060, -1)->SetCon(90);
  CreateObject(VENT, 1905, 1030, -1)->SetCon(40);
  CreateObject(VENT, 1965, 1030, -1)->SetCon(40);
  CreateObject(MVNT, 2285, 920, -1)->SetCon(50);
  CreateObject(VENT, 2285, 1060, -1)->SetCon(90);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD11,600,1180,-1);
  CreateObject(BD11,1800,1180,-1);
  CreateObject(BD11,3000,1180,-1);
  CreateObject(BD11,4200,1180,-1);

  CreateObject(BD10,100,1250,-1);
  CreateObject(BD10,1490,1250,-1);
  CreateObject(BD10,2880,1250,-1);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Red Mountain.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Red Mountain.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,720,910,-1),100*21);
    SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,900,910,-1),100*21);
    SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,2970,910,-1),100*21);
    SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,3150,910,-1),100*21);

    CreateObject(SGNP, 980, 930, -1)->Set("Helicopter",1);
    CreateObject(SGNP, 2890, 930, -1)->Set("Helicopter",1);
  }

  //Zeit
  CastObjects(TIME,7);

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Script starten
    ScriptGo(1);
    aFlagPosition = 2;

    //Flaggenposten
    aFlag[0] = CreateObject(OFPL, 1935, 655, -1);
    aFlag[0]->~Set("$Flag2$",150);

    //Teamgrenzen
    CreateObject(BRDR, 1410, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 2460, 0, -1)->Set(1,1,0,1,2);

    //Gerüste
    CreateObject(SFFG, 1935, 260, -1)->Set(5);
    CreateObject(SFFG, 1935, 702, -1)->Set(5);
    CreateObject(SFFG, 1935, 1110, -1)->Set(5);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,1380,1070,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,2490,1070,GetTeamColor(2));}
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
    //Geschützstellungen
    CreateObject(GNET, 655, 930, -1)->Set(LCAC);
    CreateObject(GNET, 3215, 930, -1)->Set(LCAC);
  }
  else
  {
    //Selbstschussanlagen
    var selfdef = CreateObject(SEGU, 660, 930, -1);
    selfdef->Set(0,1,1,180,0,1280);
    selfdef->SetTeam(1);
    selfdef = CreateObject(SEGU, 3210, 930, -1);
    selfdef->Set(0,1,1,180,2590);
    selfdef->SetTeam(2);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 965; iY = 920;
    return 1;
  }

  //HTF/CTF/LMS-Spielziel
  if(FindObject(GHTF) || FindObject(GCTF) || FindObject(GLMS))
  {
    if(iTeam == 1)
      return [[1185, 680], [1195, 730], [1260, 790], [1260, 870]];
    if(iTeam == 2)
      return [[2685, 680], [2675, 730], [2610, 790], [2610, 870]];

    return 1;
  }
}

/* Flaggensteuerung */

protected func Script250()
{
  EventInfo4K(0,Format("$MsgFlagChanging$"),SM21, 0, 0, 0, "Info_Objective.ogg");
  aFlag[0]->AddSmokeEffect4K(50,0,-10);
}

protected func Script300()
{
  RemoveEffect("IntWreckSmoke4K",aFlag[0]);
  if(aFlagPosition == 1)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(1935,655,"$Flag2$",150);
      aFlagPosition = 2;
    }
    else
    {
      aFlag[0]->MoveFlagpost(1935,1063,"$Flag3$",100);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 2)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(1935,210,"$Flag1$",130);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(1935,1063,"$Flag3$",100);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 3)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(1935,210,"$Flag1$",130);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(1935,655,"$Flag2$",150);
      aFlagPosition = 2;
    }
  }

  EventInfo4K(0,Format("$MsgFlagChanged$", GetName(aFlag[0])),SM21, 0, 0, 0, "Info_Objective.ogg");

  goto(0);
}