genhl:
	haxe -cp generator  -lib hl-idl --macro "chipmunk.Generator.generateCpp()"
	
genjs:
	haxe -cp generator -lib hl-idl --macro "chipmunk.Generator.generateJs()"
