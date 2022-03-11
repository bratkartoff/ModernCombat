name := "CMC_Operation_Metro_Frontlines.c4s"
start:
	#cd /opt/LegacyClonk; gdb -x gdbcommands ./clonk 
	cd /opt/LegacyClonk; clonk  "ModernCombat.c4f/Classic.c4f/CMC_OperationMetro.c4s"

copy: pack
	cp ~/tst/{{name}} /opt/LegacyClonk


push: pack
	scp ~/tst/{{name}} $wyip:/opt/LegacyClonk

pack:
	cp -r {{name}}/ ~/tst
	cp -r ModernCombat.c4d/Goals.c4d/Occupation.c4d ~/tst/{{name}}
	cp -r ModernCombat.c4d/Goals.c4d/Frontlines.c4d ~/tst/{{name}}
	c4group ~/tst/{{name}} -p

