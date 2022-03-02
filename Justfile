name := "ExperimentalDoNotSave.c4d"
start:
	clonk "ModernCombat.c4f/Classic.c4f/CMC_OperationMetro.c4s"

copy: pack
	cp {{name}} /opt/LegacyClonk


push: pack
	scp {{name}} $wyip:/opt/LegacyClonk

pack:
	cp -r ModernCombat.c4d/Goals.c4d/Occupation.c4d .
	c4group Occupation.c4d -p
	mv Occupation.c4d {{name}}

