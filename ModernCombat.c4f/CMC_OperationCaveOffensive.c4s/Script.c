/*-- Operation Cave Offensive --*/

#strict 2
#include CSTD

static aDoorway,aDoor,aFlag,aSelfDefense,aFlagPosition;

func RecommendedGoals()			{return [GOCC];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Türverbindungen
  aDoorway = [];
  //Türen
  aDoor = [];
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  if(C4X_VerBuild < 331) {SetNextMission("ModernCombat.c4f\\CMC_OperationCaveOffensive.c4s", "$RepeatButton$", "$RepeatButtonDesc$");}
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Concrete2", 640, 1600, 760, 1570, 760, 1600, 640, 1600, 1);
  DrawMaterialQuad("Wall-Concrete2", 680, 1350, 760, 1430, 680, 1430, 680, 1350, 1);
  DrawMaterialQuad("Wall-Concrete2", 750, 1320, 810, 1300, 810, 1320, 750, 1320, 1);
  DrawMaterialQuad("Wall-Concrete2", 1180, 1530, 1240, 1490, 1240, 1530, 1180, 1530, 1);
  DrawMaterialQuad("EARTH-Smooth", 1200, 768, 1440, 768, 1440, 778, 1200, 778, 1);
  DrawMaterialQuad("Wall-Concrete2", 1430, 1300, 1480, 1260, 1480, 1310, 1430, 1300, 1);
  DrawMaterialQuad("Wall-Concrete3", 1440, 420, 1470, 410, 1470, 420, 1440, 420, 1);
  DrawMaterialQuad("Wall-Bricks2", 1480, 1380, 1520, 1330, 1520, 1380, 1480, 1380, 1);
  DrawMaterialQuad("Wall-Concrete2", 1720, 1450, 1740, 1430, 1750, 1430, 1770, 1450, 1);
  DrawMaterialQuad("Wall-Concrete2", 2240, 460, 2280, 490, 2240, 490, 2240, 460, 1);
  DrawMaterialQuad("Wall-Concrete2", 2240, 760, 2270, 750, 2270, 760, 2240, 760, 1);
  DrawMaterialQuad("Wall-Concrete2", 2360, 490, 2380, 510, 2360, 510, 2360, 490, 1);
  DrawMaterialQuad("Wall-Concrete3", 2370, 750, 2410, 740, 2410, 750, 2370, 750, 1);
  DrawMaterialQuad("Wall-Stripes", 2470, 1230, 2510, 1270, 2500, 1280, 2460, 1240, 1);

  DrawMaterialQuad("BackWall-Stripes", 2330, 1220, 3070, 1220, 3070, 1222, 2330, 1222, 1);
  DrawMaterialQuad("Earth-Smooth", 2340, 1230, 2325, 1215, 2260, 1280, 2340, 1230, 1);
  DrawMaterialQuad("Wall-Stripes", 2560, 1210, 2560, 1230, 2570, 1225, 2570, 1210, 1);
  DrawMaterialQuad("Earth-Smooth", 3060, 1230, 3075, 1215, 3140, 1280, 3060, 1230, 1);
  DrawMaterialQuad("Wall-Stripes", 2840, 1210, 2840, 1230, 2830, 1225, 2830, 1210, 1);

  DrawMaterialQuad("Wall-Stripes", 2930, 1230, 2890, 1270, 2900, 1280, 2940, 1240, 1);
  DrawMaterialQuad("Wall-Concrete3", 2990, 740, 3030, 750, 2990, 750, 2990, 740, 1);
  DrawMaterialQuad("Wall-Concrete2", 3020, 510, 3040, 490, 3040, 510, 3020, 510, 1);
  DrawMaterialQuad("Wall-Concrete2", 3120, 490, 3160, 460, 3160, 490, 3120, 490, 1);
  DrawMaterialQuad("Wall-Concrete2", 3140, 750, 3170, 760, 3140, 760, 3140, 750, 1);
  DrawMaterialQuad("Wall-Concrete2", 3660, 1290, 3710, 1260, 3710, 1290, 3660, 1290, 1);
  DrawMaterialQuad("Wall-Concrete2", 3750, 1260, 3900, 1350, 3830, 1370, 3750, 1300, 1);
  DrawMaterialQuad("Wall-Bricks2", 3960, 1250, 4000, 1280, 4000, 1300, 3960, 1270, 1);
  DrawMaterialQuad("EARTH-Smooth", 4060, 588, 4130, 588, 4130, 598, 4060, 598, 1);
  DrawMaterialQuad("Tunnel-SMOOTH2", 4120, 1500, 4240, 1500, 4240, 1490, 4120, 1490, 1);
  DrawMaterialQuad("Tunnel-SMOOTH2", 4270, 1500, 4350, 1500, 4350, 1510, 4270, 1510, 1);
  DrawMaterialQuad("Wall-Concrete2", 4300, 490, 4360, 450, 4360, 490, 4300, 490, 1);
  DrawMaterialQuad("Granite-ROCK", 4300, 608, 4425, 608, 4425, 618, 4300, 618, 1);
  DrawMaterialQuad("Wall-Concrete2", 4600, 490, 4640, 500, 4600, 500, 4600, 490, 1);
  DrawMaterialQuad("Wall-Concrete2", 4720, 500, 4760, 490, 4760, 500, 4720, 500, 1);
  DrawMaterialQuad("Wall-Concrete2", 4870, 471, 4890, 471, 4890, 440, 4870, 460, 1);

  //Leitern
  CreateObject(LADR, 645, 680, -1)->Set(24);
  CreateObject(LADR, 775, 677, -1)->Set(5);
  CreateObject(LADR, 775, 1470, -1)->Set(11);
  CreateObject(LADR, 800, 1300, -1)->Set(4);
  CreateObject(LADR, 800, 1570, -1)->Set(4);
  CreateObject(LADR, 900, 1570, -1)->Set(4);
  CreateObject(LADR, 930, 1506, -1)->Set(31);
  CreateObject(LADR, 947, 625, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1020, 720, -1)->Set(11);

  CreateObject(LADR, 1040, 1570, -1)->Set(4);
  CreateObject(LADR, 1170, 661, -1)->Set(15, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1245, 745, -1)->Set(14, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1405, 660, -1)->Set(5, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1380, 1490, -1)->Set(4);
  CreateObject(LADR, 1415, 1363, -1)->Set(17);
  CreateObject(LADR, 1445, 1435, -1)->Set(6);
  CreateObject(LADR, 1465, 700, -1)->Set(7, LADR_Left|LADR_Right|LADR_Front|LADR_Additional, "W");
  CreateObject(LADR, 1470, 1490, -1)->Set(4);
  CreateObject(LADR, 1515, 577, -1)->Set(20);
  CreateObject(LADR, 2215, 731, -1)->Set(33);

  CreateObject(LADR, 2545, 1218, -1)->Set(87);
  CreateObject(LADR, 2605, 610, -1)->Set(11);
  CreateObject(LADR, 2605, 800, -1)->Set(7);
  CreateObject(LADR, 2665, 610, -1)->Set(11);
  CreateObject(LADR, 2665, 800, -1)->Set(7);
  CreateObject(LADR, 2735, 610, -1)->Set(11);
  CreateObject(LADR, 2735, 800, -1)->Set(7);
  CreateObject(LADR, 2795, 610, -1)->Set(11);
  CreateObject(LADR, 2795, 800, -1)->Set(7);
  CreateObject(LADR, 2855, 1218, -1)->Set(87);

  CreateObject(LADR, 3185, 731, -1)->Set(33);
  CreateObject(LADR, 3730, 1356, -1)->Set(11);
  CreateObject(LADR, 3885, 1297, -1)->Set(20);
  CreateObject(LADR, 4025, 1479, -1)->Set(35);
  CreateObject(LADR, 4105, 560, -1)->Set(10);
  CreateObject(LADR, 4455, 555, -1)->Set(13);
  CreateObject(LADR, 4455, 1557, -1)->Set(40);
  CreateObject(LADR, 4580, 470, -1)->Set(12);
  CreateObject(LADR, 4615, 1302, -1)->Set(9);
  CreateObject(LADR, 4795, 577, -1)->Set(10);
  CreateObject(LADR, 4925, 577, -1)->Set(10);
  CreateObject(LADR, 3580, 1346, -1)->Set(6);

  //Bodenluken
  CreateObject(HA4K, 800, 1263, -1);
  CreateObject(HA4K, 800, 1533, -1);
  CreateObject(HA4K, 900, 1533, -1);
  CreateObject(HA4K, 1040, 1533, -1);
  CreateObject(HA4K, 1380, 1453, -1);
  CreateObject(HA4K, 1470, 1453, -1);
  CreateObject(HA4K, 3580, 1293, -1);
  CreateObject(HA4K, 3730, 1263, -1);

  //Große Bodenluken
  CreateObject(H24K, 645, 488, -1);
  CreateObject(H24K, 645, 638, -1);
  CreateObject(H24K, 775, 638, -1);
  CreateObject(H24K, 1415, 1228, -1);
  CreateObject(H24K, 1445, 1388, -1);
  CreateObject(H24K, 1515, 418, -1);
  CreateObject(H24K, 2215, 468, -1);
  CreateObject(H24K, 3185, 468, -1);
  CreateObject(H24K, 3885, 1138, -1);
  CreateObject(H24K, 3885, 1258, -1);
  CreateObject(H24K, 4025, 1288, -1);
  CreateObject(H24K, 4025, 1408, -1);
  CreateObject(H24K, 4455, 1238, -1);
  CreateObject(H24K, 4455, 1528, -1);
  CreateObject(H24K, 4795, 498, -1);
  CreateObject(H24K, 4925, 498, -1);

  //Gitter
  CreateObject(GTNG, 1547, 1330, -1);
  CreateObject(GTNG, 4648, 1420, -1);
  CreateObject(GTNG, 4873, 490, -1);
  CreateObject(GTNG, 4978, 490, -1);

  //Kisten
  CreateObject(WCR2, 820, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 4410, 580, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 2290, 750, -1)->AutoRespawn();
  CreateObject(MWCR, 3110, 750, -1)->AutoRespawn();
  CreateObject(MWCR, 1090, 740, -1)->AutoRespawn();
  CreateObject(MWCR, 2450, 510, -1)->AutoRespawn();
  CreateObject(MWCR, 2950, 510, -1)->AutoRespawn();
  CreateObject(MWCR, 4745, 1520, -1)->AutoRespawn();
  CreateObject(MWCR, 4850, 1560, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 560, 570, -1)->AutoRespawn();
  CreateObject(BECR, 570, 1550, -1)->AutoRespawn();
  CreateObject(BECR, 1070, 740, -1)->AutoRespawn();
  CreateObject(BECR, 4948, 580, -1)->AutoRespawn();
  CreateObject(BECR, 5020, 1580, -1)->AutoRespawn();
  CreateObject(BECR, 5120, 390, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 751, 1260, -1)->Set(0,1);
  CreateObject(SBBA, 771, 1530, -1)->Set(0,1);
  CreateObject(SBBA, 910, 740, -1)->Set(1,1);
  CreateObject(SBBA, 1065, 540, -1)->Set(1,1);
  CreateObject(SBBA, 1090, 1530, -1)->Set(1,1);
  CreateObject(SBBA, 1311, 1220, -1)->Set(0,1);
  CreateObject(SBBA, 1311, 1450, -1)->Set(0,1);
  CreateObject(SBBA, 1480, 730, -1)->Set(0,1);
  CreateObject(SBBA, 1540, 1220, -1)->Set(1,1);
  CreateObject(SBBA, 1835, 1210, -1)->Set(0,1);
  CreateObject(SBBA, 1835, 1490, -1)->Set(0,1);
  CreateObject(SBBA, 2600, 1210, -1)->Set(0,1);
  CreateObject(SBBA, 2800, 1210, -1)->Set(1,1);
  CreateObject(SBBA, 3790, 1400, -1)->Set(1,1);
  CreateObject(SBBA, 3990, 1400, -1)->Set(1,1);
  CreateObject(SBBA, 4040, 570, -1)->Set(1,1);
  CreateObject(SBBA, 4260, 490, -1)->Set(0,1);
  CreateObject(SBBA, 4640, 1370, -1)->Set(1,1);

  //Container
  var container = CreateObject(CON1, 845, 630, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 845, 1260, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  CreateObject(CON1, 3600, 1373, -1)->SetGraphics("6");
  CreateObject(CON1, 3600, 1400, -1)->SetGraphics("6");
  container = CreateObject(CON1, 3658, 1373, -1);
  container->SetPerspective(3);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3658, 1401, -1);
  container->SetPerspective(3);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3675, 1131, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3690, 1103, -1);
  container->SetPerspective(0);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3704, 1131, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3715, 1401, -1);
  container->SetPerspective(0);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3733, 1131, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3762, 1131, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 3950, 1200, -1);
  container->SetPerspective(3);
  container->SetGraphics("6");
  container = CreateObject(CON1, 4505, 1371, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 4650, 1561, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");
  container = CreateObject(CON1, 4666, 1321, -1);
  container->SetPerspective(2);
  container->SetGraphics("6");

  //Gasflaschen
  CreateObject(GSBL, 700, 580, -1)->AutoRespawn();
  CreateObject(GSBL, 1435, 420, -1)->AutoRespawn();
  CreateObject(GSBL, 2360, 750, -1)->AutoRespawn();
  CreateObject(GSBL, 3040, 750, -1)->AutoRespawn();
  CreateObject(GSBL, 3815, 400, -1)->AutoRespawn();
  CreateObject(GSBL, 3715, 1260, -1)->AutoRespawn();
  CreateObject(GSBL, 4660, 1420, -1)->AutoRespawn();
  CreateObject(GSBL, 4675, 1560, -1)->AutoRespawn();
  CreateObject(GSBL, 4745, 1370, -1)->AutoRespawn();
  CreateObject(GSBL, 4970, 340, -1);

  //Schutztüren
  CreateObject(GDDR, 820, 1300, -1);
  CreateObject(GDDR, 765, 1570, -1);
  CreateObject(GDDR, 1305, 1490, -1);
  CreateObject(GDDR, 1545, 1260, -1);
  CreateObject(GDDR, 4595, 1560, -1);
  CreateObject(GDDR, 4600, 580, -1);

  //Steine
  CreateObject(STNE, 435, 1350, -1);
  CreateObject(STNE, 1300, 770, -1);
  CreateObject(STNE, 1345, 610, -1);
  CreateObject(STNE, 4005,440, -1);
  CreateObject(STNE, 4310, 1510, -1);

  //Labortische
  CreateObject(LTBL, 525, 1350, -1);
  CreateObject(LTBL, 870, 1300, -1);
  CreateObject(LTBL, 1350, 1450, -1);
  CreateObject(LTBL, 1515, 1490, -1);
  CreateObject(LTBL, 4730, 580, -1);
  CreateObject(LTBL, 4860, 580, -1);

  //Monitore
  CreateObject(MNI2, 515, 1338, -1);
  CreateObject(MNI2, 535, 1338, -1);
  CreateObject(MNI2, 860, 1288, -1);
  CreateObject(MNI2, 880, 1288, -1);
  CreateObject(MNI2, 1340, 1438, -1);
  CreateObject(MNI2, 1360, 1438, -1);
  CreateObject(MNI2, 1505, 1478, -1);
  CreateObject(MNI2, 1460, 410, -1);
  CreateObject(MNI2, 1480, 410, -1);
  CreateObject(MNI2, 2290, 732, -1);
  CreateObject(MNI2, 3110, 732, -1);
  CreateObject(MNI2, 4720, 568, -1);
  CreateObject(MNI2, 4740, 568, -1);
  CreateObject(MNI2, 4850, 568, -1);
  CreateObject(MNI2, 4870, 568, -1);

  //Rohre
  CreateObject(PIPL, 880, 505, -1)->Left()->Left()->Down()->Down()->Down();
  CreateObject(PIPL, 1080, 345, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Down()->Down();
  CreateObject(PIPL, 1430, 465, -1)->Down()->Down()->Down()->Down()->Down()->Left()->Left()->Left()->Left()->Left()->Left()->Left();
  CreateObject(PIPL, 4120, 1175, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Down()->Down()->Down()->Down()->Down()->Down()->Down();
  CreateObject(PIPL, 4120, 1475, -1)->Right()->Right()->Right()->Right()->Right()->Right()->Right()->Down()->Right()->Right()->Right()->Right()->Down()->Right()->Right()->Right()->Up()->Up();

  //Explosivfässer
  CreateObject(XBRL, 690, 580, -1)->AutoRespawn();
  CreateObject(XBRL, 950, 1530, -1)->AutoRespawn();
  CreateObject(XBRL, 1120, 740, -1)->AutoRespawn();
  CreateObject(XBRL, 4570, 1520, -1)->AutoRespawn();
  CreateObject(XBRL, 4330, 580, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 970, 1530, -1)->AutoRespawn();
  CreateObject(PBRL, 1400, 1450, -1)->AutoRespawn();
  CreateObject(PBRL, 3910, 1250, -1)->AutoRespawn();
  CreateObject(PBRL, 4550, 1520, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 780, 1260, -1)->AutoRespawn();
  CreateObject(HBRL, 4560, 1230, -1)->AutoRespawn();

  //Paletten
  CreateObject(PLLT, 2439, 1230, -1);
  CreateObject(PLLT, 2962, 1230, -1);
  CreateObject(PLLT, 4410, 1520, -1);
  CreateObject(PLLT, 4546, 1370, -1);
  CreateObject(PLLT, 4680, 500, -1);

  //Explosivtanks
  CreateObject(XTNK, 525, 1300, -1)->AutoRespawn();
  CreateObject(XTNK, 840, 1530, -1)->AutoRespawn();
  CreateObject(XTNK, 1620, 1490, -1)->AutoRespawn();
  CreateObject(XTNK, 3750, 1102, -1)->AutoRespawn();
  CreateObject(XTNK, 4370, 580, -1)->AutoRespawn();
  CreateObject(XTNK, 4505, 1520, -1)->AutoRespawn();
  CreateObject(XTNK, 4770, 1420, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 890, 630, -1)->AutoRespawn();
  CreateObject(PTNK, 1145, 1530, -1)->AutoRespawn();
  CreateObject(PTNK, 4780, 400, -1)->AutoRespawn();

  //Scheinwerfer
  CreateObject(FLGH, 2570, 1210, -1)->SetRotation(65);
  CreateObject(FLGH, 2620, 1210, -1)->SetRotation(10);
  CreateObject(FLGH, 2780, 1210, -1)->SetRotation(-10);
  CreateObject(FLGH, 2830, 1210, -1)->SetRotation(-65);

  //Stahlbrücken
  CreateObject(_HBR, 845, 1392, -1)->SwitchMode(1);
  CreateObject(_HBR, 957, 1582, -1);
  CreateObject(_HBR, 1029, 1582, -1);
  CreateObject(_HBR, 1116, 552, -1);
  CreateObject(_HBR, 1375, 432, -1);
  CreateObject(_HBR, 2417, 522, -1)->SwitchMode(1);
  CreateObject(_HBR, 2984, 522, -1)->SwitchMode(1);
  CreateObject(_HBR, 4525, 1242, -1)->SwitchMode(1);

  //Gerüste
  CreateObject(SFFG, 540, 1600, -1)->Set(4);
  CreateObject(SFFG, 525, 1300, -1);
  CreateObject(SFFG, 525, 1350, -1)->Set(4);
  CreateObject(SFFG, 700, 530, -1)->Set(4);
  CreateObject(SFFG, 700, 580, -1)->Set(5);
  CreateObject(SFFG, 700, 630, -1)->Set(4);
  CreateObject(SFFG, 760, 580, -1)->Set(4);
  CreateObject(SFFG, 760, 630, -1)->Set(5);
  CreateObject(SFFG, 840, 1530, -1)->Set(4);
  CreateObject(SFFG, 900, 1530, -1)->Set(5);
  CreateObject(SFFG, 960, 690, -1)->Set(4);
  CreateObject(SFFG, 960, 740, -1)->Set(5);
  CreateObject(SFFG, 960, 1530, -1)->Set(5);
  CreateObject(SFFG, 1080, 1530, -1)->Set(4);
  CreateObject(SFFG, 1535, 360, -1)->Set(5);
  CreateObject(SFFG, 1535, 410, -1)->Set(5);
  CreateObject(SFFG, 1620, 1440, -1)->Set(4);
  CreateObject(SFFG, 1620, 1490, -1)->Set(5);
  CreateObject(SFFG, 2320, 490, -1)->Set(4);
  CreateObject(SFFG, 2655, 1060, -1)->Set(5);
  CreateObject(SFFG, 2655, 1110, -1)->Set(5);
  CreateObject(SFFG, 2655, 1160, -1)->Set(5);
  CreateObject(SFFG, 2655, 1210, -1)->Set(5);
  CreateObject(SFFG, 2700, 1210, -1)->Set(4);
  CreateObject(SFFG, 2745, 1060, -1)->Set(5);
  CreateObject(SFFG, 2745, 1110, -1)->Set(5);
  CreateObject(SFFG, 2745, 1160, -1)->Set(5);
  CreateObject(SFFG, 2745, 1210, -1)->Set(5);
  CreateObject(SFFG, 3080, 490, -1)->Set(4);
  CreateObject(SFFG, 3770, 350, -1)->Set(5);
  CreateObject(SFFG, 3770, 400, -1)->Set(5);
  CreateObject(SFFG, 4510, 1370, -1)->Set(5);
  CreateObject(SFFG, 4510, 1420, -1)->Set(4);
  CreateObject(SFFG, 4510, 1470, -1)->Set(5);
  CreateObject(SFFG, 4510, 1520, -1)->Set(4);
  CreateObject(SFFG, 4520, 430, -1)->Set(4);
  CreateObject(SFFG, 4520, 480, -1)->Set(4);
  CreateObject(SFFG, 4520, 530, -1)->Set(5);
  CreateObject(SFFG, 4520, 580, -1)->Set(2);
  CreateObject(SFFG, 4571, 1420, -1)->Set(4);
  CreateObject(SFFG, 4690, 1370, -1)->Set(4);
  CreateObject(SFFG, 4690, 1420, -1)->Set(5);
  CreateObject(SFFG, 4770, 1370, -1);
  CreateObject(SFFG, 4770, 1420, -1)->Set(4);
  CreateObject(SFFG, 4940, 340, -1)->Set(5);
  CreateObject(SFFG, 4940, 390, -1)->Set(2);
  CreateObject(SFFG, 4940, 440, -1)->Set(4);

  //Stahltore
  CreateObject(HNG4, 980, 200, -1);
  CreateObject(HNG4, 4500, 200, -1);

  //Verbundene Räume
  aDoorway[0] = CreateObject(ROOM, 1590, 600, -1);
  aDoorway[1] = CreateObject(ROOM, 600, 1350, -1);
  aDoorway[1]->Connect(aDoorway[0]);
  aDoorway[2] = CreateObject(ROOM, 1590, 760, -1);
  aDoorway[3] = CreateObject(ROOM, 600, 1600, -1);
  aDoorway[3]->Connect(aDoorway[2]);
  aDoorway[4] = CreateObject(ROOM, 1860, 1210, -1);
  aDoorway[5] = CreateObject(ROOM, 2320, 490, -1);
  aDoorway[5]->Connect(aDoorway[4]);
  aDoorway[6] = CreateObject(ROOM, 1860, 1490, -1);
  aDoorway[7] = CreateObject(ROOM, 2340, 750, -1);
  aDoorway[7]->Connect(aDoorway[6]);
  aDoorway[8] = CreateObject(ROOM, 2380, 1230, -1);
  aDoorway[9] = CreateObject(GAT1, 2450, 685, -1);
  aDoorway[9]->Connect(aDoorway[8]);
  aDoorway[10] = CreateObject(ROOM, 3020, 1230, -1);
  aDoorway[11] = CreateObject(GAT1, 2950, 685, -1);
  aDoorway[11]->Connect(aDoorway[10]);
  aDoorway[12] = CreateObject(ROOM, 3080, 490, -1);
  aDoorway[13] = CreateObject(ROOM, 3620, 1090, -1);
  aDoorway[13]->Connect(aDoorway[12]);
  aDoorway[14] = CreateObject(ROOM, 3060, 750, -1);
  aDoorway[15] = CreateObject(ROOM, 3620, 1290, -1);
  aDoorway[15]->Connect(aDoorway[14]);
  aDoorway[16] = CreateObject(ROOM, 4900, 1410, -1);
  aDoorway[17] = CreateObject(ROOM, 3940, 440, -1);
  aDoorway[17]->Connect(aDoorway[16]);
  aDoorway[18] = CreateObject(ROOM, 4900, 1560, -1);
  aDoorway[19] = CreateObject(ROOM, 3940, 580, -1);
  aDoorway[19]->Connect(aDoorway[18]);

  //Grenze
  CreateObject(BRDR, 0, 205, -1)->Set(2);

  //Sounds

  //Lava
  CreateObject(SE4K, 350, 1500, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1060, 1620, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1890, 490, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2110, 1180, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2705, 1280, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 3350, 1150, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 3520, 470, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 4240, 1580, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 5280, 1520, -1)->Set("Lava*",1200,350);

  //Wind
  CreateObject(SE4K, 410, 70, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 4700, 60, -1)->Set("WindSound*.ogg",3000,105);

  //Hallen
  CreateObject(SE4K, 920, 1470, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1440, 1380, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3970, 1205, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 4505, 1360, -1)->Set("Interior*.ogg",665,105);

  //Erdrutsche
  CreateObject(SE4K, 370, 390, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 340, 1220, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 5280, 300, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 5210, 1480, -1)->Set("FallingDirt*.ogg",950,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Doppelflinte)
  CreateObject(AMCT, 605, 700, -1)->Set(DBSG,0,0,0,1);
  CreateObject(AMCT, 5025, 500, -1)->Set(DBSG,0,0,0,1);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 1495, 1220, -1)->Set(ATWN);
  CreateObject(AMCT, 3940, 1172, -1)->Set(ATWN);

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 750, 800, -1)->Set(ABOX);
  CreateObject(AMCT, 4570, 580, -1)->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  CreateObject(AMCT, 850, 1570, -1)->Set(GBOX);
  CreateObject(AMCT, 4505, 1560, -1)->Set(GBOX);

  //Versorgungskisten (Dragnin)
  CreateObject(AMCT, 2115, 430, -1)->Set(DGNN);
  CreateObject(AMCT, 3285, 430, -1)->Set(DGNN);

  //Raketen
  PlaceSpawnpoint(MBOX, 300, 1425);
  PlaceSpawnpoint(MBOX, 5155, 1455);

  //Geschützstellungen
  CreateObject(GNET, 2460, 740, -1)->Set(0,90);
  CreateObject(GNET, 2940, 740, -1)->Set(0,-90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Raketen
  var rocket = CreateObject(X5RK, 985, 1530, -1);
  rocket->SetCategory(C4D_StaticBack);
  rocket->SetCon(75);
  CreateObject(X5RK, 2700, 1210, -1)->SetCategory(C4D_StaticBack);
  rocket = CreateObject(X5RK, 4500, 1520, -1);
  rocket->SetCategory(C4D_StaticBack);
  rocket->SetCon(60);

  //Radare
  CreateObject(RADR, 695, 480, -1);
  CreateObject(RADR, 4275, 490, -1);

  //Orientierungslichter
  CreateObject(OLGH, 2700, 1210, -1)->Set(6, 15, 1, 1, 40);

  //Lianen
  SetCon(80, CreateObject(VINE, 530, 315, -1));
  SetCon(50, CreateObject(VINE, 555, 340, -1));
  SetCon(100, CreateObject(VINE, 590, 350, -1));
  SetCon(90, CreateObject(VINE, 650, 400, -1));
  SetCon(100, CreateObject(VINE, 475, 1460, -1));
  SetCon(75, CreateObject(VINE, 850, 650, -1));
  SetCon(50, CreateObject(VINE, 860, 650, -1));
  SetCon(60, CreateObject(VINE, 1060, 650, -1));
  SetCon(100, CreateObject(VINE, 1070, 650, -1));
  SetCon(50, CreateObject(VINE, 1140, 1020, -1));
  SetCon(90, CreateObject(VINE, 1160, 1015, -1));
  SetCon(100, CreateObject(VINE, 1175, 1020, -1));
  SetCon(50, CreateObject(VINE, 1190, 1010, -1));
  SetCon(75, CreateObject(VINE, 1930, 1355, -1));
  SetCon(90, CreateObject(VINE, 1940, 1360, -1));
  SetCon(50, CreateObject(VINE, 1955, 1370, -1));
  SetCon(90, CreateObject(VINE, 4220, 260, -1));
  SetCon(45, CreateObject(VINE, 4290, 510, -1));
  SetCon(70, CreateObject(VINE, 4330, 510, -1));
  SetCon(90, CreateObject(VINE, 4380, 510, -1));
  SetCon(80, CreateObject(VINE, 4640, 200, -1));
  SetCon(100, CreateObject(VINE, 4665, 430, -1));
  SetCon(60, CreateObject(VINE, 4780, 450, -1));
  SetCon(100, CreateObject(VINE, 4940, 240, -1));
  SetCon(100, CreateObject(VINE, 5010, 1165, -1));
  SetCon(70, CreateObject(VINE, 5110, 1380, -1));

  //Büsche
  CreateObject(BSH2, 350, 1480, -1);
  CreateObject(BSH2, 530, 335, -1);
  CreateObject(BSH2, 775, 715, -1);
  CreateObject(BSH2, 475, 1480, -1);
  CreateObject(BSH2, 1620, 1220, -1);
  CreateObject(BSH2, 1680, 1160, -1);
  CreateObject(BSH2, 1680, 1220, -1);
  CreateObject(BSH2, 1710, 1500, -1);
  CreateObject(BSH2, 1735, 1500, -1);
  CreateObject(BSH2, 1740, 1220, -1);
  CreateObject(BSH2, 1760, 1500, -1);
  CreateObject(BSH2, 1800, 1220, -1);
  CreateObject(BSH2, 2235, 1265, -1);
  CreateObject(BSH2, 2255, 1275, -1);
  CreateObject(BSH2, 2290, 1265, -1);
  CreateObject(BSH2, 3110, 1265, -1);
  CreateObject(BSH2, 3140, 1275, -1);
  CreateObject(BSH2, 3160, 1265, -1);
  CreateObject(BSH2, 3790, 1140, -1);
  CreateObject(BSH2, 3810, 1140, -1);
  CreateObject(BSH2, 3830, 1140, -1);
  CreateObject(BSH2, 3850, 1140, -1);
  CreateObject(BSH2, 4205, 575, -1);
  CreateObject(BSH2, 4230, 275, -1);
  CreateObject(BSH2, 5010, 1180, -1);
  CreateObject(BSH2, 5035, 1590, -1);

  //Lampen
  CreateObject(CLGH,865,1270,-1);
  CreateObject(BLGH,900,490,-1);
  CreateObject(BLGH,900,1470,-1);
  CreateObject(BLGH,1060,490,-1);
  CreateObject(BLGH,1060,1180,-1);
  CreateObject(BLGH,1060,1470,-1);
  CreateObject(BLGH,1380,1180,-1);
  CreateObject(BLGH,1380,1420,-1);
  CreateObject(BLGH,1500,1180,-1);
  CreateObject(BLGH,1500,1420,-1);
  CreateObject(CLGH,1532,318,-1);
  CreateObject(CLGH,1618,1400,-1);
  CreateObject(CLGH,1500,1235,-1);
  CreateObject(BLGH,2380,1150,-1);
  CreateObject(BLGH,2450,710,-1);
  CreateObject(BLGH,2950,710,-1);
  CreateObject(BLGH,3020,1150,-1);
  CreateObject(CLGH,3950,1225,-1);
  CreateObject(BLGH,4420,1180,-1);
  CreateObject(BLGH,4420,1470,-1);
  CreateObject(BLGH,4580,1180,-1);
  CreateObject(CLGH,4720,525,-1);
  CreateObject(CLGH,4860,525,-1);
  var lamp = CreateObject(ALGH, 870, 660, -1);
  lamp->SetR(180);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(50,50,255));
  lamp = CreateObject(ALGH, 1345, 1340, -1);
  lamp->SetR(-90);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(50,50,255));
  lamp = CreateObject(ALGH, 2465, 811, -1);
  lamp->SetR(180);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(50,50,255));
  lamp = CreateObject(ALGH, 2935, 811, -1);
  lamp->SetR(180);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(255,255,50));
  lamp = CreateObject(ALGH, 4125, 1180, -1);
  lamp->SetR(90);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(255,255,50));
  lamp = CreateObject(ALGH, 4555, 510, -1);
  lamp->SetR(-90);
  lamp->TurnOn();
  lamp->ChangeLightColor(RGB(255,255,50));

  //Straßenlichter
  CreateObject(SLGH, 660, 1350, -1);
  CreateObject(SLGH, 695, 1587, -1);
  CreateObject(SLGH, 1880, 1210, -1);
  CreateObject(SLGH, 1880, 1490, -1);
  CreateObject(SLGH, 2490, 510, -1);
  CreateObject(SLGH, 2910, 510, -1);
  CreateObject(SLGH, 3600, 1290, -1);
  CreateObject(SLGH, 3600, 1090, -1);
  CreateObject(SLGH, 4760, 1520, -1);
  CreateObject(SLGH, 4830, 1420, -1);

  //Notausgangslichter
  CreateObject(ETLT, 600, 1325, -1);
  CreateObject(ETLT, 600, 1575, -1);
  CreateObject(ETLT, 2340, 725, -1);
  CreateObject(ETLT, 3060, 725, -1);
  CreateObject(ETLT, 4900, 1385, -1);
  CreateObject(ETLT, 4900, 1535, -1);

  //Papierstapel
  CreateObject(PPSK, 840, 1300, -1);
  CreateObject(PPSK, 1325, 1450, -1);
  CreateObject(PPSK, 1350, 1450, -1);
  CreateObject(PPSK, 1525, 1477, -1);
  CreateObject(PPSK, 1545, 1490, -1);
  CreateObject(PPSK, 4760, 580, -1);
  CreateObject(PPSK, 4910, 580, -1);
  CreateObject(PPSK, 4920, 580, -1);

  //Ventillatoren
  CreateObject(VENT, 980, 350, -1)->SetCon(80);
  CreateObject(VENT, 980, 860, -1)->SetCon(80);
  CreateObject(VENT, 980, 1370, -1)->SetCon(80);
  CreateObject(VENT, 1440, 1378, -1)->SetCon(50);
  CreateObject(VENT, 2400, 570, -1)->SetCon(30);
  CreateObject(VENT, 2400, 610, -1)->SetCon(30);
  CreateObject(VENT, 3000, 570, -1)->SetCon(30);
  CreateObject(VENT, 3000, 610, -1)->SetCon(30);
  CreateObject(VENT, 4040, 1240, -1)->SetCon(60);
  CreateObject(VENT, 4500, 730, -1)->SetCon(80);
  CreateObject(VENT, 4500, 1370, -1)->SetCon(80);

  //Hinweisschilder
  CreateObject(SGNP, 300, 270, -1);
  CreateObject(WLSN, 2140, 410, -1)->Set("Health","Wood");
  CreateObject(SGNP, 2625, 120, -1);
  CreateObject(WLSN, 3260, 410, -1)->Set("Health","Wood");
  CreateObject(SGNP, 4940, 160, -1);

  //Poster
  CreateObject(PSTR, 1175, 1510, -1)->Set(3,1);
  CreateObject(PSTR, 1175, 1500, -1)->Set(3,2);
  CreateObject(PSTR, 1380, 1310, -1)->Set(3,1);
  CreateObject(PSTR, 1860, 1440, -1)->Set(3,2);
  CreateObject(PSTR, 1864, 1433, -1)->Set(3,0);

  //Lüftungsgitter
  CreateObject(ENGT, 2320, 670, -1);
  CreateObject(ENGT, 3080, 670, -1);
  CreateObject(ENGT, 4930, 570, -1);

  //Bildschirme
  var screen = CreateObject(SCR3, 1515, 395, -1);
  screen->SetClrModulation(RGB(255));
  screen->SetPhase(6);
  CreateObject(SCA2, 1535, 360, -1)->SetClips([12]);
  CreateObject(SCR3, 1555, 395, -1);

  screen = CreateObject(SCR3, 1590, 570, -1);
  screen->SetClrModulation(RGB(90,90,250));
  screen->SetPhase(6);
  CreateObject(SCR3, 1590, 730, -1)->SetClrModulation(RGB(90,90,250));

  CreateObject(SCR3, 2320, 460, -1)->SetClrModulation(RGB(90,90,250));
  screen = CreateObject(SCR3, 2340, 710, -1);
  screen->SetClrModulation(RGB(90,90,250));
  screen->SetPhase(5);
  CreateObject(SCR3, 2380, 1190, -1)->SetClrModulation(RGB(250,250,250));

  screen = CreateObject(SCR3, 3080, 460, -1);
  screen->SetClrModulation(RGB(250,250,50));
  screen->SetPhase(5);

  CreateObject(SCR3, 3020, 1190, -1)->SetClrModulation(RGB(250,250,250));
  CreateObject(SCR3, 3060, 710, -1)->SetClrModulation(RGB(250,250,50));

  screen = CreateObject(SCR3, 3745, 385, -1);
  screen->SetClrModulation(RGB(255));
  screen->SetPhase(6);
  CreateObject(SCA2, 3770, 350, -1)->SetClips([13]);
  CreateObject(SCR3, 3790, 385, -1);

  screen = CreateObject(SCR3, 3940, 410, -1);
  screen->SetClrModulation(RGB(250,250,50));
  screen->SetPhase(7);
  CreateObject(SCR3, 3940, 550, -1)->SetClrModulation(RGB(250,250,50));

  //Geländer
  CreateObject(RAI1, 1410, 420, -1);
  CreateObject(RAI1, 1939, 1210, -1)->SetRail([1,1]);
  CreateObject(RAI1, 2029, 1160, -1)->SetRail([1,1]);
  CreateObject(RAI1, 2109, 1160, -1)->SetRail([1,1]);
  CreateObject(RAI1, 2130, 430, -1);
  CreateObject(RAI1, 2598, 1210, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 3245, 430, -1);
  CreateObject(RAI1, 3249, 1130, -1)->SetRail([1,1]);
  CreateObject(RAI1, 3329, 1130, -1)->SetRail([1,1]);
  CreateObject(RAI1, 3479, 1090, -1)->SetRail([1,1]);
  CreateObject(RAI1, 3710, 1260, -1)->SetRail([1,1]);
  CreateObject(RAI1, 4895, 440, -1)->SetRail([1,1,1,1]);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(FindObject(GOCC))
  {
    if(pPoint == aFlag[1])
    {
      if(aSelfDefense[0])
        aSelfDefense[0]->SetTeam(iTeam);

      if(iTeam == 1)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(400,1350);
        aFlag[1] -> AddSpawnPoint(425,1530);
        aFlag[1] -> AddSpawnPoint(465,1340);
        aFlag[1] -> AddSpawnPoint(510,1540);
      }
      if(iTeam == 2)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(1450,1270);
        aFlag[1] -> AddSpawnPoint(1500,1345);
        aFlag[1] -> AddSpawnPoint(1500,1440);
        aFlag[1] -> AddSpawnPoint(1520,1210);
      }
    }

    if(pPoint == aFlag[2])
    {
      if(iTeam == 1)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(2140,420);
        aFlag[1] -> AddSpawnPoint(2240,450);
        aFlag[1] -> AddSpawnPoint(2190,680);
        aFlag[1] -> AddSpawnPoint(2235,750);
      }
      if(iTeam == 2)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(3260,420);
        aFlag[1] -> AddSpawnPoint(3160,450);
        aFlag[1] -> AddSpawnPoint(3210,680);
        aFlag[1] -> AddSpawnPoint(3175,750);
      }
    }

    if(pPoint == aFlag[3])
    {
      if(aSelfDefense[1])
        aSelfDefense[1]->SetTeam(iTeam);

      if(iTeam == 1)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(3800,1120);
        aFlag[1] -> AddSpawnPoint(3860,1240);
        aFlag[1] -> AddSpawnPoint(3710,1360);
        aFlag[1] -> AddSpawnPoint(3760,1390);
      }
      if(iTeam == 2)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(4705,1310);
        aFlag[1] -> AddSpawnPoint(4715,1510);
        aFlag[1] -> AddSpawnPoint(4560,1550);
        aFlag[1] -> AddSpawnPoint(4605,1410);
      }
    }
  }
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

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1140,740,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(640, 700);
    aFlag[0] -> AddSpawnPoint(755, 620);
    aFlag[0] -> AddSpawnPoint(845, 590);
    aFlag[0] -> AddSpawnPoint(825, 740);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",0,4);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$");
    }

    aFlag[1] = CreateObject(OFPL,1190,1427,NO_OWNER);
    aFlag[1] -> AddSpawnPoint(740, 1310);
    aFlag[1] -> AddSpawnPoint(820, 1250);
    aFlag[1] -> AddSpawnPoint(1380, 1310);
    aFlag[1] -> AddSpawnPoint(1500, 1250);
    aFlag[1]->Set("$Flag2$");

    aFlag[2] = CreateObject(OFPL,2700,770,NO_OWNER);
    aFlag[2] -> SetClrModulation(RGB(150,150,150));
    aFlag[2] -> AddSpawnPoint(2290,1240);
    aFlag[2] -> AddSpawnPoint(2570,1200);
    aFlag[2] -> AddSpawnPoint(2830,1200);
    aFlag[2] -> AddSpawnPoint(3110,1240);
    aFlag[2]->Set("$Flag3$");

    aFlag[3] = CreateObject(OFPL,4235,1448,NO_OWNER);
    aFlag[3] -> AddSpawnPoint(3940, 1390);
    aFlag[3] -> AddSpawnPoint(3960, 1240);
    aFlag[3] -> AddSpawnPoint(4580, 1220);
    aFlag[3] -> AddSpawnPoint(4610, 1360);
    aFlag[3]->Set("$Flag4$");

    aFlag[4] = CreateObject(OFPL,4400,450,NO_OWNER);
    aFlag[4] -> AddSpawnPoint(4615,485);
    aFlag[4] -> AddSpawnPoint(4720,410);
    aFlag[4] -> AddSpawnPoint(4760,480);
    aFlag[4] -> AddSpawnPoint(4760,570);
    if(aTeams[2] == true)
    {
      aFlag[4]->Set("$Flag5$",0,4);
      aFlag[4]->Capture(2,1);
    }
    else
    {
      aFlag[4]->Set("$Flag5$");
    }

    //Selbstschussanlagen und Konsolen
    aSelfDefense[0] = CreateObject(SEGU, 1060, 1220, -1);
    aSelfDefense[0]->SetCategory(C4D_StaticBack);
    aSelfDefense[1] = CreateObject(SEGU, 4235, 1175, -1);
    aSelfDefense[1]->SetCategory(C4D_StaticBack);

    //SSA Besitzer setzen
    if(aTeams[1] == true)
      aSelfDefense[0]->SetTeam(1);
    if(aTeams[2] == true)
      aSelfDefense[1]->SetTeam(2);

    //SSA aktivieren
    aSelfDefense[0]->TurnOn();
    aSelfDefense[1]->TurnOn();

    //Hinweisschilder
    CreateObject(SGNP, 1235, 1430, -1)->Set("Turret",1,1);
    CreateObject(SGNP, 4280, 1410, -1)->Set("Turret",1,1);

    //Geschützstellungen
    CreateObject(GNET, 4495, 380, -1)->Set(SATW,-90,1);
    CreateObject(GNET, 4520, 1470, -1)->Set(SBYW,-90);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOBH))
    {
      SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,2695,1140,-1),100*21);

      CreateObject(SGNP, 2520, 1210, -1)->Set("Helicopter",1,1);
      CreateObject(SGNP, 2880, 1210, -1)->Set("Helicopter",1,1);
    }
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Script starten
    ScriptGo(1);
    aFlagPosition = 2;

    //Rampen
    DrawMaterialQuad("Wall-Stripes", 1240, 960, 1280, 960, 1280, 970, 1240, 970, 1);
    DrawMaterialQuad("Wall-Bricks2", 1240, 970, 1280, 970, 1280, 1490, 1240, 1490, 1);
    DrawMaterialQuad("Wall-Stripes", 1240, 1490, 1280, 1490, 1280, 1500, 1240, 1500, 1);
    DrawMaterialQuad("Wall-Stripes", 1800, 210, 1840, 210, 1840, 220, 1800, 220, 1);
    DrawMaterialQuad("Wall-Bricks2", 1800, 220, 1840, 220, 1840, 600, 1800, 600, 1);
    DrawMaterialQuad("Wall-Stripes", 1800, 600, 1840, 600, 1840, 610, 1800, 610, 1);
    DrawMaterialQuad("Wall-Stripes", 3600, 120, 3640, 120, 3640, 130, 3600, 130, 1);
    DrawMaterialQuad("Wall-Bricks2", 3600, 130, 3640, 130, 3640, 540, 3600, 540, 1);
    DrawMaterialQuad("Wall-Stripes", 3600, 540, 3640, 540, 3640, 550, 3600, 550, 1);
    DrawMaterialQuad("Wall-Stripes", 4080, 790, 4120, 790, 4120, 800, 4080, 800, 1);
    DrawMaterialQuad("Wall-Bricks2", 4080, 800, 4120, 800, 4120, 1500, 4080, 1500, 1);
    DrawMaterialQuad("Wall-Stripes", 4080, 1500, 4120, 1500, 4120, 1510, 4080, 1510, 1);

    //Flaggenposten
    aFlag[0] = CreateObject(OFPL, 2700,770, -1);
    aFlag[0]->SetClrModulation(RGB(150,150,150));
    aFlag[0]->Set("$Flag3$",150);

    //Teamgrenzen
    CreateObject(BRDR, 2090, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 3310, 0, -1)->Set(1,1,0,1,2);

    //Türverbindungen sperren
    aDoorway[0]->SealEntrance();
    aDoorway[1]->SealEntrance();
    aDoorway[2]->SealEntrance();
    aDoorway[3]->SealEntrance();
    aDoorway[16]->SealEntrance();
    aDoorway[17]->SealEntrance();
    aDoorway[18]->SealEntrance();
    aDoorway[19]->SealEntrance();

    //Versorgungskiste (Doppelflinte)
    CreateObject(AMCT, 2695, 1210, -1)->Set(DBSG,0,0,0,1);

    //Helikopter und Hinweisschilder
    if(!FindObject(NOBH))
    {
      SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,1375,1200,-1),100*21);
      SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,3690,1050,-1),100*21);

      CreateObject(SGNP, 1440, 1220, -1)->Set("Helicopter",1);
      CreateObject(SGNP, 3580, 1090, -1)->Set("Helicopter",1);
    }

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskisten (Kugeln)
      CreateObject(AMCT, 1380, 1320, -1)->Set(ABOX);
      CreateObject(AMCT, 3950, 1250, -1)->Set(ABOX);

      //Kugeln
      PlaceSpawnpoint(ABOX, 2215, 1225);
      PlaceSpawnpoint(ABOX, 3185, 1225);

      //Gewehrgranaten
      PlaceSpawnpoint(GBOX, 1960, 1205);
      PlaceSpawnpoint(GBOX, 3500, 1085);

      //Raketen
      PlaceSpawnpoint(MBOX, 1745, 1425);
      PlaceSpawnpoint(MBOX, 3620, 1340);
    }
  }
  else
  {
    //Geschützstellungen
    CreateObject(GNET, 865, 1480, -1)->Set(SBYW,90);
    CreateObject(GNET, 875, 1260, -1)->Set(SATW,90);
    CreateObject(GNET, 1065, 630, -1)->Set(SATW,90,1);
    CreateObject(GNET, 4410, 1230, -1)->Set(SATW,-90);

    //Aufzüge
    CreateObject(LFTP, 945, 1395, -1)->DisableVertical();
    CreateObject(LFTP, 4370, 1365, -1)->DisableVertical();

    //Leiter
    CreateObject(LADR, 4135, 1460, -1)->Set(22);

    //Container
    var container = CreateObject(CON1, 4090, 1131, -1);
    container->SetPerspective(0);
    container->SetGraphics("6");

    //Phosphorfass
    CreateObject(HBRL, 4090, 1400, -1)->AutoRespawn();

    //Phosphortank
    CreateObject(PTNK, 4080, 1280, -1)->AutoRespawn();

    //Sandsackbarrieren
    CreateObject(SBBA, 4105, 1450, -1)->Set(1,1);
    CreateObject(SBBA, 4110, 1500, -1)->Set(1,1);

    //Gerüst
    CreateObject(SFFG, 4080, 1500, -1)->Set(4);

    //Lampen
    CreateObject(BLGH,4100,1090,-1);
    CreateObject(BLGH,4100,1470,-1);

    //Rohre
    CreateObject(PIPL, 1080, 1145, -1)->Right()->Right()->Right()->Right()->Right()->Down()->Down();
    CreateObject(PIPL, 1080, 1455, -1)->Right()->Right()->Right()->Right()->Right()->Up()->Up()->Up()->Up();

    //Zusätzliche Munition
    if(!FindObject(NOAM))
    {
      //Versorgungskisten (Kugeln)
      CreateObject(AMCT, 1425, 1490, -1)->Set(ABOX);
      CreateObject(AMCT, 4065, 1500, -1)->Set(ABOX);
    }
  }

  //AS-Spielziel
  if(FindObject(GASS))
  {
    //Zielobjekte
    AddAssaultTarget(PMP2, 1165, 690, 30*30, 2, "$Target1$", 0, [[[745, 1320], [775, 1530], [800, 1380]], [[475, 450], [590, 480], [670, 480]]]);
    AddAssaultTarget(PMP2, 1070, 1445, 30*30, 2, "$Target2$", 1, [[[1680, 1150], [1740, 1210], [1610, 1390]], [[1370, 610], [1360, 770], [1540, 410]]]);
    AddAssaultTarget(RADR, 2700, 775, 0, 2, "$Target3$", 2, [[[3820, 1130], [3860, 1250], [3705, 1370]], [[1520, 1220], [1510, 1260], [1500, 1355]]]);
    AddAssaultTarget(CMSN, 4215, 1500, 0, 2, "$Target4$", 3, [[[4860, 1410], [4820, 1560], [4605, 1420]], [[3260, 430], [3210, 690], [3075, 1220]]]);
    AddAssaultTarget(CCP2, 4380, 450, 0, 2, "$Target5$", 4, [[[4815, 580], [4900, 490], [4950, 390]], [[4580, 1230], [4620, 1370], [4620, 1560]]]);

    //Rampen
    DrawMaterialQuad("Wall-Bricks2", 880, 1040, 920, 1040, 920, 1200, 880, 1200, 1);
    DrawMaterialQuad("Wall-Stripes", 880, 1180, 920, 1180, 920, 1190, 880, 1190, 1);
    DrawMaterialQuad("Wall-Bricks2", 880, 1400, 920, 1400, 920, 1410, 880, 1410, 1);
    DrawMaterialQuad("Wall-Bricks2", 880, 1340, 920, 1340, 915, 1350, 885, 1350, 1);
    DrawMaterialQuad("Wall-Stripes", 970, 740, 1000, 740, 1000, 751, 970, 751, 1);
    DrawMaterialQuad("Wall-Stripes", 1800, 210, 1840, 210, 1840, 220, 1800, 220, 1);
    DrawMaterialQuad("Wall-Bricks2", 1800, 220, 1840, 220, 1840, 600, 1800, 600, 1);
    DrawMaterialQuad("Wall-Stripes", 1800, 600, 1840, 600, 1840, 610, 1800, 610, 1);
    DrawMaterialQuad("Wall-Stripes", 1805, 960, 1825, 960, 1825, 970, 1805, 970, 1);
    DrawMaterialQuad("BackWall-Metal3", 1810, 970, 1820, 970, 1820, 1150, 1810, 1150, 1);
    DrawMaterialQuad("Wall-Stripes", 3975, 770, 3995, 770, 3995, 780, 3975, 780, 1);
    DrawMaterialQuad("BackWall-Metal3", 3980, 780, 3990, 780, 3990, 900, 3980, 900, 1);
    DrawMaterialQuad("Wall-Bricks2", 3960, 1270, 3970, 1270, 3970, 1280, 3960, 1280, 1);
    DrawMaterialQuad("Wall-Stripes", 4480, 580, 4510, 580, 4510, 591, 4480, 591, 1);

    //Grenze
    CreateObject(BRDR, 0, 205, -1)->Set(2);

    //Teamgrenze
    CreateObject(BRDR, 0, 810, -1)->Set(3,1,0,1,2);

    //Geschützstellungen
    CreateObject(GNET, 1365, 1320, -1)->Set(SATW,-90);
    CreateObject(GNET, 4495, 380, -1)->Set(0,-90);
    CreateObject(GNET, 4685, 580, -1)->Set(LCAC,-90);

    //Versorgungskisten (Dragnin)
    CreateObject(AMCT, 4440, 580, -1)->Set(DGNN);

    //Hydrauliktüren und Tore
    aDoor[00] = CreateObject(SEDR, 905, 1300,-1);
    aDoor[00]->Lock();
    aDoor[01] = CreateObject(SEDR, 905, 1380,-1);
    aDoor[01]->Lock();
    aDoor[02] = CreateObject(HNG3,915,1260,-1);
    aDoor[02]->SwitchDestructability();
    aDoor[03] = CreateObject(HNG3,915,1470,-1);
    aDoor[03]->SwitchDestructability();
    aDoor[04] = CreateObject(HNG3,915,1530,-1);
    aDoor[04]->SwitchDestructability();
    aDoor[05] = CreateObject(SEDR, 919, 1570,-1);
    aDoor[05]->Lock();

    aDoor[06] = CreateObject(HNG3,1815,1030,-1);
    aDoor[06]->SwitchDestructability();
    aDoor[07] = CreateObject(HNG3,1815,1090,-1);
    aDoor[07]->SwitchDestructability();
    aDoor[08] = CreateObject(HNG3,1815,1150,-1);
    aDoor[08]->SwitchDestructability();
    aDoor[09] = CreateObject(HNG3,1815,1210,-1);
    aDoor[09]->SwitchDestructability();
    aDoor[10] = CreateObject(HNG3,1815,1430,-1);
    aDoor[10]->SwitchDestructability();
    aDoor[11] = CreateObject(HNG3,1815,1490,-1);
    aDoor[11]->SwitchDestructability();

    aDoor[12] = CreateObject(SEDR, 3930, 1250,-1);
    aDoor[12]->Lock();
    aDoor[13] = CreateObject(HNG3,3965,1340,-1);
    aDoor[13]->SwitchDestructability();
    aDoor[14] = CreateObject(HNG3,3965,1400,-1);
    aDoor[14]->SwitchDestructability();
    aDoor[15] = CreateObject(HNG3,3984,840,-1);
    aDoor[15]->SwitchDestructability();
    aDoor[16] = CreateObject(HNG3,3984,900,-1);
    aDoor[16]->SwitchDestructability();
    aDoor[17] = CreateObject(HNG3,3984,960,-1);
    aDoor[17]->SwitchDestructability();
    aDoor[18] = CreateObject(HNG3,3984,1020,-1);
    aDoor[18]->SwitchDestructability();
    aDoor[19] = CreateObject(HNG3,3984,1080,-1);
    aDoor[19]->SwitchDestructability();
    aDoor[20] = CreateObject(HNG3,3984,1140,-1);
    aDoor[20]->SwitchDestructability();
    aDoor[21] = CreateObject(HNG3,3984,1200,-1);
    aDoor[21]->SwitchDestructability();

    //Container
    CreateObject(CON1, 1430, 770, -1);

    //Sandsackbarrieren
    CreateObject(SBBA, 725, 480, -1)->Set(1,1);
    CreateObject(SBBA, 725, 580, -1)->Set(1,1);
    CreateObject(SBBA, 725, 630, -1)->Set(1,1);
    CreateObject(SBBA, 789, 530, -1)->Set(1,1);
    CreateObject(SBBA, 1250, 1490, -1)->Set(0,1);
    CreateObject(SBBA, 1415, 741, -1)->Set(0,1);
    CreateObject(SBBA, 1530, 600, -1)->Set(0,1);
  }
  else
  {
    //Schutztür
    CreateObject(GDDR, 3930, 1250, -1);

    //Lampen
    CreateObject(BLGH,900,1180,-1);
    CreateObject(BLGH,3980,1090,-1);

    //Große Bodenluken
    CreateObject(H24K, 985, 748, -1);
    CreateObject(H24K, 4495, 588, -1);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1
  if(!iIndex)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 0, 205, -1)->Set(2);

    //Teamgrenze
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),0,810,3,1);

    //Türen öffnen
    aDoor[00]->Open();
    aDoor[01]->Open();
    aDoor[02]->Open();
    aDoor[03]->Open();
    aDoor[04]->Open();
    aDoor[05]->Open();
  }

  //Ziel 2
  if(iIndex == 1)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 0, 205, -1)->Set(2);
    CreateObject(BRDR, 4100, 0, -1)->Set(1,1);
    CreateObject(BRDR, 3190, 0, -1)->Set(1,1,0,1,2);
    CreateObject(BRDR, 3400, 0, -1)->Set([0,0,1760,600],1);

    //Teamgrenze
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),1820,0,1,1);

    //Türen öffnen
    aDoor[06]->Open();
    aDoor[07]->Open();
    aDoor[08]->Open();
    aDoor[09]->Open();
    aDoor[10]->Open();
    aDoor[11]->Open();

    //Helikopter und Hinweisschilder
    if(!FindObject(NOBH))
    {
      Schedule("SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,1375,1200,-1),100*21)", 25*35);
      Schedule("SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,3690,1050,-1),100*21)", 25*35);

      CreateObject(SGNP, 1440, 1220, -1)->Set("Helicopter",1,0,1);
      CreateObject(SGNP, 3580, 1090, -1)->Set("Helicopter",1,0,1);
    }
  }

  //Ziel 3
  if(iIndex == 2)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 0, 205, -1)->Set(2);
    CreateObject(BRDR, 1300, 0, -1)->Set(0,1);
    CreateObject(BRDR, 0, 0, -1)->Set([0,0,1800,800],1);
    CreateObject(BRDR, 3400, 0, -1)->Set([0,0,1760,600],1);

    //Teamgrenze
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3310,0,1,1);

    //Türen öffnen
    aDoor[12]->Open();
    aDoor[13]->Open();
    aDoor[14]->Open();
    aDoor[15]->Open();
    aDoor[16]->Open();
    aDoor[17]->Open();
    aDoor[18]->Open();
    aDoor[19]->Open();
    aDoor[20]->Open();
    aDoor[21]->Open();

    //Türverbindungen sperren
    aDoorway[0]->SealEntrance();
    aDoorway[1]->SealEntrance();
    aDoorway[2]->SealEntrance();
    aDoorway[3]->SealEntrance();

    //Spawnpoints entfernen
    RemoveAll(VSPW);
  }

  //Ziel 4
  if(iIndex == 3)
  {
    //Grenzen neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 0, 205, -1)->Set(2);
    CreateObject(BRDR, 2750, 0, -1)->Set(0,1);

    //Teamgrenze
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3640,0,[0,0,1520,590],1);

    //Türverbindungen sperren
    aDoorway[4]->SealEntrance();
    aDoorway[5]->SealEntrance();
    aDoorway[6]->SealEntrance();
    aDoorway[7]->SealEntrance();

    //Türverbindungen öffnen
    aDoorway[16]->Unlock();
    aDoorway[17]->Unlock();
    aDoorway[18]->Unlock();
    aDoorway[19]->Unlock();
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX =4730; iY = 280;
    return 1;
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    if(iTeam == 1)
      return [[1520, 1210], [1500, 1250], [1525, 1320], [1570, 1440]];
    if(iTeam == 2)
      return [[3910, 1120], [3860, 1240], [3710, 1360], [3880, 1325]];
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
      aFlag[0]->MoveFlagpost(2700,770,"$Flag6$",150);
      aFlag[0]->SetClrModulation(RGB(150,150,150));
      aFlagPosition = 2;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2700,1160,"$Flag7$",150);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 2)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(2700,540,"$Flag3$",150);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2700,1160,"$Flag7$",150);
      aFlagPosition = 3;
    }
  }
  else
  if(aFlagPosition == 3)
  {
    if(!Random(2))
    {
      aFlag[0]->MoveFlagpost(2700,540,"$Flag6$",150);
      aFlagPosition = 1;
    }
    else
    {
      aFlag[0]->MoveFlagpost(2700,770,"$Flag3$",150);
      aFlag[0]->SetClrModulation(RGB(150,150,150));
      aFlagPosition = 2;
    }
  }

  EventInfo4K(0,Format("$MsgFlagChanged$", GetName(aFlag[0])),SM21, 0, 0, 0, "Info_Objective.ogg");

  goto(0);
}