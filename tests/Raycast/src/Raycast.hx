
class Raycast {

	public var space:chipmunk.Native.CpSpace;

	var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	var cpBodyList : Array<chipmunk.Native.CpBody> = [];

	var circleRadius : Int = 4;

	public function makeBall(x : Float, y : Float, radius : Float) : chipmunk.Native.CpShape{
		var body = chipmunk.Native.CpBody.cpBodyNew(1.0, Math.POSITIVE_INFINITY);

		var pos = new chipmunk.Native.CpVect();
		pos.x = x;
		pos.y = y;
		body.cpBodySetPosition(pos);
		
		cpBodyList.push(body);

		var cpvzero = new chipmunk.Native.CpVect();
		cpvzero.x = 0;
		cpvzero.y = 0;
		var shape = chipmunk.Native.CpCircleShape.cpCircleShapeNew(body, radius, cpvzero);
		shape.cpShapeSetElasticity(0.0);
		shape.cpShapeSetFriction(0.0);
		
		space.cpSpaceAddBodyVoid(body);
		space.cpSpaceAddShapeVoid(shape);

		cpShapesList.push(shape);

		return shape;
	}

	public function new(){
		trace('Init chipmunk space');
		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);
		
		var shape = makeBall(0, 0, 40);

		space.cpSpaceStep(0.02);

		var begin = new chipmunk.Native.CpVect();
		begin.x = -100;
		begin.y = -100;

		var end = new chipmunk.Native.CpVect();
		end.x = 100;
		end.y = 100; // Add some slope to make sure the ray does not pass between shapes grid

		var out = new chipmunk.Native.CpSegmentQueryInfo();
		var queryResult = space.cpSpaceSegmentQueryFirstNoFilter(begin, end, 2000, out);
		var queryRePoint = out.point;
		var queryReNormal = out.normal;

		trace('Query intersection point : ${queryRePoint.x} ${queryRePoint.y} ');
		trace('Query intersection normal : ${queryReNormal.x} ${queryReNormal.y} ');

		var queryResultBody = queryResult.cpShapeGetBody();
		var queryResultPos = queryResultBody.cpBodyGetPosition();
		trace('Query intersection shape pos : ${queryResultPos.x} ${queryResultPos.y} ');
		trace('Done');
	}
	

	static function main() {
		new Raycast();
    }
}