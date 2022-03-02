/*-- Breakthrough --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aDoorWay;

func RecommendedGoals()			{return [GOCC];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1,40,15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Türverbindungen
  aDoorWay = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Neustart-Button setzen
  if(C4X_VerBuild < 331) {SetNextMission("ModernCombat.c4f\\Special.c4f\\CMC_Breakthrough.c4s", "$RepeatButton$", "$RepeatButtonDesc$");}
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Grenzen
  CreateObject(BRDR, 620, 0, -1)->Set(0);
  CreateObject(BRDR, 3560, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 15, -1)->Set(2);

  //Hinweisschilder
  CreateObject(SGNP, 840, 1060, -1);
  CreateObject(SGNP, 3350, 1060, -1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",660,190,800,130,800,140,680,200,true);
  DrawMaterialQuad("Wall-Concrete2",680,320,810,340,680,340,680,320,true);
  DrawMaterialQuad("Wall-Stripes",960,260,970,250,990,250,990,260,true);
  DrawMaterialQuad("Wall-Concrete2",1240,1000,1300,1020,1240,1020,1240,1000,true);
  DrawMaterialQuad("Wall-Metal2",1350,940,1380,970,1350,970,1350,940,true);
  DrawMaterialQuad("Earth-SMOOTH",1500,1020,1580,1100,1500,1100,1500,1020,true);
  DrawMaterialQuad("Wall-Concrete3",1540,1010,1560,1000,1560,1030,1540,1010,true);
  DrawMaterialQuad("Wall-Concrete3",1610,1000,1645,1020,1610,1020,1610,1000,true);
  DrawMaterialQuad("Wall-Concrete3",2570,1000,2535,1020,2570,1020,2570,1000,true);
  DrawMaterialQuad("Wall-Concrete3",2640,1010,2620,1030,2620,1000,2640,1010,true);
  DrawMaterialQuad("Earth-SMOOTH",2600,1100,2680,1020,2680,1100,2600,1100,true);
  DrawMaterialQuad("Wall-Metal2",2830,940,2830,970,2800,970,2830,940,true);
  DrawMaterialQuad("Wall-Concrete2",2940,1000,2870,1020,2940,1020,2940,1000,true);
  DrawMaterialQuad("Wall-Stripes",3220,260,3210,250,3190,250,3190,260,true);
  DrawMaterialQuad("Wall-Concrete2",3500,320,3370,340,3500,340,3500,320,true);
  DrawMaterialQuad("Wall-Concrete2",3380,130,3520,190,3500,200,3380,140,true);

  //Leitern
  CreateObject(LADR, 690, 316, -1)->Set(6);
  CreateObject(LADR, 850, 316, -1)->Set(6);
  CreateObject(LADR, 910, 336, -1)->Set(16);
  CreateObject(LADR, 1000, 432, -1)->Set(28);
  CreateObject(LADR, 1150, 988, -1)->Set(5);
  CreateObject(LADR, 1300, 1004, -1)->Set(7);
  CreateObject(LADR, 1505, 1008, -1)->Set(4);
  CreateObject(LADR, 1705, 1100, -1)->Set(10);

  CreateObject(LADR, 2475, 1100, -1)->Set(10);
  CreateObject(LADR, 2675, 1008, -1)->Set(4);
  CreateObject(LADR, 2880, 1004, -1)->Set(7);
  CreateObject(LADR, 3030, 988, -1)->Set(5);
  CreateObject(LADR, 3180, 432, -1)->Set(28);
  CreateObject(LADR, 3270, 336, -1)->Set(16);
  CreateObject(LADR, 3330, 316, -1)->Set(6);
  CreateObject(LADR, 3490, 316, -1)->Set(6);

  //Bodenluken
  CreateObject(HA4K, 690, 263, -1);
  CreateObject(HA4K, 850, 263, -1);
  CreateObject(HA4K, 910, 203, -1);
  CreateObject(HA4K, 910, 263, -1);
  CreateObject(HA4K, 1000, 203, -1);
  CreateObject(HA4K, 1000, 253, -1);
  CreateObject(HA4K, 1150, 943, -1);
  CreateObject(HA4K, 1300, 943, -1);

  CreateObject(HA4K, 2880, 943, -1);
  CreateObject(HA4K, 3030, 943, -1);
  CreateObject(HA4K, 3180, 203, -1);
  CreateObject(HA4K, 3180, 253, -1);
  CreateObject(HA4K, 3270, 203, -1);
  CreateObject(HA4K, 3270, 263, -1);
  CreateObject(HA4K, 3330, 263, -1);
  CreateObject(HA4K, 3490, 263, -1);

  //Schutztüren
  CreateObject(GDDR,670, 320,-1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR,894, 260,-1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR,894, 340,-1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR, 1264, 940, -1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR, 1487, 1020, -1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR, 2694, 1020, -1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR, 2917, 940, -1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR,3287, 260,-1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR,3287, 340,-1)->SetColorDw(RGB(128,128,128));
  CreateObject(GDDR,3510, 320,-1)->SetColorDw(RGB(128,128,128));

  //Schutt
  CreateObject(DEB1, 1785, 1140, -1)->AddSmokeEffect4K(40,-2,-5,true,this);
  var derb = CreateObject(DEB1, 2395, 1140, -1);
  derb->AddSmokeEffect4K(40,-2,-5,true,this);
  derb->SetDir(1);

  //Rauch
  CreateObject(TIM1, 1975, 1050, -1)->AddSmokeEffect4K(80,0,0,true,this);
  CreateObject(TIM1, 2025, 1115, -1)->AddSmokeEffect4K(60,0,0,true,this);
  CreateObject(TIM1, 2205, 1050, -1)->AddSmokeEffect4K(120,0,0,true,this);

  //Verbandskisten
  CreateObject(BECR, 1040, 200, -1)->AutoRespawn();
  CreateObject(BECR, 3140, 200, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 950, 200, -1)->AutoRespawn();
  CreateObject(GSBL, 3230, 200, -1)->AutoRespawn();

  //Gerüste
  CreateObject(SFFG, 1090, 250, -1)->Set(3);
  CreateObject(SFFG, 1391, 1020, -1)->Set(4);
  CreateObject(SFFG, 2790, 1020, -1)->Set(4);
  CreateObject(SFFG, 3090, 250, -1)->Set(3);

  //Stahlbrücken
  var bridge = CreateObject(_HBR, 845, 352, -1);
  bridge->SwitchMode(1);
  bridge = CreateObject(_HBR, 1525, 1064, -1);
  bridge->SetR(45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 2655, 1065, -1);
  bridge->SetR(-45);
  bridge->SetIrreparable();
  bridge = CreateObject(_HBR, 3335, 352, -1);
  bridge->SwitchMode(1);

  //Verbundene Räume
  aDoorWay[00] = CreateObject(ROM2, 640, 320, -1);
  aDoorWay[01] = CreateObject(ROM2, 900, 890, -1);
  aDoorWay[00]->Connect(aDoorWay[01]);
  aDoorWay[02] = CreateObject(ROM2, 940, 260, -1);
  aDoorWay[03] = CreateObject(ROM2, 1340, 1020, -1);
  aDoorWay[02]->Connect(aDoorWay[03]);
  aDoorWay[04] = CreateObject(ROM2, 3240, 260, -1);
  aDoorWay[05] = CreateObject(ROM2, 2840, 1020, -1);
  aDoorWay[04]->Connect(aDoorWay[05]);
  aDoorWay[06] = CreateObject(ROM2, 3540, 320, -1);
  aDoorWay[07] = CreateObject(ROM2, 3280, 890, -1);
  aDoorWay[06]->Connect(aDoorWay[07]);

  //Sonne umplatzieren
  FindObject(LENS)->SetPosition(2130,840);

  //Sendemast
  var tower = CreateObject(AATR, 2090, 1030, -1);
  tower->AddNode(1555, 307, 0, CreateObject(REHR, 1567, 310, -1),90,2);
  tower->AddNode(2625, 302, 0, CreateObject(REHR, 2614, 312, -1),-90,2);
  tower->AddNode(1325, 567, 1, CreateObject(REHR, 1322, 574, -1),45,1);
  tower->AddNode(2858, 562, 1, CreateObject(REHR, 2858, 574, -1),-45,1);
  tower->AddNode(1670, 995, 2, CreateObject(REHR, 1660, 1020, -1));
  tower->AddNode(2515, 995, 2, CreateObject(REHR, 2520, 1020, -1));
  tower->SwitchMode();

  //Soundkulisse

  //Erdrutsche
  CreateObject(SE4K, 280, 800, -1)->Set("FallingDirt*.ogg",800,200, 75);
  CreateObject(SE4K, 3900, 800, -1)->Set("FallingDirt*.ogg",800,200, 75);

  //Wind
  CreateObject(SE4K, 1560, 150, -1)->Set("WindSound*.ogg",3000,100, 75);
  CreateObject(SE4K, 2620, 150, -1)->Set("WindSound*.ogg",3000,100, 75);

  //Innenbereich
  CreateObject(SE4K, 1300, 900, -1)->Set("Interior*.ogg",700,100, 75);
  CreateObject(SE4K, 900, 700, -1)->Set("InteriorStress*.ogg",700,100, 50,75);
  CreateObject(SE4K, 3280, 700, -1)->Set("InteriorStress*.ogg",700,100, 50,75);
  CreateObject(SE4K, 2880, 900, -1)->Set("Interior*.ogg",700,100, 75);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  CreateObject(AMCT, 1240, 940, -1)->Set(ABOX);
  CreateObject(AMCT, 2940, 940, -1)->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  CreateObject(AMCT, 1220, 940, -1)->Set(GBOX);
  CreateObject(AMCT, 2960, 940, -1)->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 980, 195);
  PlaceSpawnpoint(MBOX, 3200, 195);

  //Kaufstationen
  var store = CreateObject(WPVM,1585, 1000,-1);
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
  store = CreateObject(WPVM,2595, 1000,-1);
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

  //Artilleriebatterien
  CreateObject(ATBY,1300,890,-1)->SetRotation(40);
  CreateObject(ATBY,2880,890,-1)->SetRotation(-40);

  //Versorgungskiste (Railgun)
  CreateObject(AMCT, 2090, 1320, -1)->Set(RLGN,0,0,0,1);

  //Versorgungskisten (APW)
  CreateObject(AMCT, 795, 260, -1)->Set(ATWN);
  CreateObject(AMCT, 3385, 260, -1)->Set(ATWN);

  //Versorgungskisten (Dragnin)
  CreateObject(AMCT, 1150, 1000, -1)->Set(DGNN);
  CreateObject(AMCT, 3030, 1000, -1)->Set(DGNN);

  //MAV-Stationen
  CreateObject(MVSN, 1365, 1020, -1)->Set(1420,940,1);
  CreateObject(MVSN, 2815, 1020, -1)->Set(2760,940,1);

  //Geschützstellungen
  CreateObject(GNET, 1145, 250, -1)->Set(SATW,90);
  CreateObject(GNET, 1340, 940, -1)->Set(CNNW,90);
  CreateObject(GNET, 1490, 970, -1)->Set(CNNW,90);
  CreateObject(GNET, 1675, 1020, -1)->Set(SATW,90);

  CreateObject(GNET, 2505, 1020, -1)->Set(SATW,-90);
  CreateObject(GNET, 2690, 970, -1)->Set(CNNW,-90);
  CreateObject(GNET, 2840, 940, -1)->Set(CNNW,-90);
  CreateObject(GNET, 3035, 250, -1)->Set(SATW,-90);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Topfpflanzen
  CreateObject(PLT3, 720, 260, -1);
  CreateObject(PLT3, 820, 260, -1);
  CreateObject(PLT3, 3360, 260, -1);
  CreateObject(PLT3, 3460, 260, -1);

  //Betten
  CreateObject(BED2, 750, 260, -1);
  CreateObject(BED2, 850, 260, -1);
  CreateObject(BED2, 3330, 260, -1);
  CreateObject(BED2, 3430, 260, -1);

  //Automaten
  CreateObject(VGMN, 1595, 1100, -1)->Set(3);
  CreateObject(VGMN, 2587, 1100, -1);

  //Geländer
  CreateObject(RAI1, 658, 260, -1)->SetRail([1,3,1,3,1]);
  CreateObject(RAI1, 1000, 440, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  var rail = CreateObject(RAI1, 1700, 1030, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 15; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  rail = CreateObject(RAI1, 2340, 1030, -1);
  rail->SetCategory(C4D_StaticBack);
  rail->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  for(var i = 0; i <= 15; i++)
    rail->SetClrModulation(RGB(200, 200, 200), this, i);
  CreateObject(RAI1, 3015, 440, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 3458, 260, -1)->SetRail([1,3,1,3,1]);

  //Wandlampen
  CreateObject(BLGH, 670, 227, -1);
  CreateObject(BLGH, 1625, 1057, -1);
  CreateObject(BLGH, 2555, 1057, -1);
  CreateObject(BLGH, 3510, 227, -1);

  //Deckenlichter
  CreateObject(CLGH, 950, 215, -1);
  CreateObject(CLGH, 1340, 975, -1);
  CreateObject(CLGH, 2840, 975, -1);
  CreateObject(CLGH, 3230, 215, -1);

  //Bildschirme
  CreateObject(SCA2, 790, 220, -1);
  CreateObject(SCA2, 3390, 220, -1);

  //Jetwrack
  CreateObject(_JW1, 295, 1125, -1)->SetR(-10);
  CreateObject(_JW2, 370, 1065, -1)->SetR(-20);

  //Kaputte Boote
  CreateObject(SLBB, 600, 1110, -1)->SetR(10);
  CreateObject(SLBB, 3550, 1110, -1)->SetR(-10);

  //Straßenlichter
  CreateObject(SLGH, 1095, 940, -1);
  CreateObject(SLGH, 1425, 940, -1);
  CreateObject(SLGH, 2755, 940, -1);
  CreateObject(SLGH, 3085, 940, -1);

  //Büsche
  CreateObject(BSH2, 1440,980, -1);
  CreateObject(BSH2, 1460,980, -1);
  CreateObject(BSH2, 1950, 1270, -1);
  CreateObject(BSH2, 1980, 1275, -1);
  CreateObject(BSH2, 1990, 1300, -1);

  CreateObject(BSH2, 2190, 1300, -1);
  CreateObject(BSH2, 2200, 1275, -1);
  CreateObject(BSH2, 2230, 1270, -1);
  CreateObject(BSH2, 2720,980, -1);
  CreateObject(BSH2, 2740,980, -1);

  //Steine
  CreateObject(STNE, 425, 1040, -1);
  CreateObject(STNE, 745, 1090, -1);
  CreateObject(STNE, 3575, 1110, -1);
  CreateObject(STNE, 3755, 1040, -1);

  //Labortische
  CreateObject(LTBL, 1220, 1000, -1);
  CreateObject(LTBL, 2960, 1000, -1);

  //Monitore
  CreateObject(MNI2, 1210, 988, -1);
  CreateObject(MNI2, 1230, 988, -1);
  CreateObject(MNI2, 2950, 988, -1);
  CreateObject(MNI2, 2970, 988, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 1625, 830, -1);
  CreateObject(ENGT, 1625, 930, -1);
  CreateObject(ENGT, 2555, 830, -1);
  CreateObject(ENGT, 2555, 930, -1);

  //Orientierungslichter
  CreateObject(OLGH, 975, 890, -1)->Set(3, 18, 1, 1, 30);
  CreateObject(OLGH, 1150, 940, -1)->Set(3, 20, 1, 1, 30);
  CreateObject(OLGH, 1085, 440, -1)->Set(4, 23, 1, 1, 30);
  CreateObject(OLGH, 1870, 852, -1)->Set(4, 7, 1, 1, 30);
  CreateObject(OLGH, 2310, 852, -1)->Set(4, 7, 1, 1, 30);
  CreateObject(OLGH, 3095, 440, -1)->Set(3, 30, 1, 1, 30);
  CreateObject(OLGH, 3030, 940, -1)->Set(3, 20, 1, 1, 30);
  CreateObject(OLGH, 3205, 890, -1)->Set(3, 18, 1, 1, 30);

  //Papierstapel
  CreateObject(PPSK, 1190, 1000, -1);
  CreateObject(PPSK, 2990, 1000, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD11,600,1050,-1);
  CreateObject(BD07,1300,1170,-1);
  CreateObject(BD03,1500,500,-1);
  CreateObject(BD11,1800,1050,-1);
  CreateObject(BD11,3000,1050,-1);
  CreateObject(BD11,4200,1050,-1);

  //Bei Sturm verdunkeln
  if(FindObject(STRM))
  {
    var back = FindObjects(Find_Func("IsDeco"));
    for(var pObj in back)
      pObj->SetClrModulation(RGBa(50,50,50,5));
  }
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(FindObject(GOCC))
  {
    if(pPoint == aFlag[0])
    {
      if(aSelfDefense[0])
        aSelfDefense[0]->SetTeam(iTeam);
      if(aSelfDefense[1])
        aSelfDefense[1]->SetTeam(iTeam);
    }
    if(pPoint == aFlag[1])
    {
      if(iTeam == 1)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(750,320);
        aFlag[1] -> AddSpawnPoint(825,250);
        aFlag[1] -> AddSpawnPoint(890,190);
      }
      if(iTeam == 2)
      {
        aFlag[1] -> ResetSpawnPoint();
        aFlag[1] -> AddSpawnPoint(3290,190);
        aFlag[1] -> AddSpawnPoint(3355,250);
        aFlag[1] -> AddSpawnPoint(3430,320);
      }
    }
    if(pPoint == aFlag[2])
    {
      if(aSelfDefense[2])
        aSelfDefense[2]->SetTeam(iTeam);
      if(aSelfDefense[3])
        aSelfDefense[3]->SetTeam(iTeam);
    }
  }
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iClass)
{
  AddEffect("SpawnParachute", pClonk, 1, 10);
  return _inherited(pClonk, iClass, ...);
}

/* Fallschirmeffekt */

