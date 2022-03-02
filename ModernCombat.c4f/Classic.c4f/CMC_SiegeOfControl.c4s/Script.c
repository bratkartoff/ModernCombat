/*-- Siege of Control --*/

#strict 2
#include CSTD

func RecommendedGoals()	{return [GMNR,GLMS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,20,15);
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  SetNextMission("ModernCombat.c4f\\Classic.c4f\\CMC_SiegeOfControl.c4s", "$RepeatButton$", "$RepeatButtonDesc$");
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Metal2",370,580,420,630,410,640,360,590,true);
  DrawMaterialQuad("Wall-Metal2",370,670,420,720,410,730,360,680,true);
  DrawMaterialQuad("Wall-Metal2",530,900,440,810,430,820,520,910,true);
  DrawMaterialQuad("BackWall-Laboratory1",520,670,560,670,580,690,580,730,true);
  DrawMaterialQuad("BackWall-Laboratory1",550,770,580,740,600,760,580,780,true);
  DrawMaterialQuad("BackWall-Grid1",640,791,700,770,740,770,830,791,true);
  DrawMaterialQuad("Wall-Plate",620,820,580,840,570,840,530,820,true);
  DrawMaterialQuad("Wall-Plate",685,670,709,660,709,670,685,670,true);
  DrawMaterialQuad("Wall-Concrete3",730,570,740,575,740,580,730,580,true);
  DrawMaterialQuad("Wall-Metal2",1020,900,1040,890,1060,900,1040,910,true);
  DrawMaterialQuad("Wall-Plate",1020,750,1040,740,1100,740,1140,760,true);
  DrawMaterialQuad("Wall-Plate",1040,690,1100,650,1140,670,1070,710,true);

  DrawMaterialQuad("BackWall-Grid1",1050,771,1080,781,1080,810,1050,800,true);
  DrawMaterialQuad("BackWall-Grid1",1360,781,1390,771,1390,800,1360,810,true);

  DrawMaterialQuad("Wall-Plate",1400,690,1340,650,1300,670,1370,710,true);
  DrawMaterialQuad("Wall-Plate",1420,750,1400,740,1340,740,1300,760,true);
  DrawMaterialQuad("Wall-Metal2",1420,900,1400,890,1380,900,1400,910,true);
  DrawMaterialQuad("BackWall-Grid1",1610,791,1700,770,1740,770,1800,791,true);
  DrawMaterialQuad("Wall-Concrete3",1710,570,1700,575,1700,580,1710,580,true);
  DrawMaterialQuad("Wall-Plate",1755,670,1731,660,1731,670,1755,670,true);
  DrawMaterialQuad("Wall-Plate",1820,820,1860,840,1870,840,1910,820,true);
  DrawMaterialQuad("BackWall-Laboratory1",1920,670,1880,670,1860,690,1860,730,true);
  DrawMaterialQuad("BackWall-Laboratory1",1890,770,1860,740,1840,760,1860,780,true);
  DrawMaterialQuad("Wall-Metal2",1910,900,2000,810,2010,820,1920,910,true);
  DrawMaterialQuad("Wall-Metal2",2070,580,2020,630,2030,640,2080,590,true);
  DrawMaterialQuad("Wall-Metal2",2070,670,2020,720,2030,730,2080,680,true);

  //Schutztüren
  CreateObject(GDDR, 505, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 624, 900, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 720, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 846, 900, -1)->SetColorDw(HSL(145, 210, 100, 127));

  CreateObject(GDDR, 1055, 740, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1385, 740, -1)->SetColorDw(HSL(145, 210, 100, 127));

  CreateObject(GDDR, 1594, 900, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1720, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1816, 900, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 1935, 670, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Gitter
  CreateObject(GTNG, 584, 810, -1);
  CreateObject(GTNG, 846, 810, -1);
  CreateObject(GTNG, 1024, 800, -1);
  CreateObject(GTNG, 1416, 800, -1);
  CreateObject(GTNG, 1594, 810, -1);
  CreateObject(GTNG, 1856, 810, -1);

  //Stahlbrücken
  CreateObject(_HBR, 565, 912, -1)->SwitchMode();
  CreateObject(_HBR, 575, 582, -1)->SwitchMode();
  CreateObject(_HBR, 675, 582, -1)->SwitchMode();
  CreateObject(_HBR, 795, 552, -1)->SwitchMode();
  CreateObject(_HBR, 895, 552, -1)->SwitchMode();

  CreateObject(_HBR, 885, 822, -1)->SwitchMode();
  CreateObject(_HBR, 965, 912, -1)->SwitchMode();
  CreateObject(_HBR, 1005, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1105, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1115, 822, -1)->SwitchMode();
  CreateObject(_HBR, 1115, 902, -1)->SwitchMode();
  CreateObject(_HBR, 1325, 822, -1)->SwitchMode();
  CreateObject(_HBR, 1325, 902, -1)->SwitchMode();
  CreateObject(_HBR, 1335, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1435, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1475, 912, -1)->SwitchMode();
  CreateObject(_HBR, 1555, 822, -1)->SwitchMode();

  CreateObject(_HBR, 1545, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1645, 552, -1)->SwitchMode();
  CreateObject(_HBR, 1765, 582, -1)->SwitchMode();
  CreateObject(_HBR, 1865, 582, -1)->SwitchMode();
  CreateObject(_HBR, 1875, 912, -1)->SwitchMode();

  //Sandsackbarrieren
  CreateObject(SBBA, 515, 570, -1)->Set(0,1);
  CreateObject(SBBA, 1160, 890, -1)->Set(0,1);
  CreateObject(SBBA, 1195, 760, -1)->Set(0,1);
  CreateObject(SBBA, 1245, 760, -1)->Set(1,1);
  CreateObject(SBBA, 1280, 890, -1)->Set(1,1);
  CreateObject(SBBA, 1925, 570, -1)->Set(1,1);

  //Rahmen
  CreateObject(FRME, 551, 663, -1);
  CreateObject(FRME, 661, 663, -1);

  CreateObject(FRME, 791, 633, -1);
  CreateObject(FRME, 791, 753, -1);
  CreateObject(FRME, 891, 633, -1);
  CreateObject(FRME, 891, 753, -1);
  CreateObject(FRME, 1011, 633, -1);
  CreateObject(FRME, 1111, 633, -1);

  CreateObject(FRME, 1331, 633, -1);
  CreateObject(FRME, 1431, 633, -1);
  CreateObject(FRME, 1551, 633, -1);
  CreateObject(FRME, 1551, 753, -1);
  CreateObject(FRME, 1651, 633, -1);
  CreateObject(FRME, 1651, 753, -1);

  CreateObject(FRME, 1781, 663, -1);
  CreateObject(FRME, 1891, 663, -1);

  //Glasscheiben
  CreateObject(PANE, 502, 620, -1);
  CreateObject(PANE, 712, 610, -1);
  CreateObject(PANE, 728, 610, -1);
  CreateObject(PANE, 622, 850, -1);
  CreateObject(PANE, 638, 850, -1);
  CreateObject(PAN2, 655, 816, -1);
  CreateObject(PAN2, 685, 816, -1);
  CreateObject(PAN2, 785, 816, -1);
  CreateObject(PAN2, 815, 816, -1);
  CreateObject(PANE, 832, 850, -1);
  CreateObject(PANE, 848, 850, -1);

  CreateObject(PANE, 1592, 850, -1);
  CreateObject(PANE, 1608, 850, -1);
  CreateObject(PAN2, 1625, 816, -1);
  CreateObject(PAN2, 1655, 816, -1);
  CreateObject(PAN2, 1755, 816, -1);
  CreateObject(PAN2, 1785, 816, -1);
  CreateObject(PANE, 1712, 610, -1);
  CreateObject(PANE, 1728, 610, -1);
  CreateObject(PANE, 1802, 850, -1);
  CreateObject(PANE, 1818, 850, -1);
  CreateObject(PANE, 1938, 620, -1);

  //Metallkisten
  CreateObject(MWCR, 935, 810, -1)->AutoRespawn();
  CreateObject(MWCR, 1505, 810, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 660, 670, -1)->AutoRespawn();
  CreateObject(BECR, 740, 900, -1)->AutoRespawn();
  CreateObject(BECR, 950, 540, -1)->AutoRespawn();
  CreateObject(BECR, 1490, 540, -1)->AutoRespawn();
  CreateObject(BECR, 1700, 900, -1)->AutoRespawn();
  CreateObject(BECR, 1780, 670, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 860, 810, -1)->AutoRespawn();
  CreateObject(GSBL, 1580, 810, -1)->AutoRespawn();

  //Leitern
  CreateObject(LADR, 440, 790, -1)->Set(14);
  CreateObject(LADR, 720, 808, -1)->Set(5);
  CreateObject(LADR, 759, 640, -1)->Set(8);
  CreateObject(LADR, 770, 724, -1)->Set(7);

  CreateObject(LADR, 960, 866, -1)->Set(6);
  CreateObject(LADR, 1170, 872, -1)->Set(13);
  CreateObject(LADR, 1220, 705, -1)->Set(6);
  CreateObject(LADR, 1270, 872, -1)->Set(13);
  CreateObject(LADR, 1480, 866, -1)->Set(6);

  CreateObject(LADR, 1670, 724, -1)->Set(7);
  CreateObject(LADR, 1681, 640, -1)->Set(8);
  CreateObject(LADR, 1720, 808, -1)->Set(5);
  CreateObject(LADR, 2000, 790, -1)->Set(14);

  //Bodenluken
  CreateObject(HA4K,440,673,-1);
  CreateObject(HA4K,720,763,-1);
  CreateObject(HA4K,960,813,-1);
  CreateObject(HA4K,1170,763,-1);
  CreateObject(HA4K,1170,813,-1);
  CreateObject(HA4K,1220,653,-1);
  CreateObject(HA4K,1270,763,-1);
  CreateObject(HA4K,1270,813,-1);
  CreateObject(HA4K,1480,813,-1);
  CreateObject(HA4K,1720,763,-1);
  CreateObject(HA4K,2000,673,-1);

  //Explosivkisten
  CreateObject(XWCR,845,540,-1)->AutoRespawn();
  CreateObject(XWCR,920,900,-1)->AutoRespawn();
  CreateObject(XWCR,1520,900,-1)->AutoRespawn();
  CreateObject(XWCR,1595,540,-1)->AutoRespawn();

  //Explosivfass
  CreateObject(XBRL,1170,710,-1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL,745,540,-1)->AutoRespawn();
  CreateObject(PBRL,1270,710,-1)->AutoRespawn();
  CreateObject(PBRL,1695,540,-1)->AutoRespawn();

  //Grenze
  CreateObject(BRDR, 0, 1250, -1)->Set(3,0,1);

  //Soundkulisse

  //Rush Hour
  CreateObject(SE4K, 620, 1250, -1)->Set("Traffic*.ogg",500,100, 25,50);
  CreateObject(SE4K, 1820, 1250, -1)->Set("Traffic*.ogg",500,100, 25,50);

  //Wind
  CreateObject(SE4K, 510, 270, -1)->Set("WindSound*.ogg",1000,500);
  CreateObject(SE4K, 1930, 270, -1)->Set("WindSound*.ogg",1000,500);

  //Innenbereich
  CreateObject(SE4K, 530, 720, -1)->Set("Interior*.ogg",700,100);
  CreateObject(SE4K, 1220, 1000, -1)->Set("InteriorMetal*.ogg",1000,300, 75);
  CreateObject(SE4K, 1220, 1100, -1)->Set("InteriorStress*.ogg",1000,300, 75);
  CreateObject(SE4K, 1910, 720, -1)->Set("Interior*.ogg",700,100);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Raketen
  PlaceSpawnpoint(MBOX, 1055, 535);
  PlaceSpawnpoint(MBOX, 1385, 535);

  //Versorgungskiste (Kugeln)
  CreateObject(AMCT, 1170, 650, -1)->Set(ABOX);

  //Versorgungskiste (Gewehrgranaten)
  CreateObject(AMCT, 1270, 650, -1)->Set(GBOX);

  //Versorgungskisten (Dragnin)
  CreateObject(AMCT, 605, 670, -1)->Set(DGNN);
  CreateObject(AMCT, 1835, 670, -1)->Set(DGNN);

  //Kaufstation
  var store = CreateObject(WPVM,1220, 890,-1);
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
  CreateObject(GNET, 1115, 650, -1)->Set(LCAC,-90);
  CreateObject(GNET, 1325, 650, -1)->Set(LCAC,90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radare
  CreateObject(RADR, 1150, 540, -1);
  CreateObject(RADR, 1285, 540, -1);

  //COM-Stationen
  CreateObject(CMSN, 630, 670, -1);
  CreateObject(CMSN, 1140, 650, -1);
  CreateObject(CMSN, 1170, 710, -1);
  CreateObject(CMSN, 1270, 710, -1);
  CreateObject(CMSN, 1300, 650, -1);
  CreateObject(CMSN, 1810, 670, -1);

  //Sonnenschirme
  CreateObject(SNSE, 370, 580, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 410, 810, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 470, 810, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 625, 570, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 1815, 570, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 1970, 810, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 2030, 810, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(SNSE, 2070, 580, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Waschbecken
  CreateObject(WSHB, 655, 900, -1);
  CreateObject(WSHB, 685, 900, -1);
  CreateObject(WSHB, 715, 900, -1);

  //Toilettenkabinen
  CreateObject(TOIC, 770, 900, -1);
  CreateObject(TOIC, 790, 900, -1);
  CreateObject(TOIC, 810, 900, -1);

  //Topfpflanzen
  CreateObject(PLT3, 370, 670, -1);
  CreateObject(PLT3, 370, 810, -1);
  CreateObject(PLT3, 430, 580, -1);
  CreateObject(PLT3, 480, 670, -1);
  CreateObject(PLT4, 630, 850, -1);
  CreateObject(PLT4, 720, 610, -1);
  CreateObject(PLT4, 840, 850, -1);
  CreateObject(PLT3, 1010, 900, -1);

  CreateObject(PLT3, 1430, 900, -1);
  CreateObject(PLT4, 1600, 850, -1);
  CreateObject(PLT4, 1720, 610, -1);
  CreateObject(PLT4, 1810, 850, -1);
  CreateObject(PLT3, 1960, 670, -1);
  CreateObject(PLT3, 2010, 580, -1);
  CreateObject(PLT3, 2070, 670, -1);
  CreateObject(PLT3, 2070, 810, -1);

  //Betten
  CreateObject(BED2, 1650, 900, -1);
  CreateObject(BED2, 1765, 900, -1);

  //Automaten
  CreateObject(VGMN, 630, 760, -1)->Set(3);
  CreateObject(VGMN, 1810, 760, -1);

  //Geländer
  CreateObject(RAI1, 338, 580, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 338, 670, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 338, 810, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 418, 580, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 600, 900, -1);
  CreateObject(RAI1, 855, 900, -1)->SetRail([1,3,1,3]);
  CreateObject(RAI1, 970, 810, -1)->SetRail([1,3,1,3]);

  CreateObject(RAI1, 1423, 810, -1)->SetRail([3,1,3,1]);
  CreateObject(RAI1, 1535, 900, -1)->SetRail([3,1,3,1]);
  CreateObject(RAI1, 1820, 900, -1);
  CreateObject(RAI1, 1918, 810, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1938, 670, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1958, 580, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);

  //Deckenlichter
  CreateObject(CLGH, 625, 585, -1);
  CreateObject(CLGH, 720, 825, -1);
  CreateObject(CLGH, 1720, 825, -1);
  CreateObject(CLGH, 1815, 585, -1);

  //Feuerlöscher
  CreateObject(FIE2, 740, 655, -1);
  CreateObject(FIE2, 740, 755, -1);
  CreateObject(FIE2, 1700, 655, -1);
  CreateObject(FIE2, 1700, 755, -1);

  //Bildschirme
  CreateObject(SCA2, 710, 740, -1)->SetClips([8,9,10,11]);
  CreateObject(SCA1,670,860,-1);
  CreateObject(SCA2, 1115, 880, -1)->SetClips([12]);
  var screen = CreateObject(SCR3, 1170, 740, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCA1,1220,640,-1);
  screen = CreateObject(SCR3, 1270, 740, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCA2, 1325, 880, -1)->SetClips([13]);
  CreateObject(SCA1,1770,860,-1);
  CreateObject(SCA2, 1730, 740, -1)->SetClips([8,9,10,11]);

  //Orientierungslichter
  CreateObject(OLGH, 1220, 540, -1)->Set(9, 50, 1, 1, 20);

  //Lüftungsgitter
  CreateObject(ENGT, 790, 870, -1);
  CreateObject(ENGT, 950, 620, -1)->SetCon(60);
  CreateObject(ENGT, 950, 740, -1)->SetCon(60);
  CreateObject(ENGT, 950, 860, -1)->SetCon(60);

  CreateObject(ENGT, 1490, 620, -1)->SetCon(60);
  CreateObject(ENGT, 1490, 740, -1)->SetCon(60);
  CreateObject(ENGT, 1490, 860, -1)->SetCon(60);
  CreateObject(ENGT, 1650, 870, -1);

  //Straßenlichter
  CreateObject(SLGH, 720, 470, -1);
  CreateObject(SLGH, 1720, 470, -1);

  //Regale
  CreateObject(FRAM, 600, 710, -1);
  CreateObject(FRAM, 620, 710, -1);
  CreateObject(FRAM, 640, 710, -1);
  CreateObject(FRAM, 1800, 710, -1);
  CreateObject(FRAM, 1820, 710, -1);
  CreateObject(FRAM, 1840, 710, -1);

  //Notausgangslichter
  CreateObject(ETLT, 555, 785, -1);
  CreateObject(ETLT, 1885, 785, -1);

  //Labortische
  CreateObject(LTBL, 675, 760, -1);
  CreateObject(LTBL, 770, 760, -1);
  CreateObject(LTBL, 1765, 760, -1);
  CreateObject(LTBL, 1670, 760, -1);

  //Monitore
  CreateObject(MNI2, 685, 748, -1);
  CreateObject(MNI2, 760, 748, -1);
  CreateObject(MNI2, 1680, 748, -1);
  CreateObject(MNI2, 1755, 748, -1);

  //Papierstapel
  CreateObject(PPSK, 610, 760, -1);
  CreateObject(PPSK, 670, 747, -1);
  CreateObject(PPSK, 780, 747, -1);
  CreateObject(PPSK, 800, 760, -1);
  CreateObject(PPSK, 810, 760, -1);
  CreateObject(PPSK, 1150, 760, -1);
  CreateObject(PPSK, 1290, 760, -1);
  CreateObject(PPSK, 1630, 760, -1);
  CreateObject(PPSK, 1640, 760, -1);
  CreateObject(PPSK, 1665, 747, -1);
  CreateObject(PPSK, 1770, 747, -1);
  CreateObject(PPSK, 1830, 760, -1);

  //Ventillatoren
  CreateObject(VENT, 750, 810, -1)->SetCon(30);
  CreateObject(VENT, 1060, 800, -1)->SetCon(20);
  CreateObject(VENT, 1380, 800, -1)->SetCon(20);
  CreateObject(VENT, 1690, 810, -1)->SetCon(30);

  //Hinweisschilder
  CreateObject(WLSN, 605, 630, -1)->Set("Health","Red");
  CreateObject(WLSN, 1070, 860, -1)->Set("Toilet");
  CreateObject(WLSN, 1070, 880, -1)->Set("Left");
  CreateObject(WLSN, 1370, 860, -1)->Set("Bed");
  CreateObject(WLSN, 1370, 880, -1)->Set("Right");
  CreateObject(WLSN, 1835, 630, -1)->Set("Health","Red");
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD11,600,1130,-1);
  CreateObject(BD11,1800,1130,-1);
  CreateObject(BD11,3000,1130,-1);
  CreateObject(BD03,1200,950,-1);
  CreateObject(BD03,1400,1200,-1);
  CreateObject(BD10,100,1400,-1);
  CreateObject(BD10,1490,1400,-1);
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Infiltration.ogg");

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 1220,760, -1);
    flag->~Set("$Flag1$");

    //Versorgungskiste (APW)
    CreateObject(AMCT, 1220, 810, -1)->Set(ATWN);
  }
  else
  {
    //Versorgungskiste (APW)
    CreateObject(AMCT, 1220, 760, -1)->Set(ATWN);
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
    //Geldsäcke
    AddMoneySpawn(745, 655, [10]);
    AddMoneySpawn(880, 895, [10]);
    AddMoneySpawn(1220, 645, [15]);
    AddMoneySpawn(1220, 805, [10]);
    AddMoneySpawn(1560, 895, [10]);
    AddMoneySpawn(1695, 655, [10]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 1220; iY = 570;
    return 1;
  }

  //HTF/MR/LMS-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS))
  {
    return [[390, 590], [390, 680], [410, 800], [480, 570], [610, 800], [710, 560], [490, 850], [1950, 850], [1730, 560], [1830, 800], [1960, 570], [2030, 800], [2050, 590], [2050, 680]];
    return 1;
  }
}