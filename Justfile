name := "ExperimentalDoNotSave.c4d"
copy: pack
	cp {{name}} /opt/LegacyClonk

start: copy
	clonk "ModernCombat.c4f/Classic.c4f/CMC_OperationMetro.c4s" "/lobby:1"

push: pack
	scp {{name}} $wyip:/opt/LegacyClonk

pack:
	cp -r ModernCombat.c4d/Goals.c4d/Occupation.c4d .
	c4group Occupation.c4d -p
	mv Occupation.c4d {{name}}

