// A physics performance example (UI) with lots of objects

class PerformanceTest extends hxd.App{

	public var space:chipmunk.Native.CpSpace;

	var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	var cpBodyList : Array<chipmunk.Native.CpBody> = [];
	var heapsObjects : Array<h2d.Bitmap> = [];

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

	function syncHeapsAndPhysics() {
		// Sync chipmunk physical objects with heaps GUI
		for (i in 0...cpShapesList.length){
			var pos = cpBodyList[i].cpBodyGetPosition();
			heapsObjects[i].x = pos.x;
			heapsObjects[i].y = pos.y;
		}
	}

	override function init() {
		@:privateAccess hxd.Window.getInstance().window.title = "Performance test. 10000 colliding objects";

		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);

		var gravity = new chipmunk.Native.CpVect();
		gravity.x = 0;
		gravity.y = 1;
		space.cpSpaceSetGravity(gravity);

		var redTile = h2d.Tile.fromColor(0xFF0000, 5, 5);

		for (i in 0...100){
			for (j in 0...100){
				var shape = makeBall(i * 10, j * 10, circleRadius);

				// Add some random movement
				var velocity = new chipmunk.Native.CpVect();
				velocity.x = i < 50 ? i * 10 : i * -10;
				velocity.y = i < 50 ? -1 : 1;
				cpBodyList[cpBodyList.length - 1].cpBodySetVelocity(velocity);

				var memberbmp = new h2d.Bitmap(redTile);
				heapsObjects.push(memberbmp);
			}
		}

		// Add a moving ball
		// var shape = makeBall(20, 295, circleRadius);
		// // Set velocity to the chipmunk object
		// var velocity = new chipmunk.Native.CpVect();
		// velocity.x = 300;
		// velocity.y = 50;
		// cpBodyList[cpBodyList.length - 1].cpBodySetVelocity(velocity);
		// -----------------------
		space.cpSpaceStep(0.02);
	}

	public static var frameCount : Int = 0;

	override function update(dt:Float) {

		space.cpSpaceStep(dt);

		if (frameCount == 10){
			// README
			// If I move this to init(), then space.cpSpaceStep(dt); will crash in the first 10 frames. 
			// As I checked, cpSpace get's garbage collected in the first frames.
			//s2d.addChild(memberbmp);
			for (i in 0...heapsObjects.length){
				s2d.addChild( heapsObjects[i]);
			}
		}

		if (frameCount > 10){
			syncHeapsAndPhysics();
		}

		frameCount += 1;
	}

	static function main() {
		new PerformanceTest();
    }
}