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
#include<chipmunk/chipmunk_structs.h>
#include<chipmunk/chipmunk_types.h>
// #include<chipmunk/chipmunk_unsafe.h>
//#include<chipmunk/cpArbiter.h> // -done
#include<chipmunk/cpBB.h> // -done 
//#include<chipmunk/cpBody.h> // -done
#include<chipmunk/cpConstraint.h> // -done
// #include<chipmunk/cpDampedRotarySpring.h> // -done
// #include<chipmunk/cpDampedSpring.h>  // -done
// #include<chipmunk/cpGearJoint.h> // -done
// #include<chipmunk/cpGrooveJoint.h> // -done
#include<chipmunk/cpHastySpace.h> // done
#include<chipmunk/cpMarch.h> // TODO. Function pointer arguments
// #include<chipmunk/cpPinJoint.h> // done
// #include<chipmunk/cpPivotJoint.h> //done
#include<chipmunk/cpPolyline.h> //done
// #include<chipmunk/cpPolyShape.h>
// #include<chipmunk/cpRatchetJoint.h>
// #include<chipmunk/cpRobust.h>
// #include<chipmunk/cpRotaryLimitJoint.h>
#include<chipmunk/cpShape.h>
// #include<chipmunk/cpSimpleMotor.h> // -done
// #include<chipmunk/cpSlideJoint.h>
// #include<chipmunk/cpSpace.h>
// #include<chipmunk/cpSpatialIndex.h>
// #include<chipmunk/cpTransform.h>
#include<chipmunk/cpVect.h>

struct Chipmunk2D {};

// README 
// I had to replace the original functions because each call was duplicating the space.
// Transform from  cpShape* cpSpaceAddShape(space, shape); -- to -->  void cpSpaceAddShape(space, shape);
void cpSpaceAddShapeVoid(cpSpace* space, cpShape *shape) {
	cpSpaceAddShape(space, shape);
}

// README 
// I had to replace the original functions because each call was duplicating the space.
// Transform from  cpBody* cpSpaceAddBody(space, body); -- to -->  void cpSpaceAddBody(space, body);
void cpSpaceAddBodyVoid(cpSpace* space, cpBody *body) {
	cpSpaceAddBody(space, body);
}

";
	
	public static function generateCpp() {	
		var options = { idlFile : "src/chipmunk.idl", nativeLib : "chipmunk", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end