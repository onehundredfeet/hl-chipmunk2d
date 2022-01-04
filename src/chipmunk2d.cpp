#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) chipmunk2d_##x
#include <hl.h>

// Need to link in helpers
HL_API hl_type hltx_ui16;
HL_API hl_type hltx_ui8;

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

// Float vector
struct _hl_float2 {
	float x;
	float y;
};

struct _hl_float3 {
	float x;
	float y;
	float z;
};

struct _hl_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _hl_int2 {
	int x;
	int y;
};

struct _hl_int3 {
	int x;
	int y;
	int z;
};

struct _hl_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _hl_double2 {
	double x;
	double y;
};

struct _hl_double3 {
	double x;
	double y;
	double z;
};

struct _hl_double4 {
	double x;
	double y;
	double z;
	double w;
};

inline void testvector(_hl_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif

extern "C" {
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


class FloatArray
{
public:
	FloatArray() {}

	FloatArray(int size)
	{
		list = new float[size];
	}

	float Get(int index)
	{
		return list[index];
	}

	void Set(int index, float value)
	{
		list[index] = value;
	}

	float* GetPtr() {
		return list;
	}

private:
	float* list;
};

class IntArray
{
public:
	IntArray() {}

	IntArray(int size)
	{
		list = new int[size];
	}

	int Get(int index)
	{
		return list[index];
	}

	void Set(int index, int value)
	{
		list[index] = value;
	}

	int* GetPtr() {
		return list;
	}

private:
	int* list;
};

class CharArray
{
public:
	CharArray() {}

	CharArray(int size)
	{
		list = new unsigned char[size];
	}

	char Get(int index)
	{
		return list[index];
	}

	void Set(int index, unsigned char value)
	{
		list[index] = value;
	}

	unsigned char* GetPtr() {
		return list;
	}

private:
	unsigned char* list;
};

class ShortArray
{
public:
    ShortArray() {}

    ShortArray(int size)
	{
		list = new unsigned short[size];
	}

	short Get(int index)
	{
		return list[index];
	}

	void Set(int index, unsigned short value)
	{
		list[index] = value;
	}

	unsigned short* GetPtr() {
		return list;
	}

private:
	unsigned short* list;
};


extern "C" {

static void finalize_Vect( _ref(cpVect)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Vect_delete)( _ref(cpVect)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Vect_delete, _IDL);
static void finalize_cpTransform( _ref(cpTransform)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpTransform_delete)( _ref(cpTransform)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpTransform_delete, _IDL);
static void finalize_cpMat2x2( _ref(cpMat2x2)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpMat2x2_delete)( _ref(cpMat2x2)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpMat2x2_delete, _IDL);
static void finalize_cpBB( _ref(cpBB)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpBB_delete)( _ref(cpBB)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpBB_delete, _IDL);
static void finalize_Body( _ref(cpBody)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Body_delete)( _ref(cpBody)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Body_delete, _IDL);
static void finalize_cpPointQueryInfo( _ref(cpPointQueryInfo)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPointQueryInfo_delete)( _ref(cpPointQueryInfo)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPointQueryInfo_delete, _IDL);
static void finalize_cpSegmentQueryInfo( _ref(cpSegmentQueryInfo)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpSegmentQueryInfo_delete)( _ref(cpSegmentQueryInfo)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpSegmentQueryInfo_delete, _IDL);
static void finalize_cpShapeFilter( _ref(cpShapeFilter)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpShapeFilter_delete)( _ref(cpShapeFilter)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpShapeFilter_delete, _IDL);
static void finalize_Shape( _ref(cpShape)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Shape_delete)( _ref(cpShape)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Shape_delete, _IDL);
static void finalize_CircleShape( _ref(cpCircleShape)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(CircleShape_delete)( _ref(cpCircleShape)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, CircleShape_delete, _IDL);
static void finalize_SegmentShape( _ref(cpSegmentShape)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(SegmentShape_delete)( _ref(cpSegmentShape)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, SegmentShape_delete, _IDL);
static void finalize_Space( _ref(cpSpace)* _this ) { free_ref(_this ,cpSpaceFree); }
HL_PRIM void HL_NAME(Space_delete)( _ref(cpSpace)* _this ) {
	free_ref(_this ,cpSpaceFree);
}
DEFINE_PRIM(_VOID, Space_delete, _IDL);
static void finalize_cpContactPointSet( _ref(cpContactPointSet)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpContactPointSet_delete)( _ref(cpContactPointSet)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpContactPointSet_delete, _IDL);
static void finalize_cpArbiter( _ref(cpArbiter)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpArbiter_delete)( _ref(cpArbiter)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpArbiter_delete, _IDL);
static void finalize_Constraint( _ref(cpConstraint)* _this ) { free_ref(_this ,cpConstraintFree); }
HL_PRIM void HL_NAME(Constraint_delete)( _ref(cpConstraint)* _this ) {
	free_ref(_this ,cpConstraintFree);
}
DEFINE_PRIM(_VOID, Constraint_delete, _IDL);
static void finalize_cpDampedRotarySpring( _ref(cpDampedRotarySpring)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpDampedRotarySpring_delete)( _ref(cpDampedRotarySpring)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpDampedRotarySpring_delete, _IDL);
static void finalize_cpDampedSpring( _ref(cpDampedSpring)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpDampedSpring_delete)( _ref(cpDampedSpring)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpDampedSpring_delete, _IDL);
static void finalize_cpGearJoint( _ref(cpGearJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpGearJoint_delete)( _ref(cpGearJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpGearJoint_delete, _IDL);
static void finalize_cpSimpleMotor( _ref(cpSimpleMotor)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpSimpleMotor_delete)( _ref(cpSimpleMotor)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpSimpleMotor_delete, _IDL);
static void finalize_cpGrooveJoint( _ref(cpGrooveJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpGrooveJoint_delete)( _ref(cpGrooveJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpGrooveJoint_delete, _IDL);
static void finalize_cpHastySpace( _ref(cpHastySpace)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpHastySpace_delete)( _ref(cpHastySpace)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpHastySpace_delete, _IDL);
static void finalize_cpPinJoint( _ref(cpPinJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPinJoint_delete)( _ref(cpPinJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPinJoint_delete, _IDL);
static void finalize_cpPivotJoint( _ref(cpPivotJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPivotJoint_delete)( _ref(cpPivotJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPivotJoint_delete, _IDL);
static void finalize_cpPolyline( _ref(cpPolyline)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPolyline_delete)( _ref(cpPolyline)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPolyline_delete, _IDL);
static void finalize_cpPolylineSet( _ref(cpPolylineSet)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPolylineSet_delete)( _ref(cpPolylineSet)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPolylineSet_delete, _IDL);
static void finalize_cpPolyShape( _ref(cpPolyShape)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpPolyShape_delete)( _ref(cpPolyShape)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpPolyShape_delete, _IDL);
static void finalize_cpRatchetJoint( _ref(cpRatchetJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpRatchetJoint_delete)( _ref(cpRatchetJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpRatchetJoint_delete, _IDL);
static void finalize_cpRotaryLimitJoint( _ref(cpRotaryLimitJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpRotaryLimitJoint_delete)( _ref(cpRotaryLimitJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpRotaryLimitJoint_delete, _IDL);
static void finalize_SlideJoint( _ref(cpSlideJoint)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(SlideJoint_delete)( _ref(cpSlideJoint)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, SlideJoint_delete, _IDL);
static void finalize_cpSpaceHash( _ref(cpSpaceHash)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpSpaceHash_delete)( _ref(cpSpaceHash)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpSpaceHash_delete, _IDL);
static void finalize_cpBBTree( _ref(cpBBTree)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(cpBBTree_delete)( _ref(cpBBTree)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, cpBBTree_delete, _IDL);
static void finalize_SpatialIndex( _ref(cpSpatialIndex)* _this ) { free_ref(_this ,cpSpatialIndexFree); }
HL_PRIM void HL_NAME(SpatialIndex_delete)( _ref(cpSpatialIndex)* _this ) {
	free_ref(_this ,cpSpatialIndexFree);
}
DEFINE_PRIM(_VOID, SpatialIndex_delete, _IDL);
HL_PRIM double HL_NAME(Vect_get_x)( _ref(cpVect)* _this ) {
	return _unref(_this)->x;
}
DEFINE_PRIM(_F64,Vect_get_x,_IDL);
HL_PRIM double HL_NAME(Vect_set_x)( _ref(cpVect)* _this, double value ) {
	_unref(_this)->x = (value);
	return value;
}
DEFINE_PRIM(_F64,Vect_set_x,_IDL _F64);

HL_PRIM double HL_NAME(Vect_get_y)( _ref(cpVect)* _this ) {
	return _unref(_this)->y;
}
DEFINE_PRIM(_F64,Vect_get_y,_IDL);
HL_PRIM double HL_NAME(Vect_set_y)( _ref(cpVect)* _this, double value ) {
	_unref(_this)->y = (value);
	return value;
}
DEFINE_PRIM(_F64,Vect_set_y,_IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_new0)() {
	return alloc_ref((new cpVect()),Vect);
}
DEFINE_PRIM(_IDL, Vect_new0,);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpv2)(double x, double y) {
	return alloc_ref(new cpVect((cpv(x, y))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpv2, _F64 _F64);

HL_PRIM unsigned char HL_NAME(Vect_cpveql2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return (cpveql(*_unref(v1), *_unref(v2)));
}
DEFINE_PRIM(_I8, Vect_cpveql2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvadd2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return alloc_ref(new cpVect((cpvadd(*_unref(v1), *_unref(v2)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvadd2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvsub2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return alloc_ref(new cpVect((cpvsub(*_unref(v1), *_unref(v2)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvsub2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvneg1)(_ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpvneg(*_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvneg1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvmult2)(_ref(cpVect)* v, double s) {
	return alloc_ref(new cpVect((cpvmult(*_unref(v), s))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvmult2, _IDL _F64);

HL_PRIM double HL_NAME(Vect_cpvdot2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return (cpvdot(*_unref(v1), *_unref(v2)));
}
DEFINE_PRIM(_F64, Vect_cpvdot2, _IDL _IDL);

HL_PRIM double HL_NAME(Vect_cpvcross2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return (cpvcross(*_unref(v1), *_unref(v2)));
}
DEFINE_PRIM(_F64, Vect_cpvcross2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvperp1)(_ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpvperp(*_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvperp1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvrperp1)(_ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpvrperp(*_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvrperp1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvproject2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return alloc_ref(new cpVect((cpvproject(*_unref(v1), *_unref(v2)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvproject2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvforangle1)(double a) {
	return alloc_ref(new cpVect((cpvforangle(a))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvforangle1, _F64);

HL_PRIM double HL_NAME(Vect_cpvtoangle1)(_ref(cpVect)* v) {
	return (cpvtoangle(*_unref(v)));
}
DEFINE_PRIM(_F64, Vect_cpvtoangle1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvrotate2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return alloc_ref(new cpVect((cpvrotate(*_unref(v1), *_unref(v2)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvrotate2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvunrotate2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return alloc_ref(new cpVect((cpvunrotate(*_unref(v1), *_unref(v2)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvunrotate2, _IDL _IDL);

HL_PRIM double HL_NAME(Vect_cpvlengthsq1)(_ref(cpVect)* v) {
	return (cpvlengthsq(*_unref(v)));
}
DEFINE_PRIM(_F64, Vect_cpvlengthsq1, _IDL);

HL_PRIM double HL_NAME(Vect_cpvlength1)(_ref(cpVect)* v) {
	return (cpvlength(*_unref(v)));
}
DEFINE_PRIM(_F64, Vect_cpvlength1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvlerp3)(_ref(cpVect)* v1, _ref(cpVect)* v2, double t) {
	return alloc_ref(new cpVect((cpvlerp(*_unref(v1), *_unref(v2), t))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvlerp3, _IDL _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvnormalize1)(_ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpvnormalize(*_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvnormalize1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvslerp3)(_ref(cpVect)* v1, _ref(cpVect)* v2, double t) {
	return alloc_ref(new cpVect((cpvslerp(*_unref(v1), *_unref(v2), t))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvslerp3, _IDL _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvslerpconst3)(_ref(cpVect)* v1, _ref(cpVect)* v2, double a) {
	return alloc_ref(new cpVect((cpvslerpconst(*_unref(v1), *_unref(v2), a))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvslerpconst3, _IDL _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvclamp2)(_ref(cpVect)* v, double len) {
	return alloc_ref(new cpVect((cpvclamp(*_unref(v), len))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvclamp2, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Vect_cpvlerpconst3)(_ref(cpVect)* v1, _ref(cpVect)* v2, double d) {
	return alloc_ref(new cpVect((cpvlerpconst(*_unref(v1), *_unref(v2), d))),Vect);
}
DEFINE_PRIM(_IDL, Vect_cpvlerpconst3, _IDL _IDL _F64);

HL_PRIM double HL_NAME(Vect_cpvdist2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return (cpvdist(*_unref(v1), *_unref(v2)));
}
DEFINE_PRIM(_F64, Vect_cpvdist2, _IDL _IDL);

HL_PRIM double HL_NAME(Vect_cpvdistsq2)(_ref(cpVect)* v1, _ref(cpVect)* v2) {
	return (cpvdistsq(*_unref(v1), *_unref(v2)));
}
DEFINE_PRIM(_F64, Vect_cpvdistsq2, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Vect_cpvnear3)(_ref(cpVect)* v1, _ref(cpVect)* v2, double dist) {
	return (cpvnear(*_unref(v1), *_unref(v2), dist));
}
DEFINE_PRIM(_I8, Vect_cpvnear3, _IDL _IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_a)( _ref(cpTransform)* _this ) {
	return _unref(_this)->a;
}
DEFINE_PRIM(_F64,cpTransform_get_a,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_a)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->a = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_a,_IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_b)( _ref(cpTransform)* _this ) {
	return _unref(_this)->b;
}
DEFINE_PRIM(_F64,cpTransform_get_b,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_b)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->b = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_b,_IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_c)( _ref(cpTransform)* _this ) {
	return _unref(_this)->c;
}
DEFINE_PRIM(_F64,cpTransform_get_c,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_c)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->c = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_c,_IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_d)( _ref(cpTransform)* _this ) {
	return _unref(_this)->d;
}
DEFINE_PRIM(_F64,cpTransform_get_d,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_d)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->d = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_d,_IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_tx)( _ref(cpTransform)* _this ) {
	return _unref(_this)->tx;
}
DEFINE_PRIM(_F64,cpTransform_get_tx,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_tx)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->tx = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_tx,_IDL _F64);

HL_PRIM double HL_NAME(cpTransform_get_ty)( _ref(cpTransform)* _this ) {
	return _unref(_this)->ty;
}
DEFINE_PRIM(_F64,cpTransform_get_ty,_IDL);
HL_PRIM double HL_NAME(cpTransform_set_ty)( _ref(cpTransform)* _this, double value ) {
	_unref(_this)->ty = (value);
	return value;
}
DEFINE_PRIM(_F64,cpTransform_set_ty,_IDL _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_new0)() {
	return alloc_ref((new cpTransform()),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_new0,);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformNew6)(double a, double b, double c, double d, double tx, double ty) {
	return alloc_ref(new cpTransform((cpTransformNew(a, b, c, d, tx, ty))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformNew6, _F64 _F64 _F64 _F64 _F64 _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformNewTranspose6)(double a, double c, double tx, double b, double d, double ty) {
	return alloc_ref(new cpTransform((cpTransformNewTranspose(a, c, tx, b, d, ty))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformNewTranspose6, _F64 _F64 _F64 _F64 _F64 _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformInverse1)(_ref(cpTransform)* t) {
	return alloc_ref(new cpTransform((cpTransformInverse(*_unref(t)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformInverse1, _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformMult2)(_ref(cpTransform)* t1, _ref(cpTransform)* t2) {
	return alloc_ref(new cpTransform((cpTransformMult(*_unref(t1), *_unref(t2)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformMult2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpTransform_cpTransformPoint2)(_ref(cpTransform)* t, _ref(cpVect)* p) {
	return alloc_ref(new cpVect((cpTransformPoint(*_unref(t), *_unref(p)))),Vect);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformPoint2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpTransform_cpTransformVect2)(_ref(cpTransform)* t, _ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpTransformVect(*_unref(t), *_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformVect2, _IDL _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(cpTransform_cpTransformbBB2)(_ref(cpTransform)* t, _ref(cpBB)* bb) {
	return alloc_ref(new cpBB((cpTransformbBB(*_unref(t), *_unref(bb)))),cpBB);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformbBB2, _IDL _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformTranslate1)(_ref(cpVect)* translate) {
	return alloc_ref(new cpTransform((cpTransformTranslate(*_unref(translate)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformTranslate1, _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformScale2)(double scaleX, double scaleY) {
	return alloc_ref(new cpTransform((cpTransformScale(scaleX, scaleY))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformScale2, _F64 _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformRotate1)(double radians) {
	return alloc_ref(new cpTransform((cpTransformRotate(radians))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformRotate1, _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformRigid2)(_ref(cpVect)* translate, double radians) {
	return alloc_ref(new cpTransform((cpTransformRigid(*_unref(translate), radians))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformRigid2, _IDL _F64);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformRigidInverse1)(_ref(cpTransform)* t) {
	return alloc_ref(new cpTransform((cpTransformRigidInverse(*_unref(t)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformRigidInverse1, _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformWrap2)(_ref(cpTransform)* outer, _ref(cpTransform)* inner) {
	return alloc_ref(new cpTransform((cpTransformWrap(*_unref(outer), *_unref(inner)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformWrap2, _IDL _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformWrapInverse2)(_ref(cpTransform)* outer, _ref(cpTransform)* inner) {
	return alloc_ref(new cpTransform((cpTransformWrapInverse(*_unref(outer), *_unref(inner)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformWrapInverse2, _IDL _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformOrtho1)(_ref(cpBB)* bb) {
	return alloc_ref(new cpTransform((cpTransformOrtho(*_unref(bb)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformOrtho1, _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformBoneScale2)(_ref(cpVect)* v0, _ref(cpVect)* v1) {
	return alloc_ref(new cpTransform((cpTransformBoneScale(*_unref(v0), *_unref(v1)))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformBoneScale2, _IDL _IDL);

HL_PRIM _ref(cpTransform)* HL_NAME(cpTransform_cpTransformAxialScale3)(_ref(cpVect)* axis, _ref(cpVect)* pivot, double scale) {
	return alloc_ref(new cpTransform((cpTransformAxialScale(*_unref(axis), *_unref(pivot), scale))),cpTransform);
}
DEFINE_PRIM(_IDL, cpTransform_cpTransformAxialScale3, _IDL _IDL _F64);

HL_PRIM _ref(cpMat2x2)* HL_NAME(cpMat2x2_new0)() {
	return alloc_ref((new cpMat2x2()),cpMat2x2);
}
DEFINE_PRIM(_IDL, cpMat2x2_new0,);

HL_PRIM double HL_NAME(cpMat2x2_get_a)( _ref(cpMat2x2)* _this ) {
	return _unref(_this)->a;
}
DEFINE_PRIM(_F64,cpMat2x2_get_a,_IDL);
HL_PRIM double HL_NAME(cpMat2x2_set_a)( _ref(cpMat2x2)* _this, double value ) {
	_unref(_this)->a = (value);
	return value;
}
DEFINE_PRIM(_F64,cpMat2x2_set_a,_IDL _F64);

HL_PRIM double HL_NAME(cpMat2x2_get_b)( _ref(cpMat2x2)* _this ) {
	return _unref(_this)->b;
}
DEFINE_PRIM(_F64,cpMat2x2_get_b,_IDL);
HL_PRIM double HL_NAME(cpMat2x2_set_b)( _ref(cpMat2x2)* _this, double value ) {
	_unref(_this)->b = (value);
	return value;
}
DEFINE_PRIM(_F64,cpMat2x2_set_b,_IDL _F64);

HL_PRIM double HL_NAME(cpMat2x2_get_c)( _ref(cpMat2x2)* _this ) {
	return _unref(_this)->c;
}
DEFINE_PRIM(_F64,cpMat2x2_get_c,_IDL);
HL_PRIM double HL_NAME(cpMat2x2_set_c)( _ref(cpMat2x2)* _this, double value ) {
	_unref(_this)->c = (value);
	return value;
}
DEFINE_PRIM(_F64,cpMat2x2_set_c,_IDL _F64);

HL_PRIM double HL_NAME(cpMat2x2_get_d)( _ref(cpMat2x2)* _this ) {
	return _unref(_this)->d;
}
DEFINE_PRIM(_F64,cpMat2x2_get_d,_IDL);
HL_PRIM double HL_NAME(cpMat2x2_set_d)( _ref(cpMat2x2)* _this, double value ) {
	_unref(_this)->d = (value);
	return value;
}
DEFINE_PRIM(_F64,cpMat2x2_set_d,_IDL _F64);

HL_PRIM double HL_NAME(cpBB_get_l)( _ref(cpBB)* _this ) {
	return _unref(_this)->l;
}
DEFINE_PRIM(_F64,cpBB_get_l,_IDL);
HL_PRIM double HL_NAME(cpBB_set_l)( _ref(cpBB)* _this, double value ) {
	_unref(_this)->l = (value);
	return value;
}
DEFINE_PRIM(_F64,cpBB_set_l,_IDL _F64);

HL_PRIM double HL_NAME(cpBB_get_b)( _ref(cpBB)* _this ) {
	return _unref(_this)->b;
}
DEFINE_PRIM(_F64,cpBB_get_b,_IDL);
HL_PRIM double HL_NAME(cpBB_set_b)( _ref(cpBB)* _this, double value ) {
	_unref(_this)->b = (value);
	return value;
}
DEFINE_PRIM(_F64,cpBB_set_b,_IDL _F64);

HL_PRIM double HL_NAME(cpBB_get_r)( _ref(cpBB)* _this ) {
	return _unref(_this)->r;
}
DEFINE_PRIM(_F64,cpBB_get_r,_IDL);
HL_PRIM double HL_NAME(cpBB_set_r)( _ref(cpBB)* _this, double value ) {
	_unref(_this)->r = (value);
	return value;
}
DEFINE_PRIM(_F64,cpBB_set_r,_IDL _F64);

HL_PRIM double HL_NAME(cpBB_get_t)( _ref(cpBB)* _this ) {
	return _unref(_this)->t;
}
DEFINE_PRIM(_F64,cpBB_get_t,_IDL);
HL_PRIM double HL_NAME(cpBB_set_t)( _ref(cpBB)* _this, double value ) {
	_unref(_this)->t = (value);
	return value;
}
DEFINE_PRIM(_F64,cpBB_set_t,_IDL _F64);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBNew4)(double l, double b, double r, double t) {
	return alloc_ref(new cpBB((cpBBNew(l, b, r, t))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBNew4, _F64 _F64 _F64 _F64);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBNewForExtents3)(_ref(cpVect)* c, double hw, double hh) {
	return alloc_ref(new cpBB((cpBBNewForExtents(*_unref(c), hw, hh))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBNewForExtents3, _IDL _F64 _F64);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBNewForCircle2)(_ref(cpVect)* p, double r) {
	return alloc_ref(new cpBB((cpBBNewForCircle(*_unref(p), r))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBNewForCircle2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpBB_cpBBIntersects2)(_ref(cpBB)* a, _ref(cpBB)* b) {
	return (cpBBIntersects(*_unref(a), *_unref(b)));
}
DEFINE_PRIM(_I8, cpBB_cpBBIntersects2, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpBB_cpBBContainsBB2)(_ref(cpBB)* bb, _ref(cpBB)* other) {
	return (cpBBContainsBB(*_unref(bb), *_unref(other)));
}
DEFINE_PRIM(_I8, cpBB_cpBBContainsBB2, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpBB_cpBBContainsVect2)(_ref(cpBB)* bb, _ref(cpVect)* v) {
	return (cpBBContainsVect(*_unref(bb), *_unref(v)));
}
DEFINE_PRIM(_I8, cpBB_cpBBContainsVect2, _IDL _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBMerge2)(_ref(cpBB)* a, _ref(cpBB)* b) {
	return alloc_ref(new cpBB((cpBBMerge(*_unref(a), *_unref(b)))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBMerge2, _IDL _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBExpand2)(_ref(cpBB)* bb, _ref(cpVect)* v) {
	return alloc_ref(new cpBB((cpBBExpand(*_unref(bb), *_unref(v)))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBExpand2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpBB_cpBBCenter1)(_ref(cpBB)* bb) {
	return alloc_ref(new cpVect((cpBBCenter(*_unref(bb)))),Vect);
}
DEFINE_PRIM(_IDL, cpBB_cpBBCenter1, _IDL);

HL_PRIM double HL_NAME(cpBB_cpBBArea1)(_ref(cpBB)* bb) {
	return (cpBBArea(*_unref(bb)));
}
DEFINE_PRIM(_F64, cpBB_cpBBArea1, _IDL);

HL_PRIM double HL_NAME(cpBB_cpBBMergedArea2)(_ref(cpBB)* a, _ref(cpBB)* b) {
	return (cpBBMergedArea(*_unref(a), *_unref(b)));
}
DEFINE_PRIM(_F64, cpBB_cpBBMergedArea2, _IDL _IDL);

HL_PRIM double HL_NAME(cpBB_cpBBSegmentQuery3)(_ref(cpBB)* bb, _ref(cpVect)* a, _ref(cpVect)* b) {
	return (cpBBSegmentQuery(*_unref(bb), *_unref(a), *_unref(b)));
}
DEFINE_PRIM(_F64, cpBB_cpBBSegmentQuery3, _IDL _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpBB_cpBBIntersectsSegment3)(_ref(cpBB)* bb, _ref(cpVect)* a, _ref(cpVect)* b) {
	return (cpBBIntersectsSegment(*_unref(bb), *_unref(a), *_unref(b)));
}
DEFINE_PRIM(_I8, cpBB_cpBBIntersectsSegment3, _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpBB_cpBBClampVect2)(_ref(cpBB)* bb, _ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpBBClampVect(*_unref(bb), *_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, cpBB_cpBBClampVect2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpBB_cpBBWrapVect2)(_ref(cpBB)* bb, _ref(cpVect)* v) {
	return alloc_ref(new cpVect((cpBBWrapVect(*_unref(bb), *_unref(v)))),Vect);
}
DEFINE_PRIM(_IDL, cpBB_cpBBWrapVect2, _IDL _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(cpBB_cpBBOffset2)(_ref(cpBB)* bb, _ref(cpVect)* v) {
	return alloc_ref(new cpBB((cpBBOffset(*_unref(bb), *_unref(v)))),cpBB);
}
DEFINE_PRIM(_IDL, cpBB_cpBBOffset2, _IDL _IDL);

HL_PRIM _ref(cpBody)* HL_NAME(Body_alloc0)() {
	return alloc_ref((cpBodyAlloc()),Body);
}
DEFINE_PRIM(_IDL, Body_alloc0,);

HL_PRIM _ref(cpBody)* HL_NAME(Body_init3)(_ref(cpBody)* body, double mass, double moment) {
	return alloc_ref((cpBodyInit(_unref(body), mass, moment)),Body);
}
DEFINE_PRIM(_IDL, Body_init3, _IDL _F64 _F64);

HL_PRIM _ref(cpBody)* HL_NAME(Body_makeNew2)(double mass, double moment) {
	return alloc_ref((cpBodyNew(mass, moment)),Body);
}
DEFINE_PRIM(_IDL, Body_makeNew2, _F64 _F64);

HL_PRIM _ref(cpBody)* HL_NAME(Body_newKinematic0)() {
	return alloc_ref((cpBodyNewKinematic()),Body);
}
DEFINE_PRIM(_IDL, Body_newKinematic0,);

HL_PRIM _ref(cpBody)* HL_NAME(Body_newStatic0)() {
	return alloc_ref((cpBodyNewStatic()),Body);
}
DEFINE_PRIM(_IDL, Body_newStatic0,);

HL_PRIM void HL_NAME(Body_destroy0)(_ref(cpBody)* _this) {
	(cpBodyDestroy( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Body_destroy0, _IDL);

HL_PRIM void HL_NAME(Body_free0)(_ref(cpBody)* _this) {
	(cpBodyFree( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Body_free0, _IDL);

HL_PRIM void HL_NAME(Body_activate0)(_ref(cpBody)* _this) {
	(cpBodyActivate( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Body_activate0, _IDL);

HL_PRIM void HL_NAME(Body_sleep0)(_ref(cpBody)* _this) {
	(cpBodySleep( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Body_sleep0, _IDL);

HL_PRIM void HL_NAME(Body_sleepWithGroup1)(_ref(cpBody)* _this, _ref(cpBody)* group) {
	(cpBodySleepWithGroup( _unref(_this) , _unref(group)));
}
DEFINE_PRIM(_VOID, Body_sleepWithGroup1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Body_isSleeping0)(_ref(cpBody)* _this) {
	return (cpBodyIsSleeping( _unref(_this) ));
}
DEFINE_PRIM(_I8, Body_isSleeping0, _IDL);

HL_PRIM double HL_NAME(Body_BodyGetMass0)(_ref(cpBody)* _this) {
	return (cpBodyGetMass( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyGetMass0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetMass1)(_ref(cpBody)* _this, double m) {
	(cpBodySetMass( _unref(_this) , m));
}
DEFINE_PRIM(_VOID, Body_BodySetMass1, _IDL _F64);

HL_PRIM double HL_NAME(Body_BodyGetMoment0)(_ref(cpBody)* _this) {
	return (cpBodyGetMoment( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyGetMoment0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetMoment1)(_ref(cpBody)* _this, double i) {
	(cpBodySetMoment( _unref(_this) , i));
}
DEFINE_PRIM(_VOID, Body_BodySetMoment1, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Body_getPosition0)(_ref(cpBody)* _this) {
	return alloc_ref(new cpVect((cpBodyGetPosition( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Body_getPosition0, _IDL);

HL_PRIM void HL_NAME(Body_setPosition1)(_ref(cpBody)* _this, _ref(cpVect)* pos) {
	(cpBodySetPosition( _unref(_this) , *_unref(pos)));
}
DEFINE_PRIM(_VOID, Body_setPosition1, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetCenterOfGravity0)(_ref(cpBody)* _this) {
	return alloc_ref(new cpVect((cpBodyGetCenterOfGravity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetCenterOfGravity0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetCenterOfGravity1)(_ref(cpBody)* _this, _ref(cpVect)* cog) {
	(cpBodySetCenterOfGravity( _unref(_this) , *_unref(cog)));
}
DEFINE_PRIM(_VOID, Body_BodySetCenterOfGravity1, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetVelocity0)(_ref(cpBody)* _this) {
	return alloc_ref(new cpVect((cpBodyGetVelocity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetVelocity0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetVelocity1)(_ref(cpBody)* _this, _ref(cpVect)* velocity) {
	(cpBodySetVelocity( _unref(_this) , *_unref(velocity)));
}
DEFINE_PRIM(_VOID, Body_BodySetVelocity1, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetForce0)(_ref(cpBody)* _this) {
	return alloc_ref(new cpVect((cpBodyGetForce( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetForce0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetForce1)(_ref(cpBody)* _this, _ref(cpVect)* force) {
	(cpBodySetForce( _unref(_this) , *_unref(force)));
}
DEFINE_PRIM(_VOID, Body_BodySetForce1, _IDL _IDL);

HL_PRIM double HL_NAME(Body_BodyGetAngle0)(_ref(cpBody)* _this) {
	return (cpBodyGetAngle( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyGetAngle0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetAngle1)(_ref(cpBody)* _this, double a) {
	(cpBodySetAngle( _unref(_this) , a));
}
DEFINE_PRIM(_VOID, Body_BodySetAngle1, _IDL _F64);

HL_PRIM double HL_NAME(Body_BodyGetAngularVelocity0)(_ref(cpBody)* _this) {
	return (cpBodyGetAngularVelocity( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyGetAngularVelocity0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetAngularVelocity1)(_ref(cpBody)* _this, double angularVelocity) {
	(cpBodySetAngularVelocity( _unref(_this) , angularVelocity));
}
DEFINE_PRIM(_VOID, Body_BodySetAngularVelocity1, _IDL _F64);

HL_PRIM double HL_NAME(Body_BodyGetTorque0)(_ref(cpBody)* _this) {
	return (cpBodyGetTorque( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyGetTorque0, _IDL);

HL_PRIM void HL_NAME(Body_BodySetTorque1)(_ref(cpBody)* _this, double torque) {
	(cpBodySetTorque( _unref(_this) , torque));
}
DEFINE_PRIM(_VOID, Body_BodySetTorque1, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetRotation0)(_ref(cpBody)* _this) {
	return alloc_ref(new cpVect((cpBodyGetRotation( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetRotation0, _IDL);

HL_PRIM void HL_NAME(Body_BodyUpdateVelocity3)(_ref(cpBody)* _this, _ref(cpVect)* gravity, double damping, double dt) {
	(cpBodyUpdateVelocity( _unref(_this) , *_unref(gravity), damping, dt));
}
DEFINE_PRIM(_VOID, Body_BodyUpdateVelocity3, _IDL _IDL _F64 _F64);

HL_PRIM void HL_NAME(Body_BodyUpdatePosition1)(_ref(cpBody)* _this, double dt) {
	(cpBodyUpdatePosition( _unref(_this) , dt));
}
DEFINE_PRIM(_VOID, Body_BodyUpdatePosition1, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyLocalToWorld1)(_ref(cpBody)* _this, _ref(cpVect)* point) {
	return alloc_ref(new cpVect((cpBodyLocalToWorld( _unref(_this) , *_unref(point)))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyLocalToWorld1, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyWorldToLocal1)(_ref(cpBody)* _this, _ref(cpVect)* point) {
	return alloc_ref(new cpVect((cpBodyWorldToLocal( _unref(_this) , *_unref(point)))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyWorldToLocal1, _IDL _IDL);

HL_PRIM void HL_NAME(Body_BodyApplyForceAtWorldPoint2)(_ref(cpBody)* _this, _ref(cpVect)* force, _ref(cpVect)* point) {
	(cpBodyApplyForceAtWorldPoint( _unref(_this) , *_unref(force), *_unref(point)));
}
DEFINE_PRIM(_VOID, Body_BodyApplyForceAtWorldPoint2, _IDL _IDL _IDL);

HL_PRIM void HL_NAME(Body_BodyApplyForceAtLocalPoint2)(_ref(cpBody)* _this, _ref(cpVect)* force, _ref(cpVect)* point) {
	(cpBodyApplyForceAtLocalPoint( _unref(_this) , *_unref(force), *_unref(point)));
}
DEFINE_PRIM(_VOID, Body_BodyApplyForceAtLocalPoint2, _IDL _IDL _IDL);

HL_PRIM void HL_NAME(Body_BodyApplyImpulseAtWorldPoint2)(_ref(cpBody)* _this, _ref(cpVect)* impulse, _ref(cpVect)* point) {
	(cpBodyApplyImpulseAtWorldPoint( _unref(_this) , *_unref(impulse), *_unref(point)));
}
DEFINE_PRIM(_VOID, Body_BodyApplyImpulseAtWorldPoint2, _IDL _IDL _IDL);

HL_PRIM void HL_NAME(Body_BodyApplyImpulseAtLocalPoint2)(_ref(cpBody)* _this, _ref(cpVect)* impulse, _ref(cpVect)* point) {
	(cpBodyApplyImpulseAtLocalPoint( _unref(_this) , *_unref(impulse), *_unref(point)));
}
DEFINE_PRIM(_VOID, Body_BodyApplyImpulseAtLocalPoint2, _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetVelocityAtWorldPoint1)(_ref(cpBody)* _this, _ref(cpVect)* point) {
	return alloc_ref(new cpVect((cpBodyGetVelocityAtWorldPoint( _unref(_this) , *_unref(point)))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetVelocityAtWorldPoint1, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Body_BodyGetVelocityAtLocalPoint1)(_ref(cpBody)* _this, _ref(cpVect)* point) {
	return alloc_ref(new cpVect((cpBodyGetVelocityAtLocalPoint( _unref(_this) , *_unref(point)))),Vect);
}
DEFINE_PRIM(_IDL, Body_BodyGetVelocityAtLocalPoint1, _IDL _IDL);

HL_PRIM double HL_NAME(Body_BodyKineticEnergy0)(_ref(cpBody)* _this) {
	return (cpBodyKineticEnergy( _unref(_this) ));
}
DEFINE_PRIM(_F64, Body_BodyKineticEnergy0, _IDL);

HL_PRIM _ref(cpPointQueryInfo)* HL_NAME(cpPointQueryInfo_new0)() {
	return alloc_ref((new cpPointQueryInfo()),cpPointQueryInfo);
}
DEFINE_PRIM(_IDL, cpPointQueryInfo_new0,);

HL_PRIM HL_CONST _ref(cpShape)* HL_NAME(cpPointQueryInfo_get_shape)( _ref(cpPointQueryInfo)* _this ) {
	return alloc_ref_const(_unref(_this)->shape,Shape);
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_get_shape,_IDL);
HL_PRIM HL_CONST _ref(cpShape)* HL_NAME(cpPointQueryInfo_set_shape)( _ref(cpPointQueryInfo)* _this, HL_CONST _ref(cpShape)* value ) {
	_unref(_this)->shape = _unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_set_shape,_IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPointQueryInfo_get_point)( _ref(cpPointQueryInfo)* _this ) {
	return alloc_ref(new cpVect(_unref(_this)->point),Vect);
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_get_point,_IDL);
HL_PRIM _ref(cpVect)* HL_NAME(cpPointQueryInfo_set_point)( _ref(cpPointQueryInfo)* _this, _ref(cpVect)* value ) {
	_unref(_this)->point = *_unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_set_point,_IDL _IDL);

HL_PRIM double HL_NAME(cpPointQueryInfo_get_distance)( _ref(cpPointQueryInfo)* _this ) {
	return _unref(_this)->distance;
}
DEFINE_PRIM(_F64,cpPointQueryInfo_get_distance,_IDL);
HL_PRIM double HL_NAME(cpPointQueryInfo_set_distance)( _ref(cpPointQueryInfo)* _this, double value ) {
	_unref(_this)->distance = (value);
	return value;
}
DEFINE_PRIM(_F64,cpPointQueryInfo_set_distance,_IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(cpPointQueryInfo_get_gradient)( _ref(cpPointQueryInfo)* _this ) {
	return alloc_ref(new cpVect(_unref(_this)->gradient),Vect);
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_get_gradient,_IDL);
HL_PRIM _ref(cpVect)* HL_NAME(cpPointQueryInfo_set_gradient)( _ref(cpPointQueryInfo)* _this, _ref(cpVect)* value ) {
	_unref(_this)->gradient = *_unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpPointQueryInfo_set_gradient,_IDL _IDL);

HL_PRIM _ref(cpSegmentQueryInfo)* HL_NAME(cpSegmentQueryInfo_new0)() {
	return alloc_ref((new cpSegmentQueryInfo()),cpSegmentQueryInfo);
}
DEFINE_PRIM(_IDL, cpSegmentQueryInfo_new0,);

HL_PRIM HL_CONST _ref(cpShape)* HL_NAME(cpSegmentQueryInfo_get_shape)( _ref(cpSegmentQueryInfo)* _this ) {
	return alloc_ref_const(_unref(_this)->shape,Shape);
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_get_shape,_IDL);
HL_PRIM HL_CONST _ref(cpShape)* HL_NAME(cpSegmentQueryInfo_set_shape)( _ref(cpSegmentQueryInfo)* _this, HL_CONST _ref(cpShape)* value ) {
	_unref(_this)->shape = _unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_set_shape,_IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpSegmentQueryInfo_get_point)( _ref(cpSegmentQueryInfo)* _this ) {
	return alloc_ref(new cpVect(_unref(_this)->point),Vect);
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_get_point,_IDL);
HL_PRIM _ref(cpVect)* HL_NAME(cpSegmentQueryInfo_set_point)( _ref(cpSegmentQueryInfo)* _this, _ref(cpVect)* value ) {
	_unref(_this)->point = *_unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_set_point,_IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpSegmentQueryInfo_get_normal)( _ref(cpSegmentQueryInfo)* _this ) {
	return alloc_ref(new cpVect(_unref(_this)->normal),Vect);
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_get_normal,_IDL);
HL_PRIM _ref(cpVect)* HL_NAME(cpSegmentQueryInfo_set_normal)( _ref(cpSegmentQueryInfo)* _this, _ref(cpVect)* value ) {
	_unref(_this)->normal = *_unref(value);
	return value;
}
DEFINE_PRIM(_IDL,cpSegmentQueryInfo_set_normal,_IDL _IDL);

HL_PRIM double HL_NAME(cpSegmentQueryInfo_get_alpha)( _ref(cpSegmentQueryInfo)* _this ) {
	return _unref(_this)->alpha;
}
DEFINE_PRIM(_F64,cpSegmentQueryInfo_get_alpha,_IDL);
HL_PRIM double HL_NAME(cpSegmentQueryInfo_set_alpha)( _ref(cpSegmentQueryInfo)* _this, double value ) {
	_unref(_this)->alpha = (value);
	return value;
}
DEFINE_PRIM(_F64,cpSegmentQueryInfo_set_alpha,_IDL _F64);

HL_PRIM _ref(cpShapeFilter)* HL_NAME(cpShapeFilter_new0)() {
	return alloc_ref((new cpShapeFilter()),cpShapeFilter);
}
DEFINE_PRIM(_IDL, cpShapeFilter_new0,);

HL_PRIM int64_t HL_NAME(cpShapeFilter_get_group)( _ref(cpShapeFilter)* _this ) {
	return _unref(_this)->group;
}
DEFINE_PRIM(_I64,cpShapeFilter_get_group,_IDL);
HL_PRIM int64_t HL_NAME(cpShapeFilter_set_group)( _ref(cpShapeFilter)* _this, int64_t value ) {
	_unref(_this)->group = (value);
	return value;
}
DEFINE_PRIM(_I64,cpShapeFilter_set_group,_IDL _I64);

HL_PRIM unsigned int HL_NAME(cpShapeFilter_get_categories)( _ref(cpShapeFilter)* _this ) {
	return _unref(_this)->categories;
}
DEFINE_PRIM(_I32,cpShapeFilter_get_categories,_IDL);
HL_PRIM unsigned int HL_NAME(cpShapeFilter_set_categories)( _ref(cpShapeFilter)* _this, unsigned int value ) {
	_unref(_this)->categories = (value);
	return value;
}
DEFINE_PRIM(_I32,cpShapeFilter_set_categories,_IDL _I32);

HL_PRIM unsigned int HL_NAME(cpShapeFilter_get_mask)( _ref(cpShapeFilter)* _this ) {
	return _unref(_this)->mask;
}
DEFINE_PRIM(_I32,cpShapeFilter_get_mask,_IDL);
HL_PRIM unsigned int HL_NAME(cpShapeFilter_set_mask)( _ref(cpShapeFilter)* _this, unsigned int value ) {
	_unref(_this)->mask = (value);
	return value;
}
DEFINE_PRIM(_I32,cpShapeFilter_set_mask,_IDL _I32);

HL_PRIM _ref(cpShapeFilter)* HL_NAME(Shape_cpShapeFilterNew3)(int64_t group, unsigned int categories, unsigned int mask) {
	return alloc_ref(new cpShapeFilter((cpShapeFilterNew(group, categories, mask))),cpShapeFilter);
}
DEFINE_PRIM(_IDL, Shape_cpShapeFilterNew3, _I64 _I32 _I32);

HL_PRIM void HL_NAME(Shape_cpShapeDestroy0)(_ref(cpShape)* _this) {
	(cpShapeDestroy( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Shape_cpShapeDestroy0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeFree0)(_ref(cpShape)* _this) {
	(cpShapeFree( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Shape_cpShapeFree0, _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(Shape_cpShapeCacheBB0)(_ref(cpShape)* _this) {
	return alloc_ref(new cpBB((cpShapeCacheBB( _unref(_this) ))),cpBB);
}
DEFINE_PRIM(_IDL, Shape_cpShapeCacheBB0, _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(Shape_cpShapeUpdate1)(_ref(cpShape)* _this, _ref(cpTransform)* transform) {
	return alloc_ref(new cpBB((cpShapeUpdate( _unref(_this) , *_unref(transform)))),cpBB);
}
DEFINE_PRIM(_IDL, Shape_cpShapeUpdate1, _IDL _IDL);

HL_PRIM double HL_NAME(Shape_cpShapePointQuery2)(_ref(cpShape)* _this, _ref(cpVect)* p, _ref(cpPointQueryInfo)* out) {
	return (cpShapePointQuery( _unref(_this) , *_unref(p), _unref(out)));
}
DEFINE_PRIM(_F64, Shape_cpShapePointQuery2, _IDL _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Shape_cpShapeSegmentQuery4)(_ref(cpShape)* _this, _ref(cpVect)* a, _ref(cpVect)* b, double radius, _ref(cpSegmentQueryInfo)* info) {
	return (cpShapeSegmentQuery( _unref(_this) , *_unref(a), *_unref(b), radius, _unref(info)));
}
DEFINE_PRIM(_I8, Shape_cpShapeSegmentQuery4, _IDL _IDL _IDL _F64 _IDL);

HL_PRIM _ref(cpContactPointSet)* HL_NAME(Shape_cpShapesCollide2)(_ref(cpShape)* a, _ref(cpShape)* b) {
	return alloc_ref(new cpContactPointSet((cpShapesCollide(_unref(a), _unref(b)))),cpContactPointSet);
}
DEFINE_PRIM(_IDL, Shape_cpShapesCollide2, _IDL _IDL);

HL_PRIM _ref(cpSpace)* HL_NAME(Shape_cpShapeGetSpace0)(_ref(cpShape)* _this) {
	return alloc_ref((cpShapeGetSpace( _unref(_this) )),Space);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetSpace0, _IDL);

HL_PRIM _ref(cpBody)* HL_NAME(Shape_cpShapeGetBody0)(_ref(cpShape)* _this) {
	return alloc_ref((cpShapeGetBody( _unref(_this) )),Body);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetBody0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetBody1)(_ref(cpShape)* _this, _ref(cpBody)* body) {
	(cpShapeSetBody( _unref(_this) , _unref(body)));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetBody1, _IDL _IDL);

HL_PRIM double HL_NAME(Shape_cpShapeGetMass0)(_ref(cpShape)* _this) {
	return (cpShapeGetMass( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_cpShapeGetMass0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetMass1)(_ref(cpShape)* _this, double mass) {
	(cpShapeSetMass( _unref(_this) , mass));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetMass1, _IDL _F64);

HL_PRIM double HL_NAME(Shape_cpShapeGetDensity0)(_ref(cpShape)* _this) {
	return (cpShapeGetDensity( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_cpShapeGetDensity0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetDensity1)(_ref(cpShape)* _this, double density) {
	(cpShapeSetDensity( _unref(_this) , density));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetDensity1, _IDL _F64);

HL_PRIM double HL_NAME(Shape_cpShapeGetMoment0)(_ref(cpShape)* _this) {
	return (cpShapeGetMoment( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_cpShapeGetMoment0, _IDL);

HL_PRIM double HL_NAME(Shape_cpShapeGetArea0)(_ref(cpShape)* _this) {
	return (cpShapeGetArea( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_cpShapeGetArea0, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(Shape_cpShapeGetCenterOfGravity0)(_ref(cpShape)* _this) {
	return alloc_ref(new cpVect((cpShapeGetCenterOfGravity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetCenterOfGravity0, _IDL);

HL_PRIM _ref(cpBB)* HL_NAME(Shape_cpShapeGetBB0)(_ref(cpShape)* _this) {
	return alloc_ref(new cpBB((cpShapeGetBB( _unref(_this) ))),cpBB);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetBB0, _IDL);

HL_PRIM unsigned char HL_NAME(Shape_cpShapeGetSensor0)(_ref(cpShape)* _this) {
	return (cpShapeGetSensor( _unref(_this) ));
}
DEFINE_PRIM(_I8, Shape_cpShapeGetSensor0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetSensor1)(_ref(cpShape)* _this, unsigned char sensor) {
	(cpShapeSetSensor( _unref(_this) , sensor));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetSensor1, _IDL _I8);

HL_PRIM double HL_NAME(Shape_getElasticity0)(_ref(cpShape)* _this) {
	return (cpShapeGetElasticity( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_getElasticity0, _IDL);

HL_PRIM void HL_NAME(Shape_setElasticity1)(_ref(cpShape)* _this, double elasticity) {
	(cpShapeSetElasticity( _unref(_this) , elasticity));
}
DEFINE_PRIM(_VOID, Shape_setElasticity1, _IDL _F64);

HL_PRIM double HL_NAME(Shape_getFriction0)(_ref(cpShape)* _this) {
	return (cpShapeGetFriction( _unref(_this) ));
}
DEFINE_PRIM(_F64, Shape_getFriction0, _IDL);

HL_PRIM void HL_NAME(Shape_setFriction1)(_ref(cpShape)* _this, double friction) {
	(cpShapeSetFriction( _unref(_this) , friction));
}
DEFINE_PRIM(_VOID, Shape_setFriction1, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Shape_cpShapeGetSurfaceVelocity0)(_ref(cpShape)* _this) {
	return alloc_ref(new cpVect((cpShapeGetSurfaceVelocity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetSurfaceVelocity0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetSurfaceVelocity1)(_ref(cpShape)* _this, _ref(cpVect)* surfaceVelocity) {
	(cpShapeSetSurfaceVelocity( _unref(_this) , *_unref(surfaceVelocity)));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetSurfaceVelocity1, _IDL _IDL);

HL_PRIM void* HL_NAME(Shape_cpShapeGetUserData0)(_ref(cpShape)* _this) {
	return (cpShapeGetUserData( _unref(_this) ));
}
DEFINE_PRIM(_BYTES, Shape_cpShapeGetUserData0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetUserData1)(_ref(cpShape)* _this, void* userData) {
	(cpShapeSetUserData( _unref(_this) , userData));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetUserData1, _IDL _BYTES);

HL_PRIM _ref(cpShapeFilter)* HL_NAME(Shape_cpShapeGetFilter0)(_ref(cpShape)* _this) {
	return alloc_ref(new cpShapeFilter((cpShapeGetFilter( _unref(_this) ))),cpShapeFilter);
}
DEFINE_PRIM(_IDL, Shape_cpShapeGetFilter0, _IDL);

HL_PRIM void HL_NAME(Shape_cpShapeSetFilter1)(_ref(cpShape)* _this, _ref(cpShapeFilter)* filter) {
	(cpShapeSetFilter( _unref(_this) , *_unref(filter)));
}
DEFINE_PRIM(_VOID, Shape_cpShapeSetFilter1, _IDL _IDL);

HL_PRIM _ref(cpCircleShape)* HL_NAME(CircleShape_alloc0)() {
	return alloc_ref((cpCircleShapeAlloc()),CircleShape);
}
DEFINE_PRIM(_IDL, CircleShape_alloc0,);

HL_PRIM _ref(cpCircleShape)* HL_NAME(CircleShape_init4)(_ref(cpCircleShape)* circle, _ref(cpBody)* body, double radius, _ref(cpVect)* offset) {
	return alloc_ref((cpCircleShapeInit(_unref(circle), _unref(body), radius, *_unref(offset))),CircleShape);
}
DEFINE_PRIM(_IDL, CircleShape_init4, _IDL _IDL _F64 _IDL);

HL_PRIM _ref(cpShape)* HL_NAME(CircleShape_makeNew3)(_ref(cpBody)* body, double radius, _ref(cpVect)* offset) {
	return alloc_ref((cpCircleShapeNew(_unref(body), radius, *_unref(offset))),Shape);
}
DEFINE_PRIM(_IDL, CircleShape_makeNew3, _IDL _F64 _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(CircleShape_getOffset1)(_ref(cpShape)* shape) {
	return alloc_ref(new cpVect((cpCircleShapeGetOffset(_unref(shape)))),Vect);
}
DEFINE_PRIM(_IDL, CircleShape_getOffset1, _IDL);

HL_PRIM double HL_NAME(CircleShape_getRadius1)(_ref(cpShape)* shape) {
	return (cpCircleShapeGetRadius(_unref(shape)));
}
DEFINE_PRIM(_F64, CircleShape_getRadius1, _IDL);

HL_PRIM _ref(cpSegmentShape)* HL_NAME(SegmentShape_alloc0)() {
	return alloc_ref((cpSegmentShapeAlloc()),SegmentShape);
}
DEFINE_PRIM(_IDL, SegmentShape_alloc0,);

HL_PRIM _ref(cpSegmentShape)* HL_NAME(SegmentShape_init4)(_ref(cpSegmentShape)* _this, _ref(cpBody)* body, _ref(cpVect)* a, _ref(cpVect)* b, double radius) {
	return alloc_ref((cpSegmentShapeInit( _unref(_this) , _unref(body), *_unref(a), *_unref(b), radius)),SegmentShape);
}
DEFINE_PRIM(_IDL, SegmentShape_init4, _IDL _IDL _IDL _IDL _F64);

HL_PRIM _ref(cpShape)* HL_NAME(SegmentShape_newSegmentShape4)(_ref(cpBody)* body, _ref(cpVect)* a, _ref(cpVect)* b, double radius) {
	return alloc_ref((cpSegmentShapeNew(_unref(body), *_unref(a), *_unref(b), radius)),Shape);
}
DEFINE_PRIM(_IDL, SegmentShape_newSegmentShape4, _IDL _IDL _IDL _F64);

HL_PRIM void HL_NAME(SegmentShape_setNeighbors3)(_ref(cpShape)* shape, _ref(cpVect)* prev, _ref(cpVect)* next) {
	(cpSegmentShapeSetNeighbors(_unref(shape), *_unref(prev), *_unref(next)));
}
DEFINE_PRIM(_VOID, SegmentShape_setNeighbors3, _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(SegmentShape_getA1)(_ref(cpShape)* shape) {
	return alloc_ref(new cpVect((cpSegmentShapeGetA(_unref(shape)))),Vect);
}
DEFINE_PRIM(_IDL, SegmentShape_getA1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(SegmentShape_getB1)(_ref(cpShape)* shape) {
	return alloc_ref(new cpVect((cpSegmentShapeGetB(_unref(shape)))),Vect);
}
DEFINE_PRIM(_IDL, SegmentShape_getB1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(SegmentShape_getNormal1)(_ref(cpShape)* shape) {
	return alloc_ref(new cpVect((cpSegmentShapeGetNormal(_unref(shape)))),Vect);
}
DEFINE_PRIM(_IDL, SegmentShape_getNormal1, _IDL);

HL_PRIM double HL_NAME(SegmentShape_getRadius1)(_ref(cpShape)* shape) {
	return (cpSegmentShapeGetRadius(_unref(shape)));
}
DEFINE_PRIM(_F64, SegmentShape_getRadius1, _IDL);

HL_PRIM _ref(cpSpace)* HL_NAME(Space_init1)(_ref(cpSpace)* space) {
	return alloc_ref((cpSpaceInit(_unref(space))),Space);
}
DEFINE_PRIM(_IDL, Space_init1, _IDL);

HL_PRIM _ref(cpSpace)* HL_NAME(Space_makeNew0)() {
	return alloc_ref((cpSpaceNew()),Space);
}
DEFINE_PRIM(_IDL, Space_makeNew0,);

HL_PRIM void HL_NAME(Space_destroy0)(_ref(cpSpace)* _this) {
	(cpSpaceDestroy( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Space_destroy0, _IDL);

HL_PRIM void HL_NAME(Space_free0)(_ref(cpSpace)* _this) {
	(cpSpaceFree( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Space_free0, _IDL);

HL_PRIM int HL_NAME(Space_getIterations0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetIterations( _unref(_this) ));
}
DEFINE_PRIM(_I32, Space_getIterations0, _IDL);

HL_PRIM void HL_NAME(Space_setIterations1)(_ref(cpSpace)* _this, int iterations) {
	(cpSpaceSetIterations( _unref(_this) , iterations));
}
DEFINE_PRIM(_VOID, Space_setIterations1, _IDL _I32);

HL_PRIM _ref(cpVect)* HL_NAME(Space_cpSpaceGetGravity0)(_ref(cpSpace)* _this) {
	return alloc_ref(new cpVect((cpSpaceGetGravity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, Space_cpSpaceGetGravity0, _IDL);

HL_PRIM void HL_NAME(Space_setGravity1)(_ref(cpSpace)* _this, _ref(cpVect)* gravity) {
	(cpSpaceSetGravity( _unref(_this) , *_unref(gravity)));
}
DEFINE_PRIM(_VOID, Space_setGravity1, _IDL _IDL);

HL_PRIM double HL_NAME(Space_getDamping0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetDamping( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getDamping0, _IDL);

HL_PRIM void HL_NAME(Space_setDamping1)(_ref(cpSpace)* _this, double damping) {
	(cpSpaceSetDamping( _unref(_this) , damping));
}
DEFINE_PRIM(_VOID, Space_setDamping1, _IDL _F64);

HL_PRIM double HL_NAME(Space_getIdleSpeedThreshold0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetIdleSpeedThreshold( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getIdleSpeedThreshold0, _IDL);

HL_PRIM void HL_NAME(Space_setIdleSpeedThreshold1)(_ref(cpSpace)* _this, double idleSpeedThreshold) {
	(cpSpaceSetIdleSpeedThreshold( _unref(_this) , idleSpeedThreshold));
}
DEFINE_PRIM(_VOID, Space_setIdleSpeedThreshold1, _IDL _F64);

HL_PRIM double HL_NAME(Space_getSleepTimeThreshold0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetSleepTimeThreshold( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getSleepTimeThreshold0, _IDL);

HL_PRIM void HL_NAME(Space_setSleepTimeThreshold1)(_ref(cpSpace)* _this, double sleepTimeThreshold) {
	(cpSpaceSetSleepTimeThreshold( _unref(_this) , sleepTimeThreshold));
}
DEFINE_PRIM(_VOID, Space_setSleepTimeThreshold1, _IDL _F64);

HL_PRIM double HL_NAME(Space_getCollisionSlop0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetCollisionSlop( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getCollisionSlop0, _IDL);

HL_PRIM void HL_NAME(Space_setCollisionSlop1)(_ref(cpSpace)* _this, double collisionSlop) {
	(cpSpaceSetCollisionSlop( _unref(_this) , collisionSlop));
}
DEFINE_PRIM(_VOID, Space_setCollisionSlop1, _IDL _F64);

HL_PRIM double HL_NAME(Space_getCollisionBias0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetCollisionBias( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getCollisionBias0, _IDL);

HL_PRIM void HL_NAME(Space_setCollisionBias1)(_ref(cpSpace)* _this, double collisionBias) {
	(cpSpaceSetCollisionBias( _unref(_this) , collisionBias));
}
DEFINE_PRIM(_VOID, Space_setCollisionBias1, _IDL _F64);

HL_PRIM void* HL_NAME(Space_getUserData0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetUserData( _unref(_this) ));
}
DEFINE_PRIM(_BYTES, Space_getUserData0, _IDL);

HL_PRIM void HL_NAME(Space_setUserData1)(_ref(cpSpace)* _this, void* userData) {
	(cpSpaceSetUserData( _unref(_this) , userData));
}
DEFINE_PRIM(_VOID, Space_setUserData1, _IDL _BYTES);

HL_PRIM _ref(cpBody)* HL_NAME(Space_getStaticBody0)(_ref(cpSpace)* _this) {
	return alloc_ref((cpSpaceGetStaticBody( _unref(_this) )),Body);
}
DEFINE_PRIM(_IDL, Space_getStaticBody0, _IDL);

HL_PRIM double HL_NAME(Space_getCurrentTimeStep0)(_ref(cpSpace)* _this) {
	return (cpSpaceGetCurrentTimeStep( _unref(_this) ));
}
DEFINE_PRIM(_F64, Space_getCurrentTimeStep0, _IDL);

HL_PRIM unsigned char HL_NAME(Space_isLocked0)(_ref(cpSpace)* _this) {
	return (cpSpaceIsLocked( _unref(_this) ));
}
DEFINE_PRIM(_I8, Space_isLocked0, _IDL);

HL_PRIM void HL_NAME(Space_addShape1)(_ref(cpSpace)* _this, _ref(cpShape)* shape) {
	(cpSpaceAddShape( _unref(_this) , _unref(shape)));
}
DEFINE_PRIM(_VOID, Space_addShape1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_addBody1)(_ref(cpSpace)* _this, _ref(cpBody)* body) {
	(cpSpaceAddBody( _unref(_this) , _unref(body)));
}
DEFINE_PRIM(_VOID, Space_addBody1, _IDL _IDL);

HL_PRIM _ref(cpConstraint)* HL_NAME(Space_cpSpaceAddConstraint1)(_ref(cpSpace)* _this, _ref(cpConstraint)* constraint) {
	return alloc_ref((cpSpaceAddConstraint( _unref(_this) , _unref(constraint))),Constraint);
}
DEFINE_PRIM(_IDL, Space_cpSpaceAddConstraint1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceRemoveShape1)(_ref(cpSpace)* _this, _ref(cpShape)* shape) {
	(cpSpaceRemoveShape( _unref(_this) , _unref(shape)));
}
DEFINE_PRIM(_VOID, Space_cpSpaceRemoveShape1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceRemoveBody1)(_ref(cpSpace)* _this, _ref(cpBody)* body) {
	(cpSpaceRemoveBody( _unref(_this) , _unref(body)));
}
DEFINE_PRIM(_VOID, Space_cpSpaceRemoveBody1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceRemoveConstraint1)(_ref(cpSpace)* _this, _ref(cpConstraint)* constraint) {
	(cpSpaceRemoveConstraint( _unref(_this) , _unref(constraint)));
}
DEFINE_PRIM(_VOID, Space_cpSpaceRemoveConstraint1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Space_cpSpaceContainsShape1)(_ref(cpSpace)* _this, _ref(cpShape)* shape) {
	return (cpSpaceContainsShape( _unref(_this) , _unref(shape)));
}
DEFINE_PRIM(_I8, Space_cpSpaceContainsShape1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Space_cpSpaceContainsBody1)(_ref(cpSpace)* _this, _ref(cpBody)* body) {
	return (cpSpaceContainsBody( _unref(_this) , _unref(body)));
}
DEFINE_PRIM(_I8, Space_cpSpaceContainsBody1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Space_cpSpaceContainsConstraint1)(_ref(cpSpace)* _this, _ref(cpConstraint)* constraint) {
	return (cpSpaceContainsConstraint( _unref(_this) , _unref(constraint)));
}
DEFINE_PRIM(_I8, Space_cpSpaceContainsConstraint1, _IDL _IDL);

HL_PRIM _ref(cpShape)* HL_NAME(Space_cpSpaceSegmentQueryFirst5)(_ref(cpSpace)* _this, _ref(cpVect)* start, _ref(cpVect)* end, double radius, _ref(cpShapeFilter)* filter, _ref(cpSegmentQueryInfo)* out) {
	return alloc_ref((cpSpaceSegmentQueryFirst( _unref(_this) , *_unref(start), *_unref(end), radius, *_unref(filter), _unref(out))),Shape);
}
DEFINE_PRIM(_IDL, Space_cpSpaceSegmentQueryFirst5, _IDL _IDL _IDL _F64 _IDL _IDL);

HL_PRIM _ref(cpShape)* HL_NAME(Space_cpSpaceSegmentQueryFirstNoFilter4)(_ref(cpSpace)* _this, _ref(cpVect)* start, _ref(cpVect)* end, double radius, _ref(cpSegmentQueryInfo)* out) {
	return alloc_ref((cpSpaceSegmentQueryFirstNoFilter( _unref(_this) , *_unref(start), *_unref(end), radius, _unref(out))),Shape);
}
DEFINE_PRIM(_IDL, Space_cpSpaceSegmentQueryFirstNoFilter4, _IDL _IDL _IDL _F64 _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceReindexStatic0)(_ref(cpSpace)* _this) {
	(cpSpaceReindexStatic( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Space_cpSpaceReindexStatic0, _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceReindexShape1)(_ref(cpSpace)* _this, _ref(cpShape)* shape) {
	(cpSpaceReindexShape( _unref(_this) , _unref(shape)));
}
DEFINE_PRIM(_VOID, Space_cpSpaceReindexShape1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceReindexShapesForBody1)(_ref(cpSpace)* _this, _ref(cpBody)* body) {
	(cpSpaceReindexShapesForBody( _unref(_this) , _unref(body)));
}
DEFINE_PRIM(_VOID, Space_cpSpaceReindexShapesForBody1, _IDL _IDL);

HL_PRIM void HL_NAME(Space_cpSpaceUseSpatialHash2)(_ref(cpSpace)* _this, double dim, int count) {
	(cpSpaceUseSpatialHash( _unref(_this) , dim, count));
}
DEFINE_PRIM(_VOID, Space_cpSpaceUseSpatialHash2, _IDL _F64 _I32);

HL_PRIM void HL_NAME(Space_step1)(_ref(cpSpace)* _this, double dt) {
	(cpSpaceStep( _unref(_this) , dt));
}
DEFINE_PRIM(_VOID, Space_step1, _IDL _F64);

HL_PRIM double HL_NAME(cpArbiter_cpArbiterGetRestitution0)(_ref(cpArbiter)* _this) {
	return (cpArbiterGetRestitution( _unref(_this) ));
}
DEFINE_PRIM(_F64, cpArbiter_cpArbiterGetRestitution0, _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterSetRestitution1)(_ref(cpArbiter)* _this, double restitution) {
	(cpArbiterSetRestitution( _unref(_this) , restitution));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterSetRestitution1, _IDL _F64);

HL_PRIM double HL_NAME(cpArbiter_cpArbiterGetFriction0)(_ref(cpArbiter)* _this) {
	return (cpArbiterGetFriction( _unref(_this) ));
}
DEFINE_PRIM(_F64, cpArbiter_cpArbiterGetFriction0, _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterSetFriction1)(_ref(cpArbiter)* _this, double friction) {
	(cpArbiterSetFriction( _unref(_this) , friction));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterSetFriction1, _IDL _F64);

HL_PRIM _ref(cpVect)* HL_NAME(cpArbiter_cpArbiterGetSurfaceVelocity0)(_ref(cpArbiter)* _this) {
	return alloc_ref(new cpVect((cpArbiterGetSurfaceVelocity( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterGetSurfaceVelocity0, _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterSetSurfaceVelocity1)(_ref(cpArbiter)* _this, _ref(cpVect)* vr) {
	(cpArbiterSetSurfaceVelocity( _unref(_this) , *_unref(vr)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterSetSurfaceVelocity1, _IDL _IDL);

HL_PRIM void* HL_NAME(cpArbiter_cpArbiterGetUserData0)(_ref(cpArbiter)* _this) {
	return (cpArbiterGetUserData( _unref(_this) ));
}
DEFINE_PRIM(_BYTES, cpArbiter_cpArbiterGetUserData0, _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterSetUserData1)(_ref(cpArbiter)* _this, void* userData) {
	(cpArbiterSetUserData( _unref(_this) , userData));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterSetUserData1, _IDL _BYTES);

HL_PRIM _ref(cpVect)* HL_NAME(cpArbiter_cpArbiterTotalImpulse0)(_ref(cpArbiter)* _this) {
	return alloc_ref(new cpVect((cpArbiterTotalImpulse( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterTotalImpulse0, _IDL);

HL_PRIM double HL_NAME(cpArbiter_cpArbiterTotalKE0)(_ref(cpArbiter)* _this) {
	return (cpArbiterTotalKE( _unref(_this) ));
}
DEFINE_PRIM(_F64, cpArbiter_cpArbiterTotalKE0, _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterIgnore0)(_ref(cpArbiter)* _this) {
	return (cpArbiterIgnore( _unref(_this) ));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterIgnore0, _IDL);

HL_PRIM _ref(cpContactPointSet)* HL_NAME(cpArbiter_cpArbiterGetContactPointSet0)(_ref(cpArbiter)* _this) {
	return alloc_ref(new cpContactPointSet((cpArbiterGetContactPointSet( _unref(_this) ))),cpContactPointSet);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterGetContactPointSet0, _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterSetContactPointSet1)(_ref(cpArbiter)* _this, _ref(cpContactPointSet)* set) {
	(cpArbiterSetContactPointSet( _unref(_this) , _unref(set)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterSetContactPointSet1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterIsFirstContact0)(_ref(cpArbiter)* _this) {
	return (cpArbiterIsFirstContact( _unref(_this) ));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterIsFirstContact0, _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterIsRemoval0)(_ref(cpArbiter)* _this) {
	return (cpArbiterIsRemoval( _unref(_this) ));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterIsRemoval0, _IDL);

HL_PRIM int HL_NAME(cpArbiter_cpArbiterGetCount0)(_ref(cpArbiter)* _this) {
	return (cpArbiterGetCount( _unref(_this) ));
}
DEFINE_PRIM(_I32, cpArbiter_cpArbiterGetCount0, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpArbiter_cpArbiterGetNormal0)(_ref(cpArbiter)* _this) {
	return alloc_ref(new cpVect((cpArbiterGetNormal( _unref(_this) ))),Vect);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterGetNormal0, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpArbiter_cpArbiterGetPointA1)(_ref(cpArbiter)* _this, int i) {
	return alloc_ref(new cpVect((cpArbiterGetPointA( _unref(_this) , i))),Vect);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterGetPointA1, _IDL _I32);

HL_PRIM _ref(cpVect)* HL_NAME(cpArbiter_cpArbiterGetPointB1)(_ref(cpArbiter)* _this, int i) {
	return alloc_ref(new cpVect((cpArbiterGetPointB( _unref(_this) , i))),Vect);
}
DEFINE_PRIM(_IDL, cpArbiter_cpArbiterGetPointB1, _IDL _I32);

HL_PRIM double HL_NAME(cpArbiter_cpArbiterGetDepth1)(_ref(cpArbiter)* _this, int i) {
	return (cpArbiterGetDepth( _unref(_this) , i));
}
DEFINE_PRIM(_F64, cpArbiter_cpArbiterGetDepth1, _IDL _I32);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterCallWildcardBeginA1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	return (cpArbiterCallWildcardBeginA( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterCallWildcardBeginA1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterCallWildcardBeginB1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	return (cpArbiterCallWildcardBeginB( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterCallWildcardBeginB1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterCallWildcardPreSolveA1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	return (cpArbiterCallWildcardPreSolveA( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterCallWildcardPreSolveA1, _IDL _IDL);

HL_PRIM unsigned char HL_NAME(cpArbiter_cpArbiterCallWildcardPreSolveB1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	return (cpArbiterCallWildcardPreSolveB( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_I8, cpArbiter_cpArbiterCallWildcardPreSolveB1, _IDL _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterCallWildcardPostSolveA1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	(cpArbiterCallWildcardPostSolveA( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterCallWildcardPostSolveA1, _IDL _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterCallWildcardPostSolveB1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	(cpArbiterCallWildcardPostSolveB( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterCallWildcardPostSolveB1, _IDL _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterCallWildcardSeparateA1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	(cpArbiterCallWildcardSeparateA( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterCallWildcardSeparateA1, _IDL _IDL);

HL_PRIM void HL_NAME(cpArbiter_cpArbiterCallWildcardSeparateB1)(_ref(cpArbiter)* _this, _ref(cpSpace)* space) {
	(cpArbiterCallWildcardSeparateB( _unref(_this) , _unref(space)));
}
DEFINE_PRIM(_VOID, cpArbiter_cpArbiterCallWildcardSeparateB1, _IDL _IDL);

HL_PRIM void HL_NAME(Constraint_destroy0)(_ref(cpConstraint)* _this) {
	(cpConstraintDestroy( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Constraint_destroy0, _IDL);

HL_PRIM void HL_NAME(Constraint_free0)(_ref(cpConstraint)* _this) {
	(cpConstraintFree( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Constraint_free0, _IDL);

HL_PRIM _ref(cpSpace)* HL_NAME(Constraint_getSpace0)(_ref(cpConstraint)* _this) {
	return alloc_ref((cpConstraintGetSpace( _unref(_this) )),Space);
}
DEFINE_PRIM(_IDL, Constraint_getSpace0, _IDL);

HL_PRIM _ref(cpBody)* HL_NAME(Constraint_getBodyA0)(_ref(cpConstraint)* _this) {
	return alloc_ref((cpConstraintGetBodyA( _unref(_this) )),Body);
}
DEFINE_PRIM(_IDL, Constraint_getBodyA0, _IDL);

HL_PRIM _ref(cpBody)* HL_NAME(Constraint_getBodyB0)(_ref(cpConstraint)* _this) {
	return alloc_ref((cpConstraintGetBodyB( _unref(_this) )),Body);
}
DEFINE_PRIM(_IDL, Constraint_getBodyB0, _IDL);

HL_PRIM double HL_NAME(Constraint_getMaxForce0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetMaxForce( _unref(_this) ));
}
DEFINE_PRIM(_F64, Constraint_getMaxForce0, _IDL);

HL_PRIM void HL_NAME(Constraint_setMaxForce1)(_ref(cpConstraint)* _this, double maxForce) {
	(cpConstraintSetMaxForce( _unref(_this) , maxForce));
}
DEFINE_PRIM(_VOID, Constraint_setMaxForce1, _IDL _F64);

HL_PRIM double HL_NAME(Constraint_getErrorBias0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetErrorBias( _unref(_this) ));
}
DEFINE_PRIM(_F64, Constraint_getErrorBias0, _IDL);

HL_PRIM void HL_NAME(Constraint_setErrorBias1)(_ref(cpConstraint)* _this, double errorBias) {
	(cpConstraintSetErrorBias( _unref(_this) , errorBias));
}
DEFINE_PRIM(_VOID, Constraint_setErrorBias1, _IDL _F64);

HL_PRIM double HL_NAME(Constraint_getMaxBias0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetMaxBias( _unref(_this) ));
}
DEFINE_PRIM(_F64, Constraint_getMaxBias0, _IDL);

HL_PRIM void HL_NAME(Constraint_setMaxBias1)(_ref(cpConstraint)* _this, double maxBias) {
	(cpConstraintSetMaxBias( _unref(_this) , maxBias));
}
DEFINE_PRIM(_VOID, Constraint_setMaxBias1, _IDL _F64);

HL_PRIM unsigned char HL_NAME(Constraint_getCollideBodies0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetCollideBodies( _unref(_this) ));
}
DEFINE_PRIM(_I8, Constraint_getCollideBodies0, _IDL);

HL_PRIM void HL_NAME(Constraint_cpConstraintSetCollideBodies1)(_ref(cpConstraint)* _this, unsigned char collideBodies) {
	(cpConstraintSetCollideBodies( _unref(_this) , collideBodies));
}
DEFINE_PRIM(_VOID, Constraint_cpConstraintSetCollideBodies1, _IDL _I8);

HL_PRIM void* HL_NAME(Constraint_cpConstraintGetUserData0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetUserData( _unref(_this) ));
}
DEFINE_PRIM(_BYTES, Constraint_cpConstraintGetUserData0, _IDL);

HL_PRIM void HL_NAME(Constraint_cpConstraintSetUserData1)(_ref(cpConstraint)* _this, void* userData) {
	(cpConstraintSetUserData( _unref(_this) , userData));
}
DEFINE_PRIM(_VOID, Constraint_cpConstraintSetUserData1, _IDL _BYTES);

HL_PRIM double HL_NAME(Constraint_cpConstraintGetImpulse0)(_ref(cpConstraint)* _this) {
	return (cpConstraintGetImpulse( _unref(_this) ));
}
DEFINE_PRIM(_F64, Constraint_cpConstraintGetImpulse0, _IDL);

HL_PRIM unsigned char HL_NAME(cpDampedRotarySpring_cpConstraintIsDampedRotarySpring1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsDampedRotarySpring(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpDampedRotarySpring_cpConstraintIsDampedRotarySpring1, _IDL);

HL_PRIM _ref(cpDampedRotarySpring)* HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringAlloc0)() {
	return alloc_ref((cpDampedRotarySpringAlloc()),cpDampedRotarySpring);
}
DEFINE_PRIM(_IDL, cpDampedRotarySpring_cpDampedRotarySpringAlloc0,);

HL_PRIM _ref(cpDampedRotarySpring)* HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringInit6)(_ref(cpDampedRotarySpring)* joint, _ref(cpBody)* a, _ref(cpBody)* b, double restAngle, double stiffness, double damping) {
	return alloc_ref((cpDampedRotarySpringInit(_unref(joint), _unref(a), _unref(b), restAngle, stiffness, damping)),cpDampedRotarySpring);
}
DEFINE_PRIM(_IDL, cpDampedRotarySpring_cpDampedRotarySpringInit6, _IDL _IDL _IDL _F64 _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringNew5)(_ref(cpBody)* a, _ref(cpBody)* b, double restAngle, double stiffness, double damping) {
	return alloc_ref((cpDampedRotarySpringNew(_unref(a), _unref(b), restAngle, stiffness, damping)),Constraint);
}
DEFINE_PRIM(_IDL, cpDampedRotarySpring_cpDampedRotarySpringNew5, _IDL _IDL _F64 _F64 _F64);

HL_PRIM double HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringGetRestAngle1)(_ref(cpConstraint)* constraint) {
	return (cpDampedRotarySpringGetRestAngle(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedRotarySpring_cpDampedRotarySpringGetRestAngle1, _IDL);

HL_PRIM void HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringSetRestAngle2)(_ref(cpConstraint)* constraint, double restAngle) {
	(cpDampedRotarySpringSetRestAngle(_unref(constraint), restAngle));
}
DEFINE_PRIM(_VOID, cpDampedRotarySpring_cpDampedRotarySpringSetRestAngle2, _IDL _F64);

HL_PRIM double HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringGetStiffness1)(_ref(cpConstraint)* constraint) {
	return (cpDampedRotarySpringGetStiffness(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedRotarySpring_cpDampedRotarySpringGetStiffness1, _IDL);

HL_PRIM void HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringSetStiffness2)(_ref(cpConstraint)* constraint, double stiffness) {
	(cpDampedRotarySpringSetStiffness(_unref(constraint), stiffness));
}
DEFINE_PRIM(_VOID, cpDampedRotarySpring_cpDampedRotarySpringSetStiffness2, _IDL _F64);

HL_PRIM double HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringGetDamping1)(_ref(cpConstraint)* constraint) {
	return (cpDampedRotarySpringGetDamping(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedRotarySpring_cpDampedRotarySpringGetDamping1, _IDL);

HL_PRIM void HL_NAME(cpDampedRotarySpring_cpDampedRotarySpringSetDamping2)(_ref(cpConstraint)* constraint, double damping) {
	(cpDampedRotarySpringSetDamping(_unref(constraint), damping));
}
DEFINE_PRIM(_VOID, cpDampedRotarySpring_cpDampedRotarySpringSetDamping2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpDampedSpring_cpConstraintIsDampedSpring1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsDampedSpring(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpDampedSpring_cpConstraintIsDampedSpring1, _IDL);

HL_PRIM _ref(cpDampedSpring)* HL_NAME(cpDampedSpring_cpDampedSpringAlloc0)() {
	return alloc_ref((cpDampedSpringAlloc()),cpDampedSpring);
}
DEFINE_PRIM(_IDL, cpDampedSpring_cpDampedSpringAlloc0,);

HL_PRIM _ref(cpDampedSpring)* HL_NAME(cpDampedSpring_cpDampedSpringInit8)(_ref(cpDampedSpring)* joint, _ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB, double restLength, double stiffness, double damping) {
	return alloc_ref((cpDampedSpringInit(_unref(joint), _unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB), restLength, stiffness, damping)),cpDampedSpring);
}
DEFINE_PRIM(_IDL, cpDampedSpring_cpDampedSpringInit8, _IDL _IDL _IDL _IDL _IDL _F64 _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpDampedSpring_cpDampedSpringNew7)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB, double restLength, double stiffness, double damping) {
	return alloc_ref((cpDampedSpringNew(_unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB), restLength, stiffness, damping)),Constraint);
}
DEFINE_PRIM(_IDL, cpDampedSpring_cpDampedSpringNew7, _IDL _IDL _IDL _IDL _F64 _F64 _F64);

HL_PRIM _ref(cpVect)* HL_NAME(cpDampedSpring_cpDampedSpringGetAnchorA1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpDampedSpringGetAnchorA(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpDampedSpring_cpDampedSpringGetAnchorA1, _IDL);

HL_PRIM void HL_NAME(cpDampedSpring_cpDampedSpringSetAnchorA2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorA) {
	(cpDampedSpringSetAnchorA(_unref(constraint), *_unref(anchorA)));
}
DEFINE_PRIM(_VOID, cpDampedSpring_cpDampedSpringSetAnchorA2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpDampedSpring_cpDampedSpringGetAnchorB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpDampedSpringGetAnchorB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpDampedSpring_cpDampedSpringGetAnchorB1, _IDL);

HL_PRIM void HL_NAME(cpDampedSpring_cpDampedSpringSetAnchorB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorB) {
	(cpDampedSpringSetAnchorB(_unref(constraint), *_unref(anchorB)));
}
DEFINE_PRIM(_VOID, cpDampedSpring_cpDampedSpringSetAnchorB2, _IDL _IDL);

HL_PRIM double HL_NAME(cpDampedSpring_cpDampedSpringGetRestLength1)(_ref(cpConstraint)* constraint) {
	return (cpDampedSpringGetRestLength(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedSpring_cpDampedSpringGetRestLength1, _IDL);

HL_PRIM void HL_NAME(cpDampedSpring_cpDampedSpringSetRestLength2)(_ref(cpConstraint)* constraint, double restLength) {
	(cpDampedSpringSetRestLength(_unref(constraint), restLength));
}
DEFINE_PRIM(_VOID, cpDampedSpring_cpDampedSpringSetRestLength2, _IDL _F64);

HL_PRIM double HL_NAME(cpDampedSpring_cpDampedSpringGetStiffness1)(_ref(cpConstraint)* constraint) {
	return (cpDampedSpringGetStiffness(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedSpring_cpDampedSpringGetStiffness1, _IDL);

HL_PRIM void HL_NAME(cpDampedSpring_cpDampedSpringSetStiffness2)(_ref(cpConstraint)* constraint, double stiffness) {
	(cpDampedSpringSetStiffness(_unref(constraint), stiffness));
}
DEFINE_PRIM(_VOID, cpDampedSpring_cpDampedSpringSetStiffness2, _IDL _F64);

HL_PRIM double HL_NAME(cpDampedSpring_cpDampedSpringGetDamping1)(_ref(cpConstraint)* constraint) {
	return (cpDampedSpringGetDamping(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpDampedSpring_cpDampedSpringGetDamping1, _IDL);

HL_PRIM void HL_NAME(cpDampedSpring_cpDampedSpringSetDamping2)(_ref(cpConstraint)* constraint, double damping) {
	(cpDampedSpringSetDamping(_unref(constraint), damping));
}
DEFINE_PRIM(_VOID, cpDampedSpring_cpDampedSpringSetDamping2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpGearJoint_cpConstraintIsGearJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsGearJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpGearJoint_cpConstraintIsGearJoint1, _IDL);

HL_PRIM _ref(cpGearJoint)* HL_NAME(cpGearJoint_cpGearJointAlloc0)() {
	return alloc_ref((cpGearJointAlloc()),cpGearJoint);
}
DEFINE_PRIM(_IDL, cpGearJoint_cpGearJointAlloc0,);

HL_PRIM _ref(cpGearJoint)* HL_NAME(cpGearJoint_cpGearJointInit5)(_ref(cpGearJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, double phase, double ratio) {
	return alloc_ref((cpGearJointInit(_unref(joint), _unref(a), _unref(b), phase, ratio)),cpGearJoint);
}
DEFINE_PRIM(_IDL, cpGearJoint_cpGearJointInit5, _IDL _IDL _IDL _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpGearJoint_cpGearJointNew4)(_ref(cpBody)* a, _ref(cpBody)* b, double phase, double ratio) {
	return alloc_ref((cpGearJointNew(_unref(a), _unref(b), phase, ratio)),Constraint);
}
DEFINE_PRIM(_IDL, cpGearJoint_cpGearJointNew4, _IDL _IDL _F64 _F64);

HL_PRIM double HL_NAME(cpGearJoint_cpGearJointGetPhase1)(_ref(cpConstraint)* constraint) {
	return (cpGearJointGetPhase(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpGearJoint_cpGearJointGetPhase1, _IDL);

HL_PRIM void HL_NAME(cpGearJoint_cpGearJointSetPhase2)(_ref(cpConstraint)* constraint, double phase) {
	(cpGearJointSetPhase(_unref(constraint), phase));
}
DEFINE_PRIM(_VOID, cpGearJoint_cpGearJointSetPhase2, _IDL _F64);

HL_PRIM double HL_NAME(cpGearJoint_cpGearJointGetRatio1)(_ref(cpConstraint)* constraint) {
	return (cpGearJointGetRatio(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpGearJoint_cpGearJointGetRatio1, _IDL);

HL_PRIM void HL_NAME(cpGearJoint_cpGearJointSetRatio2)(_ref(cpConstraint)* constraint, double ratio) {
	(cpGearJointSetRatio(_unref(constraint), ratio));
}
DEFINE_PRIM(_VOID, cpGearJoint_cpGearJointSetRatio2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpSimpleMotor_cpConstraintIsSimpleMotor1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsSimpleMotor(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpSimpleMotor_cpConstraintIsSimpleMotor1, _IDL);

HL_PRIM _ref(cpSimpleMotor)* HL_NAME(cpSimpleMotor_cpSimpleMotorAlloc0)() {
	return alloc_ref((cpSimpleMotorAlloc()),cpSimpleMotor);
}
DEFINE_PRIM(_IDL, cpSimpleMotor_cpSimpleMotorAlloc0,);

HL_PRIM _ref(cpSimpleMotor)* HL_NAME(cpSimpleMotor_cpSimpleMotorInit4)(_ref(cpSimpleMotor)* joint, _ref(cpBody)* a, _ref(cpBody)* b, double rate) {
	return alloc_ref((cpSimpleMotorInit(_unref(joint), _unref(a), _unref(b), rate)),cpSimpleMotor);
}
DEFINE_PRIM(_IDL, cpSimpleMotor_cpSimpleMotorInit4, _IDL _IDL _IDL _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpSimpleMotor_cpSimpleMotorNew3)(_ref(cpBody)* a, _ref(cpBody)* b, double rate) {
	return alloc_ref((cpSimpleMotorNew(_unref(a), _unref(b), rate)),Constraint);
}
DEFINE_PRIM(_IDL, cpSimpleMotor_cpSimpleMotorNew3, _IDL _IDL _F64);

HL_PRIM double HL_NAME(cpSimpleMotor_cpSimpleMotorGetRate1)(_ref(cpConstraint)* constraint) {
	return (cpSimpleMotorGetRate(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpSimpleMotor_cpSimpleMotorGetRate1, _IDL);

HL_PRIM void HL_NAME(cpSimpleMotor_cpSimpleMotorSetRate2)(_ref(cpConstraint)* constraint, double rate) {
	(cpSimpleMotorSetRate(_unref(constraint), rate));
}
DEFINE_PRIM(_VOID, cpSimpleMotor_cpSimpleMotorSetRate2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpGrooveJoint_cpConstraintIsGrooveJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsGrooveJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpGrooveJoint_cpConstraintIsGrooveJoint1, _IDL);

HL_PRIM _ref(cpGrooveJoint)* HL_NAME(cpGrooveJoint_cpGrooveJointAlloc0)() {
	return alloc_ref((cpGrooveJointAlloc()),cpGrooveJoint);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointAlloc0,);

HL_PRIM _ref(cpGrooveJoint)* HL_NAME(cpGrooveJoint_cpGrooveJointInit6)(_ref(cpGrooveJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* groove_a, _ref(cpVect)* groove_b, _ref(cpVect)* anchorB) {
	return alloc_ref((cpGrooveJointInit(_unref(joint), _unref(a), _unref(b), *_unref(groove_a), *_unref(groove_b), *_unref(anchorB))),cpGrooveJoint);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointInit6, _IDL _IDL _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpGrooveJoint_cpGrooveJointNew5)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* groove_a, _ref(cpVect)* groove_b, _ref(cpVect)* anchorB) {
	return alloc_ref((cpGrooveJointNew(_unref(a), _unref(b), *_unref(groove_a), *_unref(groove_b), *_unref(anchorB))),Constraint);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointNew5, _IDL _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpGrooveJoint_cpGrooveJointGetGrooveA1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpGrooveJointGetGrooveA(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointGetGrooveA1, _IDL);

HL_PRIM void HL_NAME(cpGrooveJoint_cpGrooveJointSetGrooveA2)(_ref(cpConstraint)* constraint, _ref(cpVect)* grooveA) {
	(cpGrooveJointSetGrooveA(_unref(constraint), *_unref(grooveA)));
}
DEFINE_PRIM(_VOID, cpGrooveJoint_cpGrooveJointSetGrooveA2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpGrooveJoint_cpGrooveJointGetGrooveB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpGrooveJointGetGrooveB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointGetGrooveB1, _IDL);

HL_PRIM void HL_NAME(cpGrooveJoint_cpGrooveJointSetGrooveB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* grooveB) {
	(cpGrooveJointSetGrooveB(_unref(constraint), *_unref(grooveB)));
}
DEFINE_PRIM(_VOID, cpGrooveJoint_cpGrooveJointSetGrooveB2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpGrooveJoint_cpGrooveJointGetAnchorB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpGrooveJointGetAnchorB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpGrooveJoint_cpGrooveJointGetAnchorB1, _IDL);

HL_PRIM void HL_NAME(cpGrooveJoint_cpGrooveJointSetAnchorB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorB) {
	(cpGrooveJointSetAnchorB(_unref(constraint), *_unref(anchorB)));
}
DEFINE_PRIM(_VOID, cpGrooveJoint_cpGrooveJointSetAnchorB2, _IDL _IDL);

HL_PRIM _ref(cpSpace)* HL_NAME(cpHastySpace_cpHastySpaceNew0)() {
	return alloc_ref((cpHastySpaceNew()),Space);
}
DEFINE_PRIM(_IDL, cpHastySpace_cpHastySpaceNew0,);

HL_PRIM void HL_NAME(cpHastySpace_cpHastySpaceFree1)(_ref(cpSpace)* space) {
	(cpHastySpaceFree(_unref(space)));
}
DEFINE_PRIM(_VOID, cpHastySpace_cpHastySpaceFree1, _IDL);

HL_PRIM void HL_NAME(cpHastySpace_cpHastySpaceSetThreads2)(_ref(cpSpace)* space, unsigned int threads) {
	(cpHastySpaceSetThreads(_unref(space), threads));
}
DEFINE_PRIM(_VOID, cpHastySpace_cpHastySpaceSetThreads2, _IDL _I32);

HL_PRIM unsigned int HL_NAME(cpHastySpace_cpHastySpaceGetThreads1)(_ref(cpSpace)* space) {
	return (cpHastySpaceGetThreads(_unref(space)));
}
DEFINE_PRIM(_I32, cpHastySpace_cpHastySpaceGetThreads1, _IDL);

HL_PRIM void HL_NAME(cpHastySpace_cpHastySpaceStep2)(_ref(cpSpace)* space, double dt) {
	(cpHastySpaceStep(_unref(space), dt));
}
DEFINE_PRIM(_VOID, cpHastySpace_cpHastySpaceStep2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpPinJoint_cpConstraintIsPinJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsPinJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpPinJoint_cpConstraintIsPinJoint1, _IDL);

HL_PRIM _ref(cpPinJoint)* HL_NAME(cpPinJoint_cpPinJointAlloc0)() {
	return alloc_ref((cpPinJointAlloc()),cpPinJoint);
}
DEFINE_PRIM(_IDL, cpPinJoint_cpPinJointAlloc0,);

HL_PRIM _ref(cpPinJoint)* HL_NAME(cpPinJoint_cpPinJointInit5)(_ref(cpPinJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB) {
	return alloc_ref((cpPinJointInit(_unref(joint), _unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB))),cpPinJoint);
}
DEFINE_PRIM(_IDL, cpPinJoint_cpPinJointInit5, _IDL _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpPinJoint_cpPinJointNew4)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB) {
	return alloc_ref((cpPinJointNew(_unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB))),Constraint);
}
DEFINE_PRIM(_IDL, cpPinJoint_cpPinJointNew4, _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPinJoint_cpPinJointGetAnchorA1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpPinJointGetAnchorA(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpPinJoint_cpPinJointGetAnchorA1, _IDL);

HL_PRIM void HL_NAME(cpPinJoint_cpPinJointSetAnchorA2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorA) {
	(cpPinJointSetAnchorA(_unref(constraint), *_unref(anchorA)));
}
DEFINE_PRIM(_VOID, cpPinJoint_cpPinJointSetAnchorA2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPinJoint_cpPinJointGetAnchorB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpPinJointGetAnchorB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpPinJoint_cpPinJointGetAnchorB1, _IDL);

HL_PRIM void HL_NAME(cpPinJoint_cpPinJointSetAnchorB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorB) {
	(cpPinJointSetAnchorB(_unref(constraint), *_unref(anchorB)));
}
DEFINE_PRIM(_VOID, cpPinJoint_cpPinJointSetAnchorB2, _IDL _IDL);

HL_PRIM double HL_NAME(cpPinJoint_cpPinJointGetDist1)(_ref(cpConstraint)* constraint) {
	return (cpPinJointGetDist(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpPinJoint_cpPinJointGetDist1, _IDL);

HL_PRIM void HL_NAME(cpPinJoint_cpPinJointSetDist2)(_ref(cpConstraint)* constraint, double dist) {
	(cpPinJointSetDist(_unref(constraint), dist));
}
DEFINE_PRIM(_VOID, cpPinJoint_cpPinJointSetDist2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpPivotJoint_cpConstraintIsPivotJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsPivotJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpPivotJoint_cpConstraintIsPivotJoint1, _IDL);

HL_PRIM _ref(cpPivotJoint)* HL_NAME(cpPivotJoint_cpPivotJointAlloc0)() {
	return alloc_ref((cpPivotJointAlloc()),cpPivotJoint);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointAlloc0,);

HL_PRIM _ref(cpPivotJoint)* HL_NAME(cpPivotJoint_cpPivotJointInit5)(_ref(cpPivotJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB) {
	return alloc_ref((cpPivotJointInit(_unref(joint), _unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB))),cpPivotJoint);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointInit5, _IDL _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpPivotJoint_cpPivotJointNew3)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* pivot) {
	return alloc_ref((cpPivotJointNew(_unref(a), _unref(b), *_unref(pivot))),Constraint);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointNew3, _IDL _IDL _IDL);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpPivotJoint_cpPivotJointNew24)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB) {
	return alloc_ref((cpPivotJointNew2(_unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB))),Constraint);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointNew24, _IDL _IDL _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPivotJoint_cpPivotJointGetAnchorA1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpPivotJointGetAnchorA(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointGetAnchorA1, _IDL);

HL_PRIM void HL_NAME(cpPivotJoint_cpPivotJointSetAnchorA2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorA) {
	(cpPivotJointSetAnchorA(_unref(constraint), *_unref(anchorA)));
}
DEFINE_PRIM(_VOID, cpPivotJoint_cpPivotJointSetAnchorA2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPivotJoint_cpPivotJointGetAnchorB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpPivotJointGetAnchorB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, cpPivotJoint_cpPivotJointGetAnchorB1, _IDL);

HL_PRIM void HL_NAME(cpPivotJoint_cpPivotJointSetAnchorB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorB) {
	(cpPivotJointSetAnchorB(_unref(constraint), *_unref(anchorB)));
}
DEFINE_PRIM(_VOID, cpPivotJoint_cpPivotJointSetAnchorB2, _IDL _IDL);

HL_PRIM void HL_NAME(cpPolyline_cpPolylineFree1)(_ref(cpPolyline)* line) {
	(cpPolylineFree(_unref(line)));
}
DEFINE_PRIM(_VOID, cpPolyline_cpPolylineFree1, _IDL);

HL_PRIM unsigned char HL_NAME(cpPolyline_cpPolylineIsClosed1)(_ref(cpPolyline)* line) {
	return (cpPolylineIsClosed(_unref(line)));
}
DEFINE_PRIM(_I8, cpPolyline_cpPolylineIsClosed1, _IDL);

HL_PRIM _ref(cpPolyline)* HL_NAME(cpPolyline_cpPolylineSimplifyCurves2)(_ref(cpPolyline)* line, double tol) {
	return alloc_ref((cpPolylineSimplifyCurves(_unref(line), tol)),cpPolyline);
}
DEFINE_PRIM(_IDL, cpPolyline_cpPolylineSimplifyCurves2, _IDL _F64);

HL_PRIM _ref(cpPolyline)* HL_NAME(cpPolyline_cpPolylineSimplifyVertexes2)(_ref(cpPolyline)* line, double tol) {
	return alloc_ref((cpPolylineSimplifyVertexes(_unref(line), tol)),cpPolyline);
}
DEFINE_PRIM(_IDL, cpPolyline_cpPolylineSimplifyVertexes2, _IDL _F64);

HL_PRIM _ref(cpPolyline)* HL_NAME(cpPolyline_cpPolylineToConvexHull2)(_ref(cpPolyline)* line, double tol) {
	return alloc_ref((cpPolylineToConvexHull(_unref(line), tol)),cpPolyline);
}
DEFINE_PRIM(_IDL, cpPolyline_cpPolylineToConvexHull2, _IDL _F64);

HL_PRIM _ref(cpPolylineSet)* HL_NAME(cpPolylineSet_cpPolylineSetAlloc0)() {
	return alloc_ref((cpPolylineSetAlloc()),cpPolylineSet);
}
DEFINE_PRIM(_IDL, cpPolylineSet_cpPolylineSetAlloc0,);

HL_PRIM _ref(cpPolylineSet)* HL_NAME(cpPolylineSet_cpPolylineSetInit1)(_ref(cpPolylineSet)* set) {
	return alloc_ref((cpPolylineSetInit(_unref(set))),cpPolylineSet);
}
DEFINE_PRIM(_IDL, cpPolylineSet_cpPolylineSetInit1, _IDL);

HL_PRIM _ref(cpPolylineSet)* HL_NAME(cpPolylineSet_cpPolylineSetNew0)() {
	return alloc_ref((cpPolylineSetNew()),cpPolylineSet);
}
DEFINE_PRIM(_IDL, cpPolylineSet_cpPolylineSetNew0,);

HL_PRIM void HL_NAME(cpPolylineSet_cpPolylineSetDestroy2)(_ref(cpPolylineSet)* set, unsigned char freePolylines) {
	(cpPolylineSetDestroy(_unref(set), freePolylines));
}
DEFINE_PRIM(_VOID, cpPolylineSet_cpPolylineSetDestroy2, _IDL _I8);

HL_PRIM void HL_NAME(cpPolylineSet_cpPolylineSetFree2)(_ref(cpPolylineSet)* set, unsigned char freePolylines) {
	(cpPolylineSetFree(_unref(set), freePolylines));
}
DEFINE_PRIM(_VOID, cpPolylineSet_cpPolylineSetFree2, _IDL _I8);

HL_PRIM void HL_NAME(cpPolylineSet_cpPolylineSetCollectSegment3)(_ref(cpVect)* v0, _ref(cpVect)* v1, _ref(cpPolylineSet)* lines) {
	(cpPolylineSetCollectSegment(*_unref(v0), *_unref(v1), _unref(lines)));
}
DEFINE_PRIM(_VOID, cpPolylineSet_cpPolylineSetCollectSegment3, _IDL _IDL _IDL);

HL_PRIM _ref(cpPolylineSet)* HL_NAME(cpPolylineSet_cpPolylineConvexDecomposition2)(_ref(cpPolyline)* line, double tol) {
	return alloc_ref((cpPolylineConvexDecomposition(_unref(line), tol)),cpPolylineSet);
}
DEFINE_PRIM(_IDL, cpPolylineSet_cpPolylineConvexDecomposition2, _IDL _F64);

HL_PRIM _ref(cpPolyShape)* HL_NAME(cpPolyShape_cpPolyShapeAlloc0)() {
	return alloc_ref((cpPolyShapeAlloc()),cpPolyShape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeAlloc0,);

HL_PRIM _ref(cpPolyShape)* HL_NAME(cpPolyShape_cpPolyShapeInit6)(_ref(cpPolyShape)* poly, _ref(cpBody)* body, int count, varray* verts, _ref(cpTransform)* transform, double radius) {
	return alloc_ref((cpPolyShapeInit(_unref(poly), _unref(body), count, (cpVect*)hl_aptr(verts,HL_CONST double), *_unref(transform), radius)),cpPolyShape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeInit6, _IDL _IDL _I32 _ARR _IDL _F64);

HL_PRIM _ref(cpPolyShape)* HL_NAME(cpPolyShape_cpPolyShapeInitRaw5)(_ref(cpPolyShape)* poly, _ref(cpBody)* body, int count, varray* verts, double radius) {
	return alloc_ref((cpPolyShapeInitRaw(_unref(poly), _unref(body), count, (cpVect*)hl_aptr(verts,HL_CONST double), radius)),cpPolyShape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeInitRaw5, _IDL _IDL _I32 _ARR _F64);

HL_PRIM _ref(cpShape)* HL_NAME(cpPolyShape_cpPolyShapeNew5)(_ref(cpBody)* body, int count, varray* verts, _ref(cpTransform)* transform, double radius) {
	return alloc_ref((cpPolyShapeNew(_unref(body), count, (cpVect*)hl_aptr(verts,HL_CONST double), *_unref(transform), radius)),Shape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeNew5, _IDL _I32 _ARR _IDL _F64);

HL_PRIM _ref(cpShape)* HL_NAME(cpPolyShape_cpPolyShapeNewRaw4)(_ref(cpBody)* body, int count, varray* verts, double radius) {
	return alloc_ref((cpPolyShapeNewRaw(_unref(body), count, (cpVect*)hl_aptr(verts,HL_CONST double), radius)),Shape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeNewRaw4, _IDL _I32 _ARR _F64);

HL_PRIM _ref(cpPolyShape)* HL_NAME(cpPolyShape_cpBoxShapeInit5)(_ref(cpPolyShape)* poly, _ref(cpBody)* body, double width, double height, double radius) {
	return alloc_ref((cpBoxShapeInit(_unref(poly), _unref(body), width, height, radius)),cpPolyShape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpBoxShapeInit5, _IDL _IDL _F64 _F64 _F64);

HL_PRIM _ref(cpPolyShape)* HL_NAME(cpPolyShape_cpBoxShapeInit24)(_ref(cpPolyShape)* poly, _ref(cpBody)* body, _ref(cpBB)* box, double radius) {
	return alloc_ref((cpBoxShapeInit2(_unref(poly), _unref(body), *_unref(box), radius)),cpPolyShape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpBoxShapeInit24, _IDL _IDL _IDL _F64);

HL_PRIM _ref(cpShape)* HL_NAME(cpPolyShape_cpBoxShapeNew4)(_ref(cpBody)* body, double width, double height, double radius) {
	return alloc_ref((cpBoxShapeNew(_unref(body), width, height, radius)),Shape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpBoxShapeNew4, _IDL _F64 _F64 _F64);

HL_PRIM _ref(cpShape)* HL_NAME(cpPolyShape_cpBoxShapeNew23)(_ref(cpBody)* body, _ref(cpBB)* box, double radius) {
	return alloc_ref((cpBoxShapeNew2(_unref(body), *_unref(box), radius)),Shape);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpBoxShapeNew23, _IDL _IDL _F64);

HL_PRIM int HL_NAME(cpPolyShape_cpPolyShapeGetCount1)(_ref(cpShape)* shape) {
	return (cpPolyShapeGetCount(_unref(shape)));
}
DEFINE_PRIM(_I32, cpPolyShape_cpPolyShapeGetCount1, _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(cpPolyShape_cpPolyShapeGetVert2)(_ref(cpShape)* shape, int index) {
	return alloc_ref(new cpVect((cpPolyShapeGetVert(_unref(shape), index))),Vect);
}
DEFINE_PRIM(_IDL, cpPolyShape_cpPolyShapeGetVert2, _IDL _I32);

HL_PRIM double HL_NAME(cpPolyShape_cpPolyShapeGetRadius1)(_ref(cpShape)* shape) {
	return (cpPolyShapeGetRadius(_unref(shape)));
}
DEFINE_PRIM(_F64, cpPolyShape_cpPolyShapeGetRadius1, _IDL);

HL_PRIM unsigned char HL_NAME(cpRatchetJoint_cpConstraintIsRatchetJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsRatchetJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpRatchetJoint_cpConstraintIsRatchetJoint1, _IDL);

HL_PRIM _ref(cpRatchetJoint)* HL_NAME(cpRatchetJoint_cpRatchetJointAlloc0)() {
	return alloc_ref((cpRatchetJointAlloc()),cpRatchetJoint);
}
DEFINE_PRIM(_IDL, cpRatchetJoint_cpRatchetJointAlloc0,);

HL_PRIM _ref(cpRatchetJoint)* HL_NAME(cpRatchetJoint_cpRatchetJointInit5)(_ref(cpRatchetJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, double phase, double ratchet) {
	return alloc_ref((cpRatchetJointInit(_unref(joint), _unref(a), _unref(b), phase, ratchet)),cpRatchetJoint);
}
DEFINE_PRIM(_IDL, cpRatchetJoint_cpRatchetJointInit5, _IDL _IDL _IDL _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpRatchetJoint_cpRatchetJointNew4)(_ref(cpBody)* a, _ref(cpBody)* b, double phase, double ratchet) {
	return alloc_ref((cpRatchetJointNew(_unref(a), _unref(b), phase, ratchet)),Constraint);
}
DEFINE_PRIM(_IDL, cpRatchetJoint_cpRatchetJointNew4, _IDL _IDL _F64 _F64);

HL_PRIM double HL_NAME(cpRatchetJoint_cpRatchetJointGetAngle1)(_ref(cpConstraint)* constraint) {
	return (cpRatchetJointGetAngle(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpRatchetJoint_cpRatchetJointGetAngle1, _IDL);

HL_PRIM void HL_NAME(cpRatchetJoint_cpRatchetJointSetAngle2)(_ref(cpConstraint)* constraint, double angle) {
	(cpRatchetJointSetAngle(_unref(constraint), angle));
}
DEFINE_PRIM(_VOID, cpRatchetJoint_cpRatchetJointSetAngle2, _IDL _F64);

HL_PRIM double HL_NAME(cpRatchetJoint_cpRatchetJointGetPhase1)(_ref(cpConstraint)* constraint) {
	return (cpRatchetJointGetPhase(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpRatchetJoint_cpRatchetJointGetPhase1, _IDL);

HL_PRIM void HL_NAME(cpRatchetJoint_cpRatchetJointSetPhase2)(_ref(cpConstraint)* constraint, double phase) {
	(cpRatchetJointSetPhase(_unref(constraint), phase));
}
DEFINE_PRIM(_VOID, cpRatchetJoint_cpRatchetJointSetPhase2, _IDL _F64);

HL_PRIM double HL_NAME(cpRatchetJoint_cpRatchetJointGetRatchet1)(_ref(cpConstraint)* constraint) {
	return (cpRatchetJointGetRatchet(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpRatchetJoint_cpRatchetJointGetRatchet1, _IDL);

HL_PRIM void HL_NAME(cpRatchetJoint_cpRatchetJointSetRatchet2)(_ref(cpConstraint)* constraint, double ratchet) {
	(cpRatchetJointSetRatchet(_unref(constraint), ratchet));
}
DEFINE_PRIM(_VOID, cpRatchetJoint_cpRatchetJointSetRatchet2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(cpRotaryLimitJoint_cpConstraintIsRotaryLimitJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsRotaryLimitJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, cpRotaryLimitJoint_cpConstraintIsRotaryLimitJoint1, _IDL);

HL_PRIM _ref(cpRotaryLimitJoint)* HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointAlloc0)() {
	return alloc_ref((cpRotaryLimitJointAlloc()),cpRotaryLimitJoint);
}
DEFINE_PRIM(_IDL, cpRotaryLimitJoint_cpRotaryLimitJointAlloc0,);

HL_PRIM _ref(cpRotaryLimitJoint)* HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointInit5)(_ref(cpRotaryLimitJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, double min, double max) {
	return alloc_ref((cpRotaryLimitJointInit(_unref(joint), _unref(a), _unref(b), min, max)),cpRotaryLimitJoint);
}
DEFINE_PRIM(_IDL, cpRotaryLimitJoint_cpRotaryLimitJointInit5, _IDL _IDL _IDL _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointNew4)(_ref(cpBody)* a, _ref(cpBody)* b, double min, double max) {
	return alloc_ref((cpRotaryLimitJointNew(_unref(a), _unref(b), min, max)),Constraint);
}
DEFINE_PRIM(_IDL, cpRotaryLimitJoint_cpRotaryLimitJointNew4, _IDL _IDL _F64 _F64);

HL_PRIM double HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointGetMin1)(_ref(cpConstraint)* constraint) {
	return (cpRotaryLimitJointGetMin(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpRotaryLimitJoint_cpRotaryLimitJointGetMin1, _IDL);

HL_PRIM void HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointSetMin2)(_ref(cpConstraint)* constraint, double min) {
	(cpRotaryLimitJointSetMin(_unref(constraint), min));
}
DEFINE_PRIM(_VOID, cpRotaryLimitJoint_cpRotaryLimitJointSetMin2, _IDL _F64);

HL_PRIM double HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointGetMax1)(_ref(cpConstraint)* constraint) {
	return (cpRotaryLimitJointGetMax(_unref(constraint)));
}
DEFINE_PRIM(_F64, cpRotaryLimitJoint_cpRotaryLimitJointGetMax1, _IDL);

HL_PRIM void HL_NAME(cpRotaryLimitJoint_cpRotaryLimitJointSetMax2)(_ref(cpConstraint)* constraint, double max) {
	(cpRotaryLimitJointSetMax(_unref(constraint), max));
}
DEFINE_PRIM(_VOID, cpRotaryLimitJoint_cpRotaryLimitJointSetMax2, _IDL _F64);

HL_PRIM unsigned char HL_NAME(SlideJoint_cpConstraintIsSlideJoint1)(_ref(cpConstraint)* constraint) {
	return (cpConstraintIsSlideJoint(_unref(constraint)));
}
DEFINE_PRIM(_I8, SlideJoint_cpConstraintIsSlideJoint1, _IDL);

HL_PRIM _ref(cpSlideJoint)* HL_NAME(SlideJoint_cpSlideJointAlloc0)() {
	return alloc_ref((cpSlideJointAlloc()),SlideJoint);
}
DEFINE_PRIM(_IDL, SlideJoint_cpSlideJointAlloc0,);

HL_PRIM _ref(cpSlideJoint)* HL_NAME(SlideJoint_cpSlideJointInit7)(_ref(cpSlideJoint)* joint, _ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB, double min, double max) {
	return alloc_ref((cpSlideJointInit(_unref(joint), _unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB), min, max)),SlideJoint);
}
DEFINE_PRIM(_IDL, SlideJoint_cpSlideJointInit7, _IDL _IDL _IDL _IDL _IDL _F64 _F64);

HL_PRIM _ref(cpConstraint)* HL_NAME(SlideJoint_cpSlideJointNew6)(_ref(cpBody)* a, _ref(cpBody)* b, _ref(cpVect)* anchorA, _ref(cpVect)* anchorB, double min, double max) {
	return alloc_ref((cpSlideJointNew(_unref(a), _unref(b), *_unref(anchorA), *_unref(anchorB), min, max)),Constraint);
}
DEFINE_PRIM(_IDL, SlideJoint_cpSlideJointNew6, _IDL _IDL _IDL _IDL _F64 _F64);

HL_PRIM _ref(cpVect)* HL_NAME(SlideJoint_cpSlideJointGetAnchorA1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpSlideJointGetAnchorA(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, SlideJoint_cpSlideJointGetAnchorA1, _IDL);

HL_PRIM void HL_NAME(SlideJoint_cpSlideJointSetAnchorA2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorA) {
	(cpSlideJointSetAnchorA(_unref(constraint), *_unref(anchorA)));
}
DEFINE_PRIM(_VOID, SlideJoint_cpSlideJointSetAnchorA2, _IDL _IDL);

HL_PRIM _ref(cpVect)* HL_NAME(SlideJoint_cpSlideJointGetAnchorB1)(_ref(cpConstraint)* constraint) {
	return alloc_ref(new cpVect((cpSlideJointGetAnchorB(_unref(constraint)))),Vect);
}
DEFINE_PRIM(_IDL, SlideJoint_cpSlideJointGetAnchorB1, _IDL);

HL_PRIM void HL_NAME(SlideJoint_cpSlideJointSetAnchorB2)(_ref(cpConstraint)* constraint, _ref(cpVect)* anchorB) {
	(cpSlideJointSetAnchorB(_unref(constraint), *_unref(anchorB)));
}
DEFINE_PRIM(_VOID, SlideJoint_cpSlideJointSetAnchorB2, _IDL _IDL);

HL_PRIM double HL_NAME(SlideJoint_cpSlideJointGetMin1)(_ref(cpConstraint)* constraint) {
	return (cpSlideJointGetMin(_unref(constraint)));
}
DEFINE_PRIM(_F64, SlideJoint_cpSlideJointGetMin1, _IDL);

HL_PRIM void HL_NAME(SlideJoint_cpSlideJointSetMin2)(_ref(cpConstraint)* constraint, double min) {
	(cpSlideJointSetMin(_unref(constraint), min));
}
DEFINE_PRIM(_VOID, SlideJoint_cpSlideJointSetMin2, _IDL _F64);

HL_PRIM double HL_NAME(SlideJoint_cpSlideJointGetMax1)(_ref(cpConstraint)* constraint) {
	return (cpSlideJointGetMax(_unref(constraint)));
}
DEFINE_PRIM(_F64, SlideJoint_cpSlideJointGetMax1, _IDL);

HL_PRIM void HL_NAME(SlideJoint_cpSlideJointSetMax2)(_ref(cpConstraint)* constraint, double max) {
	(cpSlideJointSetMax(_unref(constraint), max));
}
DEFINE_PRIM(_VOID, SlideJoint_cpSlideJointSetMax2, _IDL _F64);

HL_PRIM _ref(cpSpaceHash)* HL_NAME(SpatialIndex_cpSpaceHashAlloc0)() {
	return alloc_ref((cpSpaceHashAlloc()),cpSpaceHash);
}
DEFINE_PRIM(_IDL, SpatialIndex_cpSpaceHashAlloc0,);

HL_PRIM void HL_NAME(SpatialIndex_cpSpaceHashResize3)(_ref(cpSpaceHash)* hash, double celldim, int numcells) {
	(cpSpaceHashResize(_unref(hash), celldim, numcells));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpaceHashResize3, _IDL _F64 _I32);

HL_PRIM _ref(cpBBTree)* HL_NAME(SpatialIndex_cpBBTreeAlloc0)() {
	return alloc_ref((cpBBTreeAlloc()),cpBBTree);
}
DEFINE_PRIM(_IDL, SpatialIndex_cpBBTreeAlloc0,);

HL_PRIM void HL_NAME(SpatialIndex_cpBBTreeOptimize0)(_ref(cpSpatialIndex)* _this) {
	(cpBBTreeOptimize( _unref(_this) ));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpBBTreeOptimize0, _IDL);

HL_PRIM void HL_NAME(SpatialIndex_free0)(_ref(cpSpatialIndex)* _this) {
	(cpSpatialIndexFree( _unref(_this) ));
}
DEFINE_PRIM(_VOID, SpatialIndex_free0, _IDL);

HL_PRIM void HL_NAME(SpatialIndex_cpSpatialIndexDestroy0)(_ref(cpSpatialIndex)* _this) {
	(cpSpatialIndexDestroy( _unref(_this) ));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpatialIndexDestroy0, _IDL);

HL_PRIM int HL_NAME(SpatialIndex_cpSpatialIndexCount0)(_ref(cpSpatialIndex)* _this) {
	return (cpSpatialIndexCount( _unref(_this) ));
}
DEFINE_PRIM(_I32, SpatialIndex_cpSpatialIndexCount0, _IDL);

HL_PRIM unsigned char HL_NAME(SpatialIndex_cpSpatialIndexContains2)(_ref(cpSpatialIndex)* _this, void* obj, unsigned int hashid) {
	return (cpSpatialIndexContains( _unref(_this) , obj, hashid));
}
DEFINE_PRIM(_I8, SpatialIndex_cpSpatialIndexContains2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(SpatialIndex_cpSpatialIndexInsert2)(_ref(cpSpatialIndex)* _this, void* obj, unsigned int hashid) {
	(cpSpatialIndexInsert( _unref(_this) , obj, hashid));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpatialIndexInsert2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(SpatialIndex_cpSpatialIndexRemove2)(_ref(cpSpatialIndex)* _this, void* obj, unsigned int hashid) {
	(cpSpatialIndexRemove( _unref(_this) , obj, hashid));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpatialIndexRemove2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(SpatialIndex_cpSpatialIndexReindex0)(_ref(cpSpatialIndex)* _this) {
	(cpSpatialIndexReindex( _unref(_this) ));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpatialIndexReindex0, _IDL);

HL_PRIM void HL_NAME(SpatialIndex_cpSpatialIndexReindexObject2)(_ref(cpSpatialIndex)* _this, void* obj, unsigned int hashid) {
	(cpSpatialIndexReindexObject( _unref(_this) , obj, hashid));
}
DEFINE_PRIM(_VOID, SpatialIndex_cpSpatialIndexReindexObject2, _IDL _BYTES _I32);

HL_PRIM double HL_NAME(Math_momentForCircle4)(double m, double r1, double r2, _ref(cpVect)* offset) {
	return (cpMomentForCircle(m, r1, r2, *_unref(offset)));
}
DEFINE_PRIM(_F64, Math_momentForCircle4, _F64 _F64 _F64 _IDL);

HL_PRIM double HL_NAME(Math_areaForCircle2)(double r1, double r2) {
	return (cpAreaForCircle(r1, r2));
}
DEFINE_PRIM(_F64, Math_areaForCircle2, _F64 _F64);

HL_PRIM double HL_NAME(Math_momentForSegment4)(double m, _ref(cpVect)* a, _ref(cpVect)* b, double radius) {
	return (cpMomentForSegment(m, *(cpVect*)_unref(a), *_unref(b), radius));
}
DEFINE_PRIM(_F64, Math_momentForSegment4, _F64 _IDL _IDL _F64);

HL_PRIM double HL_NAME(Math_momentForPoly5)(double m, int count, varray* verts, _ref(cpVect)* offset, double radius) {
	return (cpMomentForPoly(m, count, (cpVect*)hl_aptr(verts,HL_CONST double), *_unref(offset), radius));
}
DEFINE_PRIM(_F64, Math_momentForPoly5, _F64 _I32 _ARR _IDL _F64);

HL_PRIM double HL_NAME(Math_areaForSegment3)(_ref(cpVect)* a, _ref(cpVect)* b, double radius) {
	return (cpAreaForSegment(*_unref(a), *_unref(b), radius));
}
DEFINE_PRIM(_F64, Math_areaForSegment3, _IDL _IDL _F64);

HL_PRIM double HL_NAME(Math_areaForPoly3)(int count, varray* verts, double radius) {
	return (cpAreaForPoly(count, (cpVect*)hl_aptr(verts,HL_CONST double), radius));
}
DEFINE_PRIM(_F64, Math_areaForPoly3, _I32 _ARR _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Math_centroidForPoly2)(int count, varray* verts) {
	return alloc_ref(new cpVect((cpCentroidForPoly(count, (cpVect*)hl_aptr(verts,double)))),Vect);
}
DEFINE_PRIM(_IDL, Math_centroidForPoly2, _I32 _ARR);

HL_PRIM double HL_NAME(Math_momentForBox3)(double m, double width, double height) {
	return (cpMomentForBox(m, width, height));
}
DEFINE_PRIM(_F64, Math_momentForBox3, _F64 _F64 _F64);

HL_PRIM double HL_NAME(Math_cpMomentForBox22)(double m, _ref(cpBB)* box) {
	return (cpMomentForBox2(m, *_unref(box)));
}
DEFINE_PRIM(_F64, Math_cpMomentForBox22, _F64 _IDL);

HL_PRIM int HL_NAME(Math_cpConvexHull5)(int count, varray* verts, varray* result, varray* first, double tol) {
	return (cpConvexHull(count, (cpVect*)hl_aptr(verts,HL_CONST double), (cpVect*)hl_aptr(result,double), hl_aptr(first,int), tol));
}
DEFINE_PRIM(_I32, Math_cpConvexHull5, _I32 _ARR _ARR _ARR _F64);

HL_PRIM _ref(cpVect)* HL_NAME(Math_cpClosetPointOnSegment3)(_ref(cpVect)* p, _ref(cpVect)* a, _ref(cpVect)* b) {
	return alloc_ref(new cpVect((cpClosetPointOnSegment(*_unref(p), *_unref(a), *_unref(b)))),Vect);
}
DEFINE_PRIM(_IDL, Math_cpClosetPointOnSegment3, _IDL _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Math_cpCheckPointGreater3)(_ref(cpVect)* a, _ref(cpVect)* b, _ref(cpVect)* c) {
	return (cpCheckPointGreater(*_unref(a), *_unref(b), *_unref(c)));
}
DEFINE_PRIM(_I8, Math_cpCheckPointGreater3, _IDL _IDL _IDL);

HL_PRIM unsigned char HL_NAME(Math_cpCheckAxis4)(_ref(cpVect)* v0, _ref(cpVect)* v1, _ref(cpVect)* p, _ref(cpVect)* n) {
	return (cpCheckAxis(*_unref(v0), *_unref(v1), *_unref(p), *_unref(n)));
}
DEFINE_PRIM(_I8, Math_cpCheckAxis4, _IDL _IDL _IDL _IDL);

}
