package chipmunk;

typedef Native = haxe.macro.MacroType<[webidl.Module.build({ idlFile : "chipmunk.idl", chopPrefix : "", autoGC : true, nativeLib : "chipmunk" })]>;
