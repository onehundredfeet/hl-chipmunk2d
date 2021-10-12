package chipmunk;

#if eval
class Generator {

	// Put any necessary includes in this string and they will be added to the generated files
	static var INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif

#include<chipmunk/chipmunk.h>
// #include<chipmunk/chipmunk_ffi.h>
// #include<chipmunk/chipmunk_private.h>
// #include<chipmunk/chipmunk_structs.h>
#include<chipmunk/chipmunk_types.h>
// #include<chipmunk/chipmunk_unsafe.h>
// #include<chipmunk/cpArbiter.h>
#include<chipmunk/cpBB.h>
// #include<chipmunk/cpBody.h>
// #include<chipmunk/cpConstraint.h>
// #include<chipmunk/cpDampedRotarySpring.h>
// #include<chipmunk/cpDampedSpring.h>
// #include<chipmunk/cpGearJoint.h>
// #include<chipmunk/cpGrooveJoint.h>
// #include<chipmunk/cpHastySpace.h>
// #include<chipmunk/cpMarch.h>
// #include<chipmunk/cpPinJoint.h>
// #include<chipmunk/cpPivotJoint.h>
// #include<chipmunk/cpPolyline.h>
// #include<chipmunk/cpPolyShape.h>
// #include<chipmunk/cpRatchetJoint.h>
// #include<chipmunk/cpRobust.h>
// #include<chipmunk/cpRotaryLimitJoint.h>
// #include<chipmunk/cpShape.h>
// #include<chipmunk/cpSimpleMotor.h>
// #include<chipmunk/cpSlideJoint.h>
// #include<chipmunk/cpSpace.h>
// #include<chipmunk/cpSpatialIndex.h>
// #include<chipmunk/cpTransform.h>
#include<chipmunk/cpVect.h>

struct Chipmunk2D {};

struct Apple{
	int x;
	int y;
};

Apple* getApples(){
	Apple* apples = new Apple[2];
	apples[0].x = 0;
	apples[0].y = 1;
	apples[1].x = 2;
	apples[1].y = 3;
	return apples;
}

void alterApples(Apple* apples){
	apples[0].x = 10;
	apples[0].y = 11;
	apples[1].x = 12;
	apples[1].y = 13;
}
";
	
	public static function generateCpp() {	
		var options = { idlFile : "generator/chipmunk.idl", nativeLib : "chipmunk", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end