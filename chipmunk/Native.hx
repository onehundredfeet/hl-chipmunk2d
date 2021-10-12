package chipmunk;

typedef Native = haxe.macro.MacroType<[webidl.Module.build({ idlFile : "generator/chipmunk.idl", chopPrefix : "cp", autoGC : true, nativeLib : "chipmunk" })]>;
