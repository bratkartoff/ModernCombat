/*-- Infinity Island --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aStationary,aTowerInterior,aDoorWay;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,50);
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0),RGB(152,147,128),RGB(255,254,236));
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Turmobjekte
  aTowerInterior = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  if(C4X_VerBuild < 331) {SetNextMission("ModernCombat.c4f\\CMC_InfinityIsland.c4s", "$RepeatButton$", "$RepeatButtonDesc$");}
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 1255, 1260, -1)->Set(14, LADR_Left);
  CreateObject(LADR, 1480, 1238, -1)->Set(25);
  CreateObject(LADR, 1590, 1238, -1)->Set(25);
  CreateObject(LADR, 1720, 1250, -1)->Set(9);
  CreateObject(LADR, 1880, 1258, -1)->Set(10);
  CreateObject(LADR, 2040, 1258, -1)->Set(10);
  CreateObject(LADR, 2200, 1258, -1)->Set(10);
  CreateObject(LADR, 2395, 1260, -1)->Set(14, LADR_Right);
  CreateObject(LADR, 3412, 1090, -1)->Set(23, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3530, 1260, -1)->Set(17, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3625, 1256, -1)->Set(2, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3635, 1055, -1)->Set(5, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 3910, 640, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4380, 805, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 4650, 1070, -1)->Set(19, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 5425, 1267, -1)->Set(4);
  CreateObject(LADR, 5575, 1267, -1)->Set(4);

  //Bodenluken
  CreateObject(HA4K, 1480, 1033, -1);
  CreateObject(HA4K, 1480, 1113, -1);
  CreateObject(HA4K, 1480, 1173, -1);
  CreateObject(HA4K, 1590, 1033, -1);
  CreateObject(HA4K, 1590, 1113, -1);
  CreateObject(HA4K, 1590, 1173, -1);
  CreateObject(HA4K, 1720, 1173, -1);
  CreateObject(HA4K, 1880, 1173, -1);
  CreateObject(HA4K, 2040, 1173, -1);
  CreateObject(HA4K, 2200, 1173, -1);

  //Rampen
  DrawMaterialQuad("Wall-Plate",1260,1320,2390,1320,2270,1350,1290,1350,true);
  DrawMaterialQuad("Wall-Metal2",1290,1350,2270,1350,2230,1360,1300,1360,true);

  DrawMaterialQuad("Wall-Stripes",1310,1140,1340,1140,1430,1170,1400,1170,true);

  DrawMaterialQuad("Wall-Metal2",1340,1300,1400,1280,1400,1300,1340,1300,true);
  DrawMaterialQuad("Wall-Plate",1400,1280,1460,1260,1460,1280,1400,1280,true);

  DrawMaterialQuad("Wall-Plate",1730,1260,1790,1280,1730,1280,1730,1260,true);
  DrawMaterialQuad("Wall-Metal2",1790,1280,1850,1300,1790,1300,1790,1280,true);

  DrawMaterialQuad("Wall-Stripes",2220,1170,2310,1140,2340,1140,2250,1170,true);

  DrawMaterialQuad("Wall-Stripes", 2290, 1279, 2350, 1279, 2290, 1280, 2350, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3130, 1278, 3180, 1278, 3130, 1280, 3180, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3350, 1278, 3400, 1278, 3350, 1280, 3400, 1280, 1);
  DrawMaterialQuad("Rock-ROUGH", 3500, 1278, 3570, 1278, 3500, 1280, 3570, 1280, 1);
  DrawMaterialQuad("Granite-Rock", 4570, 1228, 4630, 1228, 4570, 1230, 4630, 1230, 1);

  //Glasscheiben
  CreateObject(PANE, 1422, 1081, -1);
  CreateObject(PANE, 1422, 1110, -1);
  CreateObject(PANE, 1648, 1081, -1);
  CreateObject(PANE, 1648, 1110, -1);

  //Kisten
  CreateObject(WCR2, 1690, 1170, -1)->AutoRespawn();
  CreateObject(WCR2, 1690, 1260, -1)->AutoRespawn();
  CreateObject(WCR2, 3640, 521, -1);
  CreateObject(WCR2, 3700, 571, -1);

  //Metallkiste
  CreateObject(MWCR, 4650, 1220, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 1670, 1260, -1)->AutoRespawn();
  CreateObject(BECR, 3705, 292, -1);
  CreateObject(BECR, 4120, 580, -1)->AutoRespawn();
  CreateObject(BECR, 5190, 1240, -1)->AutoRespawn();
  CreateObject(BECR, 5380, 1270, -1);

  //Explosive Kisten
  CreateObject(XWCR, 1845, 1170, -1)->AutoRespawn();
  CreateObject(XWCR, 3600, 868, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1620, 1260, -1)->AutoRespawn();
  CreateObject(PBRL, 3660, 850, -1)->AutoRespawn();
  CreateObject(PBRL, 3670, 292, -1);
  CreateObject(PBRL, 4505, 938, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 1600, 1260, -1)->AutoRespawn();
  CreateObject(XBRL, 3280, 1208, -1)->AutoRespawn();
  CreateObject(XBRL, 4605, 918, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1635, 1030, -1)->AutoRespawn();
  CreateObject(GSBL, 4220, 818, -1)->AutoRespawn();
  CreateObject(GSBL, 5160, 1238, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 5325, 1282, -1);
  CreateObject(_HBR, 5395, 1282, -1);

  //Gerüste
  CreateObject(SFFG, 4715, 1240, -1)->Set(5);
  CreateObject(SFFG, 5470, 1280, -1)->Set(4);
  var plat = CreateObject(SFFG, 5470, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
  CreateObject(SFFG, 5530, 1280, -1)->Set(4);
  plat = CreateObject(SFFG, 5530, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));

  //Container
  CreateObject(CON1, 1780, 1171, -1)->SetGraphics("5");
  var container = CreateObject(CON1, 1820, 1171, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 4530, 940, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");

  //Hydrauliktüren
  var door = CreateObject(SEDR,1459, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1459, 1170,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1611, 1030,-1);
  door->Open();
  door->Lock();
  door = CreateObject(SEDR,1611, 1170,-1);
  door->Open();
  door->Lock();

  //Explosivtanks
  CreateObject(XTNK, 1480, 1260, -1)->AutoRespawn();
  CreateObject(XTNK, 1590, 1260, -1)->AutoRespawn();

  //Tore und Konsolen
  var autod = CreateObject(HNG3, 1423, 1110, -1);
  autod->Open();
  CreateObject(CONS, 1500, 1105, -1)->Set(autod);
  autod = CreateObject(HNG3, 1647, 1110, -1);
  autod->Open();
  CreateObject(CONS, 1570, 1105, -1)->Set(autod);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1310, 1140, -1);
  aSelfDefense[0]->Set(0,1,0,180);
  aSelfDefense[1] = CreateObject(SEGU, 2340, 1140, -1);
  aSelfDefense[1]->Set(0,1,0,180);

  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(1250, 1270, 1150, 35)))
    RemoveObject(obj);

  //Sendemast
  var tower = CreateObject(AATR, 3670, 850, -1);
  tower->AddNode(3203, 1205, 0, CreateObject(REHR, 3195, 1230, -1));
  tower->AddNode(3443, 885, 2, CreateObject(REHR, 3435, 900, -1), -40, 1);
  tower->AddNode(3860, 672, 2, CreateObject(REHR, 3855, 690, -1), -90, 2);
  tower->AddNode(3955, 550, 0, CreateObject(REHR, 3942, 560, -1), -40, 2);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(GAT3, 3670, 455, -1);
  aDoorWay[01] = CreateObject(ROOM, 4125, 640, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);

  //Wasseroberfläche ebnen
  DrawMaterialQuad("Water",1761,1270,2380,1270,2380,1271,1766,1271,1);

  //Soundkulisse

  //Zikaden
  CreateObject(SE4K, 3600, 850, -1)->Set("Cicada*.ogg",700,100, 50);
  CreateObject(SE4K, 4700, 1000, -1)->Set("Cicada*.ogg",700,100, 50);

  //Motor
  CreateObject(SE4K, 1400, 1360, -1)->Set("tank");

  //Möven
  CreateObject(SE4K, 800, 1200, -1)->Set("Seagulls*.ogg",1000,500, 25,50);
  CreateObject(SE4K, 3000, 1200, -1)->Set("Seagulls*.ogg",1000,500, 10,25);
  CreateObject(SE4K, 5700, 1200, -1)->Set("Seagulls*.ogg",1000,500, 10,25);

  //Vogelzwitschern
  CreateObject(SE4K, 3600, 800, -1)->Set("BirdSong*",200,50);
  CreateObject(SE4K, 4700, 900, -1)->Set("BirdSong*",200,50);

  //Wipfe
  CreateObject(SE4K, 4950, 1100, -1)->Set("Snuff*",900,100);

  //Erdrutsche
  CreateObject(SE4K, 4000, 1200, -1)->Set("FallingDirt*.ogg",800,200, 25,50);

  //Wellen
  CreateObject(SE4K, 400, 1270, -1)->Set("Wave*.ogg",100,50);
  CreateObject(SE4K, 1070, 1270, -1)->Set("Wave*.ogg",100,50);
  CreateObject(SE4K, 2780, 1270, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 5450, 1270, -1)->Set("Wave*.ogg",100,50, 75);
  CreateObject(SE4K, 6530, 1270, -1)->Set("Wave*.ogg",100,50);

  //Jungel
  CreateObject(SE4K, 3290, 1140, -1)->Set("Jungle*.ogg",800,200, 75);
  CreateObject(SE4K, 3660, 800, -1)->Set("Jungle*.ogg",800,200, 75);
  CreateObject(SE4K, 4130, 550, -1)->Set("Jungle*.ogg",800,200, 75);
  CreateObject(SE4K, 4900, 1100, -1)->Set("Jungle*.ogg",800,200, 75);

  //Lautsprecher
  CreateObject(SE4K, 1800, 1060, -1)->Set("Announce*.ogg",3000,200, 75);

  //Wind
  CreateObject(SE4K, 1100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);
  CreateObject(SE4K, 2100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);
  CreateObject(SE4K, 3100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);
  CreateObject(SE4K, 4100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);
  CreateObject(SE4K, 5100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);
  CreateObject(SE4K, 6100, 300, -1)->Set("WindSound*.ogg",3000,300, 75);

  //Innenbereich
  CreateObject(SE4K, 1535, 1280, -1)->Set("Interior*.ogg",800,300, 50,75);
  CreateObject(SE4K, 1800, 1280, -1)->Set("InteriorMetal*.ogg",1500,300, 25,50);
  CreateObject(SE4K, 2070, 1280, -1)->Set("InteriorStress*.ogg",1000,300, 50,75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 1770, 1142, -1)->Set(ABOX);
  CreateObject(AMCT, 3790, 790, -1)->Set(ABOX);
  CreateObject(AMCT, 4530, 1220, -1)->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  CreateObject(AMCT, 1820, 1142, -1)->Set(GBOX);
  CreateObject(AMCT, 4140, 770, -1)->Set(GBOX);
  CreateObject(AMCT, 4650, 1090, -1)->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 3440, 1125);
  PlaceSpawnpoint(MBOX, 4560, 935);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 3490, 1120, -1)->Set(ATWN);
  CreateObject(AMCT, 4360, 820, -1)->Set(ATWN);

  //Versorgungskiste (Doppelflinte)
  CreateObject(AMCT, 4090, 1060, -1)->Set(DBSG,0,0,0,1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Sonnenschirme
  CreateObject(SNSE, 4665, 1090, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(SNSE, 4910, 1170, -1)->SetColorDw(HSL(40, 210, 100, 127));
  CreateObject(SNSE, 5280, 1260, -1)->SetColorDw(HSL(40, 210, 100, 127));

  //Labortische
  CreateObject(LTBL, 1450, 1110, -1);
  CreateObject(LTBL, 1620, 1110, -1);

  //Monitore
  CreateObject(MNI2, 1440, 1098, -1);
  CreateObject(MNI2, 1460, 1098, -1);
  CreateObject(MNI2, 1610, 1098, -1)->Off();
  CreateObject(MNI2, 1630, 1098, -1);
  CreateObject(MNI2, 1690, 1152, -1);

  //Bildschirm
  CreateObject(SCA2, 1535, 1095, -1)->SetClips([12]);

  //Jetwrack
  CreateObject(_JW1, 5390, 1340, -1)->SetR(-20);
  CreateObject(_JW2, 5535, 1415, -1)->SetR(20);

  //Schiffswrack
  CreateObject(_WRK, 2670, 1450, -1);

  //Truhe
  CreateObject(CHST, 2715, 1450, -1);

  //Kaputtes Boot
  CreateObject(SLBB, 1025, 1485, -1)->SetR(40);

  //Holzhütten
  CreateObject(HTB2, 4610, 1075, -1)->SetR(2);
  CreateObject(HTB2, 4815, 1100, -1)->SetR(5);
  CreateObject(HTB2, 4885, 1170, -1)->SetR(-1);
  CreateObject(HTB2, 5065, 1170, -1)->SetR(5);
  CreateObject(HTB2, 5175, 1240, -1)->SetR(-2);

  //Regale
  CreateObject(FRAM, 1440, 1170, -1);
  CreateObject(FRAM, 1630, 1170, -1);
  CreateObject(FRAM, 1650, 1170, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1480, 1025, -1);

  //Satellitenschüssel
  CreateObject(SADH, 1535, 980, -1);

  //Radare
  CreateObject(RADR, 1490, 980, -1);
  CreateObject(RADR, 1580, 980, -1)->SetPhase(10);
  CreateObject(RADR, 1758, 1030, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 5535, 1230, -1);

  //Markierungen
  CreateObject(MSGN, 1282, 1260, -1);
  CreateObject(MSGN, 1410, 1260, -1);
  CreateObject(MSGN, 1780, 1260, -1);
  CreateObject(MSGN, 1705, 1170, -1);
  CreateObject(MSGN, 1735, 1170, -1);
  CreateObject(MSGN, 1790, 1260, -1);
  CreateObject(MSGN, 1865, 1170, -1);
  CreateObject(MSGN, 1895, 1170, -1);
  CreateObject(MSGN, 2025, 1170, -1);
  CreateObject(MSGN, 2055, 1170, -1);
  CreateObject(MSGN, 2185, 1170, -1);
  CreateObject(MSGN, 2215, 1170, -1);
  CreateObject(MSGN, 2370, 1260, -1);

  //Hinweisschild
  CreateObject(WLSN, 1500, 1025, -1)->Set("Extinguisher","Red");

  //Apparaturen
  CreateObject(GADG, 4690, 1240, -1)->Set(1);
  CreateObject(GADG, 4700, 1240, -1)->Set(1);
  CreateObject(GADG, 4710, 1240, -1)->Set(1);

  //Orientierungslichter
  CreateObject(OLGH, 1310, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 1540, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1960, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2340, 1140, -1)->Set(2, 10, 1, 1, 30);

  //Bäume
  CreateObject(TRE1, 3455, 895, -1)->SetR(-15);
  CreateObject(TRE1, 3935, 575, -1)->SetR(-10);
  CreateObject(TRE1, 4280, 615, -1)->SetR(10);
  CreateObject(TRE1, 4630, 925, -1)->SetR(15);

  //Lianen
  SetCon(60, CreateObject(VINE, 3335, 1180, -1));
  SetCon(100, CreateObject(VINE, 3385, 1195, -1));
  SetCon(80, CreateObject(VINE, 3450, 1160, -1));
  SetCon(90, CreateObject(VINE, 3500, 1160, -1));
  SetCon(90, CreateObject(VINE, 3525, 1040, -1));
  SetCon(60, CreateObject(VINE, 3540, 1030, -1));
  SetCon(60, CreateObject(VINE, 3555, 1130, -1));
  SetCon(100, CreateObject(VINE, 3625, 1160, -1));
  SetCon(60, CreateObject(VINE, 3585, 1030, -1));
  SetCon(100, CreateObject(VINE, 3670, 1200, -1));
  SetCon(70, CreateObject(VINE, 3705, 1180, -1));
  SetCon(60, CreateObject(VINE, 3835, 855, -1));
  SetCon(100, CreateObject(VINE, 3900, 880, -1));
  SetCon(70, CreateObject(VINE, 3925, 880, -1));
  SetCon(90, CreateObject(VINE, 3965, 620, -1));
  SetCon(60, CreateObject(VINE, 3970, 870, -1));
  SetCon(100, CreateObject(VINE, 3990, 640, -1));
  SetCon(50, CreateObject(VINE, 4000, 640, -1));
  SetCon(40, CreateObject(VINE, 4020, 620, -1));
  SetCon(80, CreateObject(VINE, 4050, 610, -1));
  SetCon(100, CreateObject(VINE, 4115, 690, -1));
  SetCon(60, CreateObject(VINE, 4145, 700, -1));
  SetCon(50, CreateObject(VINE, 4160, 710, -1));
  SetCon(40, CreateObject(VINE, 4315, 910, -1));
  SetCon(80, CreateObject(VINE, 4320, 910, -1));
  SetCon(100, CreateObject(VINE, 4345, 910, -1));
  SetCon(50, CreateObject(VINE, 4370, 920, -1));
  SetCon(40, CreateObject(VINE, 4380, 920, -1));
  SetCon(50, CreateObject(VINE, 4415, 910, -1));
  SetCon(100, CreateObject(VINE, 4655, 1140, -1));
  SetCon(90, CreateObject(VINE, 4675, 1130, -1));
  SetCon(40, CreateObject(VINE, 4700, 1140, -1));
  SetCon(45, CreateObject(VINE, 4775, 1140, -1));
  SetCon(90, CreateObject(VINE, 4805, 1140, -1));
  SetCon(70, CreateObject(VINE, 4825, 1140, -1));
  SetCon(50, CreateObject(VINE, 5055, 1190, -1));
  SetCon(40, CreateObject(VINE, 5075, 1200, -1));
  SetCon(30, CreateObject(VINE, 5085, 1200, -1));

  //Büsche
  CreateObject(BSH2, 3430, 990, -1);
  CreateObject(BSH2, 3780, 1080, -1);
  CreateObject(BSH2, 3860, 1000, -1);
  CreateObject(BSH2, 3880, 1070, -1);
  CreateObject(BSH2, 3870, 720, -1);
  CreateObject(BSH2, 3940, 970, -1);
  CreateObject(BSH2, 3980, 545, -1);
  CreateObject(BSH2, 4000, 540, -1);
  CreateObject(BSH2, 4020, 540, -1);
  CreateObject(BSH2, 4040, 545, -1);
  CreateObject(BSH2, 4300, 670, -1);
  CreateObject(BSH2, 4365, 1010, -1);
  CreateObject(BSH2, 4390, 1020, -1);
  CreateObject(BSH2, 4430, 1040, -1);
  CreateObject(BSH2, 4600, 1000, -1);
  CreateObject(BSH2, 4760, 1100, -1);
  CreateObject(BSH2, 5445, 1250, -1);
  CreateObject(BSH2, 5520, 1255, -1);

  //Steine
  CreateObject(STNE, 1140, 1420, -1);
  CreateObject(STNE, 1680, 1460, -1);
  CreateObject(STNE, 2895, 1370, -1);
  CreateObject(STNE, 3737, 820, -1)->Set(3);
  CreateObject(STNE, 3760, 1310, -1)->Set(1);
  CreateObject(STNE, 3900, 980, -1)->Set(0);
  CreateObject(STNE, 4135, 1310, -1)->Set(4);
  CreateObject(STNE, 5770, 1480, -1);

  //Spind
  CreateObject(LCKR, 1435, 1170, -1);

  //Rohre
  CreateObject(PIPL, 1290, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1320, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1950, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1980, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2110, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2140, 1325, -1)->Up()->Up();

  //Geländer
  CreateObject(RAI1, 1265, 1140, -1)->SetRail([1,3,1,3,1]);
  var rail = CreateObject(RAI1, 1280, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 19; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 1409, 1030, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,2,2,2,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 1463, 980, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  rail = CreateObject(RAI1, 1728, 1260, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 65; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 2318, 1140, -1)->SetRail([1,3,1,3,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 1360, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);
  CreateObject(ENGT, 1800, 1260, -1)->AddSmokeEffect4K(100,0,-10,true,this);
  CreateObject(ENGT, 1955, 1260, -1);
  CreateObject(ENGT, 2115, 1260, -1);
  CreateObject(ENGT, 2295, 1260, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,500,250,-1);
  CreateObject(BD03,2000,350,-1);
  CreateObject(BD03,3500,200,-1);
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");

  //Geschützstellungen entfernen
  if(aTowerInterior[0]) aTowerInterior[0]->DecoExplode(30);
  if(aTowerInterior[1]) aTowerInterior[1]->DecoExplode(30);

  //Türverbindung entfernen
  aDoorWay[00]->SealEntrance(1);
  aDoorWay[01]->SealEntrance();
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
  {
    aSelfDefense[0]->SetTeam(iTeam);
    aSelfDefense[1]->SetTeam(iTeam);
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Blue Resort.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Resort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Aufzug
  CreateObject(LFTP, 1535, 1270, -1);

  //Haie
  CreateObject(SHRK, 400, 1500, -1)->AutoRespawn();
  CreateObject(SHRK, 1000, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 1200, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 2400, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 2700, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 5600, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 5700, 1400, -1)->AutoRespawn();
  CreateObject(SHRK, 6400, 1500, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 990, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 1240, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 2420, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 2700, 1440, -1)->AutoRespawn();
  CreateObject(PIRA, 3910, 1290, -1)->AutoRespawn();
  CreateObject(PIRA, 4005, 1090, -1)->AutoRespawn();
  CreateObject(PIRA, 4180, 1090, -1)->AutoRespawn();
  CreateObject(PIRA, 5660, 1420, -1)->AutoRespawn();
  CreateObject(PIRA, 5810, 1420, -1)->AutoRespawn();

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,2320,1280,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1615,1100);
    aFlag[0] -> AddSpawnPoint(1645,1160);
    aFlag[0] -> AddSpawnPoint(1645,1250);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,3470,890,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(3785,980);
    aFlag[1] -> AddSpawnPoint(3805,1070);
    aFlag[1] -> AddSpawnPoint(3710,1040);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,4010,530,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(4085,640);
    aFlag[2] -> AddSpawnPoint(4265,720);
    aFlag[2] -> AddSpawnPoint(4305,830);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,4760,1090,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(4715,1230);
    aFlag[3] -> AddSpawnPoint(4805,1200);
    aFlag[3] -> AddSpawnPoint(4995,1230);
    if(aTeams[2] == true)
    {
      aFlag[3]->Set("$Flag4$",0,4);
      aFlag[3]->Capture(2,1);
    }
    else
    {
      aFlag[3]->Set("$Flag4$");
    }

    //Objekte entfernen
    RemoveObject(FindObject2(Find_ID(XTNK),Find_InRect(1450, 1220, 60, 40)));
    RemoveObject(FindObject2(Find_ID(RSPT),Find_InRect(1479, 1258, 2, 2)));

    //Kaufstation
    var store = CreateObject(WPVM,1480, 1260,-1);
    store->AddWare(C4PA,10);
    store->AddWare(FAPK,10);
    store->AddWare(CDBT,10);
    store->AddWare(BWTH,10);
    store->AddWare(RSHL,10);
    store->AddWare(ATWN,20);
    store->AddWare(FGRN,20);
    store->AddWare(FRAG,20);
    store->AddWare(PGRN,20);
    store->AddWare(SGRN,20);
    store->AddWare(STUN,20);
    store->AddWare(SRBL,20);

    //Geschützstellungen
    CreateObject(GNET, 1795, 1142, -1)->Set(SATW);
    CreateObject(GNET, 2380, 1140, -1)->Set(SATW,90,1);
    CreateObject(GNET, 3880, 660, -1)->Set(SATW,-90,1);
    CreateObject(GNET, 4310, 720, -1)->Set(SATW,90,1);
    CreateObject(GNET, 5060, 1170, -1)->Set(SATW,0,1);
    aTowerInterior[0] = CreateObject(GNET, 3640, 473, -1);
    aTowerInterior[0] -> Set(SATW,-90);
    aTowerInterior[1] = CreateObject(GNET, 3700, 473, -1);
    aTowerInterior[1] -> Set(SATW,90);

    //MAV-Station
    CreateObject(MVSN, 4170, 580, -1)->Set(4170,530,1);

    //Gerüst
    CreateObject(SFFG, 4170, 580, -1)->Set(4);

    //Helikopter und Hinweisschild
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1970,1140,-1),100*21);
      SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,2135,1140,-1),100*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5495,1200,-1),100*21);

      CreateObject(SGNP, 1640, 1170, -1)->Set("Helicopter",1);
      CreateObject(SGNP, 5405, 1270, -1)->Set("Helicopter");
    }

    //SSA Besitzer setzen
    if(aTeams[1] == true)
    {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

    //SSA aktivieren
    aSelfDefense[0]->TurnOn();
    aSelfDefense[1]->TurnOn();

    //Grenzen
    CreateObject(BRDR, 910, 0, -1)->Set(0);
    CreateObject(BRDR, 5970, 0, -1)->Set(1);

    //Hinweisschilder
    CreateObject(SGNP, 830, 1220, -1);
    CreateObject(SGNP, 6010, 1210, -1);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1821,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1904,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4150,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }

  //AS-Spielziel
  if(FindObject(GASS))
  {
    //Zielobjekte
    AddAssaultTarget(GSTA, 3260, 1210, 30*30, 2, "$Target1$", 0, [[[3550, 1278], [3690, 850], [3755, 800]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
    AddAssaultTarget(GSTA, 3700, 1050, 30*30, 2, "$Target1$", 1, [[[3550, 1278], [3690, 850], [3755, 800]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
    AddAssaultTarget(RADR, 4010, 530, 30*30, 2, "$Target2$", 2, [[[4300, 970], [4465, 1050], [4530, 911]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
    AddAssaultTarget(CMSN, 4245, 820, 30*30, 2, "$Target3$", 3, [[[4300, 970], [4465, 1050], [4530, 911]], [[1640, 1260], [1660, 1170], [2120, 1170]]]);
    AddAssaultTarget(HUT3, 4760, 1090, 30*30, 2, "$Target4$", 4, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);
    AddAssaultTarget(CMSN, 4710, 1240, 30*30, 2, "$Target5$", 5, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);
    AddAssaultTarget(LBPC, 4180, 1270, 30*30, 2, "$Target3$", 6, [[[4995, 1240], [5105, 1190], [5280, 1260]], [[3690, 850], [3805, 790], [3890, 660]]]);

    //Ziele verbinden
    ConnectAssaultTargets([0, 1]);
    ConnectAssaultTargets([2, 3]);
    ConnectAssaultTargets([4, 5, 6]);

    //Leitern
    CreateObject(LADR, 4310, 670, -1)->Set(15, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
    CreateObject(LADR, 4330, 715, -1)->Set(5, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

    //Container
    CreateObject(CON1, 4215, 582, -1);
    CreateObject(CON1, 4265, 582, -1);
    CreateObject(CON1, 4280, 610, -1)->SetPerspective(2);

    //Metallkisten
    CreateObject(MWCR, 4050, 910, -1)->AutoRespawn();
    CreateObject(MWCR, 4290, 720, -1);
    CreateObject(MWCR, 4300, 702, -1);
    CreateObject(MWCR, 4310, 720, -1);

    //Boden
    DrawMaterialQuad("Rock-Rough", 3745, 1200, 3800, 1150, 3810, 1340, 3770, 1320);
    DrawMaterialQuad("Granite-Rock", 3800, 1150, 3880, 1130, 3840, 1350, 3810, 1340);
    DrawMaterialQuad("Wall-Stripes", 4130,1270, 4230,1270, 4230,1280, 4130,1280);

    //Grenzen
    CreateObject(BRDR, 910, 0, -1)->Set(0);
    CreateObject(BRDR, 3900, 0, -1)->Set(1,1);
    CreateObject(BRDR, 0, 700, -1)->Set(2,1);

    //Hinweisschilder
    CreateObject(SGNP, 830, 1220, -1);
    CreateObject(SGNP, 6010, 1210, -1);

    //SSA Besitzer setzen
    if(aTeams[1] == true)
    {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

    //SSA aktivieren
    aSelfDefense[0]->TurnOn();
    aSelfDefense[1]->TurnOn();

    //Versorgungskiste (APW)
    CreateObject(AMCT, 5540, 1230, -1)->Set(ATWN);

    //Geschützstellungen
    aStationary[0] = CreateObject(GNET, 1795, 1141, -1);
    aStationary[0]->Set(0,90,1);
    aStationary[1] = CreateObject(GNET, 3410, 1130, -1);
    aStationary[1]->Set(0,-90,1);
    aStationary[2] = CreateObject(GNET, 3460, 890, -1);
    aStationary[2]->Set(SATW,-90,1);
    aStationary[3] = CreateObject(GNET, 3880, 660, -1);
    aStationary[3]->Set(SATW,-90);
    aStationary[4] = CreateObject(GNET, 5060, 1170, -1);
    aStationary[4]->Set(SATW);
    aTowerInterior[0] = CreateObject(GNET, 3640, 473, -1);
    aTowerInterior[0]->Set(SATW,-90);
    aTowerInterior[1] = CreateObject(GNET, 3700, 473, -1);
    aTowerInterior[1]->Set(SATW,90);

    //MAV-Station
    CreateObject(MVSN, 4805, 1210, -1)->Set(4885,1165,1);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1970,1140,-1),50*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5495,1200,-1),50*21);

      CreateObject(SGNP, 1640, 1170, -1)->Set("Helicopter",1);
      CreateObject(SGNP, 5405, 1270, -1)->Set("Helicopter");
    }

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1821,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,1904,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,3610,1269,-1),50*21);
  }
  else
  {
    //Leiter
    CreateObject(LADR, 4240, 795, -1)->Set(26, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

    if(!FindObject(GCTF))
    {
      //Leiter
      CreateObject(LADR, 3855, 950, -1)->Set(4, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");

      //Rampen
      DrawMaterialQuad("Tunnel-Rough",3800,930,3870,925,3868,990,3800,990,true);
      DrawMaterialQuad("Tunnel-Rough",3740,820,3795,815,3795,860,3720,845,true);
      DrawMaterialQuad("Tunnel-Rough",3970,900,4080,830,4060,905,3980,920,true);

      //Lianen
      SetCon(70, CreateObject(VINE, 3810, 925, -1));
      SetCon(100, CreateObject(VINE, 3840, 925, -1));

      //Büsche
      CreateObject(BSH2, 3780, 867, -1);
      CreateObject(BSH2, 3800, 1000, -1);
      CreateObject(BSH2, 4020, 923, -1);

      //Metallkiste
      CreateObject(MWCR, 4050, 910, -1)->AutoRespawn();

      //Gasflasche
      CreateObject(GSBL, 3825, 990, -1)->AutoRespawn();
    }
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 4010,530, -1);
    flag->~Set("$Flag3$");

    //Grenzen
    CreateObject(BRDR, 2890, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 6010, 0, -1)->Set(1);

    //Teamgrenzen
    CreateObject(BRDR, 3620, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 4600, 0, -1)->Set(1,1,0,1,2);

    //Selbstschussanlagen
    aSelfDefense[2] = CreateObject(SEGU, 3085, 1230, -1);
    aSelfDefense[2]->Set(0,1,1,180,0,3430);
    aSelfDefense[2]->SetTeam(1);
    aSelfDefense[3] = CreateObject(SEGU, 5450, 1230, -1);
    aSelfDefense[3]->Set(0,1,1,180,4850);
    aSelfDefense[3]->SetTeam(2);

    //Geschützstellungen
    CreateObject(GNET, 3260, 1210, -1)->Set(LCAC);
    CreateObject(GNET, 5060, 1170, -1)->Set(LCAC);

    //Hinweisschild
    CreateObject(SGNP, 6010, 1210, -1);

    //Leitern
    CreateObject(LADR, 2965, 1267, -1)->Set(4);
    CreateObject(LADR, 3115, 1267, -1)->Set(4);

    //Gerüste
    CreateObject(SFFG, 3010, 1280, -1)->Set(4);
    var plat = CreateObject(SFFG, 3010, 1330, -1);
    plat->Set(5);
    plat->SetClrModulation(RGB(125,125,250));
    CreateObject(SFFG, 3070, 1280, -1)->Set(4);
    plat = CreateObject(SFFG, 3070, 1330, -1);
    plat->Set(5);
    plat->SetClrModulation(RGB(125,125,250));

    //Boje
    CreateObject(BUOY, 2890, 1293,  -1);

    //Büsche
    CreateObject(BSH2, 3030, 1260, -1);
    CreateObject(BSH2, 3050, 1275, -1);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOHC))
    {
      SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,3045,1200,-1),50*21);
      SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,5495,1200,-1),50*21);

      CreateObject(SGNP, 3160, 1280, -1)->Set("Helicopter");
      CreateObject(SGNP, 5405, 1270, -1)->Set("Helicopter");
    }

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(3780, 980, [20]);
    AddMoneySpawn(4040, 1270, [20]);
    AddMoneySpawn(4080, 550, [20]);
    AddMoneySpawn(4480, 930, [20]);

    //Grenzen
    CreateObject(BRDR, 3130, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 5310, 0, -1)->Set(1);

    //Teamgrenzen
    CreateObject(BRDR, 3410, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 4670, 0, -1)->Set(1,1,0,1,2);

    //Hinweisschilder
    CreateObject(SGNP, 3160, 1280, -1);
    CreateObject(SGNP, 3260, 1210, -1);
    CreateObject(SGNP, 3420, 1130, -1);
    CreateObject(SGNP, 5065, 1170, -1);
    CreateObject(SGNP, 5100, 1250, -1);
    CreateObject(SGNP, 5180, 1240, -1);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
      {CreateFlag(1,3710,1050,GetTeamColor(1));}
    if(aTeams[2] == true)
      {CreateFlag(2,4480,940,GetTeamColor(2));}

    //Grenzen
    CreateObject(BRDR, 3130, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 5310, 0, -1)->Set(1);

    //Container
    CreateObject(CON1, 4420, 860, -1);
    CreateObject(CON1, 4420, 889, -1)->SetPerspective(2);
    CreateObject(CON1, 4125, 640, -1)->SetPerspective(2);

    //Hinweisschilder
    CreateObject(SGNP, 3160, 1280, -1);
    CreateObject(SGNP, 3260, 1210, -1);
    CreateObject(SGNP, 3420, 1130, -1);
    CreateObject(SGNP, 5065, 1170, -1);
    CreateObject(SGNP, 5100, 1250, -1);
    CreateObject(SGNP, 5180, 1240, -1);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);

    //Spawnpoint entfernen
    RemoveObject(FindObject2(Find_ID(RSPT),Find_InRect(4119, 1069, 2, 2)));
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
    //Grenzen
    CreateObject(BRDR, 3130, 0, -1)->Set(0,0,0,0,0,1);
    CreateObject(BRDR, 5310, 0, -1)->Set(1);

    //Hinweisschilder
    CreateObject(SGNP, 3160, 1280, -1);
    CreateObject(SGNP, 3260, 1210, -1);
    CreateObject(SGNP, 3420, 1130, -1);
    CreateObject(SGNP, 5065, 1170, -1);
    CreateObject(SGNP, 5100, 1250, -1);
    CreateObject(SGNP, 5180, 1240, -1);

    //Patrouillenboote
    SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3210,1269,-1),50*21);
    SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4235,1269,-1),50*21);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if(!iIndex || iIndex == 1)
  {
    if(fConnectedDestroyed)
    {
      //Grenzen neu setzen
      RemoveAll(BRDR);
      CreateObject(BRDR, 910, 0, -1)->Set(0,1);
      CreateObject(BRDR, 4580, 0, -1)->Set(1,1);

      //Teamgrenze
      FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3760,0,1,1);

      //Geschützstellungen entfernen
      aStationary[1]->DecoExplode(30);
      aStationary[2]->DecoExplode(30);
    }
  }

  //Ziel 3 und 4
  if(iIndex == 2 || iIndex == 3)
  {
    if(fConnectedDestroyed)
    {
      //Grenzen neu setzen
      RemoveAll(BRDR);
      CreateObject(BRDR, 910, 0, -1)->Set(0,1);
      CreateObject(BRDR, 5970, 0, -1)->Set(1,1);

      //Teamgrenze
      FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),4370,0,1,1);

      //Spawnpoints entfernen
      RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(1969, 1139, 2, 2)));
      RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(3609,1279, 2, 2)));

      //Geschützstellung entfernen
      aStationary[3]->DecoExplode(30);
    }
  }

  //Ziel 5, 6 und 7
  if(iIndex == 4 || iIndex == 5 || iIndex == 6)
  {
    if(fConnectedDestroyed)
    {
      //Geschützstellung entfernen
      aStationary[4]->DecoExplode(30);
    }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 3730; iY = 740;
    return 1;
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
    if(iTeam == 1)
      return [[3315, 1210], [3430, 1210], [3420, 1120], [3590, 1200]];
    if(iTeam == 2)
      return [[4720, 1230], [4810, 1200], [4820, 1090], [4890, 1160]];
    return 1;
  }
}