global func FxSpawnParachuteTimer(object pTarget)
{
  //Ziel im Freien?
  if(!Contained(pTarget))
  {
    //Ziel ist festem Boden zu nahe: Abbruch
    var x = GetX(pTarget), y = GetY(pTarget), xdir = GetXDir(pTarget, 100), ydir = GetYDir(pTarget, 100);
    SimFlight(x, y, xdir, ydir, 0, 0, 0, 100);
    if(Distance(xdir, ydir) < 700)
      return -1;

    //Ansonsten Fallschirm erstellen
    CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
    AddEffect("Flying", pTarget, 101, 5);

    //Effekt
    Sound("Airstrike2", 0, pTarget);

    return -1;
  }
  //Ansonsten abwarten
}

/* Sturmerstellung */

func FormStorm()
{
  //Sturm erstellen
  CreateObject(STRM);

  //Dunkelheit erhöhen
  if(GetDarkness() < 3)
    FadeDarkness(3,60);

  //Hintergründe verdunkeln
  var back = FindObjects(Find_Func("IsDeco"));
  for(var pObj in back)
    pObj->SetClrModulation(RGBa(50,50,50,5));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Back in the Earth.ogg;CMC_Breaching.ogg;CMC_Deep Universe.ogg;CMC_Drone in Flight.ogg;CMC_Enemy Approaching.ogg;CMC_Eurocorps.ogg;CMC_Firehawk.ogg;CMC_Getaway.ogg;CMC_Infiltration.ogg;CMC_Locked and Loaded.ogg;CMC_Matrix.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Titanium City.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Enemy Approaching.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
    aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,1090,410,-1),70*21);
    SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,975,870,-1),70*21);
    SetupVehicleSpawn([LBRD],DIR_Right,CreateObject(VSPW,1145,920,-1),70*21);

    SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,3030,920,-1),70*21);
    SetupVehicleSpawn([LBRD],DIR_Left,CreateObject(VSPW,3205,870,-1),70*21);
    SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,3090,410,-1),70*21);

    //Hinweisschilder
    var sign = CreateObject(SNPT, 1080, 860, -1);
    sign->SetMode(1);
    sign->SetAction("Sign3");
    sign = CreateObject(SNPT, 3100, 860, -1);
    sign->SetMode(1);
    sign->SetAction("Sign3");
  }

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1375, 960, -1);
  aSelfDefense[0]->Set(0,1,1,225,0,1790);
  aSelfDefense[0]->SetTeam(1);

  aSelfDefense[1] = CreateObject(SEGU, 930, 375, -1);
  aSelfDefense[1]->Set(0,1,1,-90,0,1670);
  aSelfDefense[1]->SetTeam(1);

  aSelfDefense[2] = CreateObject(SEGU, 3250, 375, -1);
  aSelfDefense[2]->Set(0,1,1,90,2510,0);
  aSelfDefense[2]->SetTeam(2);

  aSelfDefense[3] = CreateObject(SEGU, 2810, 960, -1);
  aSelfDefense[3]->Set(0,1,1,135,2400,0);
  aSelfDefense[3]->SetTeam(2);

  //Zeitverzögertes Gewitter
  Schedule("GameCall(\"FormStorm\")", RandomX(12000,17000));

  //OCC-Spielziel
  if(FindObject(GOCC))
  {
    //Flaggenposten
    aFlag[0] = CreateObject(OFPL,1625,750,NO_OWNER);
    aFlag[0] -> AddSpawnPoint(1060,930);
    aFlag[0] -> AddSpawnPoint(1110,990);
    aFlag[0] -> AddSpawnPoint(1270,1000);
    aFlag[0] -> AddSpawnPoint(1430,1010);
    if(aTeams[1] == true)
    {
      aFlag[0]->Set("$Flag1$",160,12);
      aFlag[0]->Capture(1,1);
    }
    else
    {
      aFlag[0]->Set("$Flag1$",160,12);
    }

    aFlag[1] = CreateObject(OFPL,2090,1030,NO_OWNER);
    aFlag[1]->Set("$Flag2$",160);

    aFlag[2] = CreateObject(OFPL,2555,750,NO_OWNER);
    aFlag[2] -> AddSpawnPoint(2750,1010);
    aFlag[2] -> AddSpawnPoint(2910,1000);
    aFlag[2] -> AddSpawnPoint(3070,990);
    aFlag[2] -> AddSpawnPoint(3120,930);
    if(aTeams[2] == true)
    {
      aFlag[2]->Set("$Flag3$",160,12);
      aFlag[2]->Capture(2,1);
    }
    else
    {
      aFlag[2]->Set("$Flag3$",160,12);
    }

    //Grenze
    CreateObject(BRDR, 0, 1130, -1)->Set(3,1);

    //Hinweisschilder
    CreateObject(SGNP, 1830, 1210, -1);
    CreateObject(SGNP, 2350, 1210, -1);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    //Flaggenposten
    var flag = CreateObject(OFPL, 2090, 1160, -1);
    flag->~Set("$Flag2$",180);

    //Teamgrenzen
    CreateObject(BRDR, 1670, 0, -1)->Set(0,1,0,1,1);
    CreateObject(BRDR, 2510, 0, -1)->Set(1,1,0,1,2);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    //Flaggen
    if(aTeams[1] == true)
    {CreateFlag(1,1390,970,GetTeamColor(1));}
    if(aTeams[2] == true)
    {CreateFlag(2,2790,970,GetTeamColor(2));}

    //Objekte entfernen
    RemoveObject(aSelfDefense[0]);
    RemoveObject(aSelfDefense[3]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
    iX = 2090; iY = 850;
    return 1;
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
    if(iTeam == 1)
      return [[1060,930], [1110,990], [1270,1000]];
    if(iTeam == 2)
      return [[2910,1000], [3070,990], [3120,930]];
    return 1;
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
    if(iTeam == 1)
      return [[750,320], [825,250], [890,190]];
    if(iTeam == 2)
      return [[3290,190], [3355,250], [3430,320]];
    return 1;
  }
}