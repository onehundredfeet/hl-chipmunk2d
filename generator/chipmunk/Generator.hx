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

extern \"C\" {
#include<chipmunk/chipmunk.h>
#include<chipmunk/cpHastySpace.h> // done
#include<chipmunk/cpPolyline.h> //done
#include<chipmunk/cpRobust.h> //done
}

/*
#include<chipmunk/cpBody.h> // -done

// #include<chipmunk/chipmunk_ffi.h>  // TODO ?
// #include<chipmunk/chipmunk_private.h> // done
#include<chipmunk/chipmunk_structs.h> // -done
#include<chipmunk/chipmunk_types.h> // -done
// #include<chipmunk/chipmunk_unsafe.h> // TODO ?
//#include<chipmunk/cpArbiter.h> // -done
#include<chipmunk/cpBB.h> // -done 
#include<chipmunk/cpConstraint.h> // -done
// #include<chipmunk/cpDampedRotarySpring.h> // -done
// #include<chipmunk/cpDampedSpring.h>  // -done
// #include<chipmunk/cpGearJoint.h> // -done
// #include<chipmunk/cpGrooveJoint.h> // -done
#include<chipmunk/cpMarch.h> // TODO. Function pointer arguments
// #include<chipmunk/cpPinJoint.h> // done
// #include<chipmunk/cpPivotJoint.h> //done
// #include<chipmunk/cpPolyShape.h> //done
// #include<chipmunk/cpRatchetJoint.h> //done
#include<chipmunk/cpRobust.h> //done
// #include<chipmunk/cpRotaryLimitJoint.h> //done
#include<chipmunk/cpShape.h> //done
// #include<chipmunk/cpSimpleMotor.h> // -done
// #include<chipmunk/cpSlideJoint.h>  // -done
// #include<chipmunk/cpSpace.h>  // -done
// #include<chipmunk/cpSpatialIndex.h>  // -done
// #include<chipmunk/cpTransform.h> // -done
#include<chipmunk/cpVect.h> // -done

struct Chipmunk2D {};

// I had to replace the original functions because each call was duplicating the space.
// Transform from  cpShape* cpSpaceAddShape(space, shape); -- to -->  void cpSpaceAddShape(space, shape);
void cpSpaceAddShapeVoid(cpSpace* space, cpShape *shape) {
	cpSpaceAddShape(space, shape);
}

// I had to replace the original functions because each call was duplicating the space.
// Transform from  cpBody* cpSpaceAddBody(space, body); -- to -->  void cpSpaceAddBody(space, body);
void cpSpaceAddBodyVoid(cpSpace* space, cpBody *body) {
	cpSpaceAddBody(space, body);
}
*/
cpShape *cpSpaceSegmentQueryFirstNoFilter(cpSpace *space, cpVect start, cpVect end, cpFloat radius, cpSegmentQueryInfo *out)
{
	cpShapeFilter filter = cpShapeFilterNew(0, 0, 0);
	return cpSpaceSegmentQueryFirst(space, start, end, radius, filter, out);
};
template <typename T>
inline void cpConstraintFreeT( T*ptr ) {
	cpConstraintFree((cpConstraint *)ptr);
}
template <typename T>
inline void cpShapeFreeT( T*ptr ) {
	cpShapeFree((cpShape*)ptr);
}

template <typename T>
inline void cpSpatialIndexFreeT( T*ptr ) {
	cpSpatialIndexFree((cpSpatialIndex*)ptr);
}
";
	
	public static function generateCpp() {	
		var options = { idlFile : "src/chipmunk.idl", nativeLib : "chipmunk2d", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end