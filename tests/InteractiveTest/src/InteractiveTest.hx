// An interactive example where you can mouse click something to happen (UI)
import hxd.Event;

class InteractiveTest extends hxd.App{

	public var space:chipmunk.Native.CpSpace;

	var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	var cpBodyList : Array<chipmunk.Native.CpBody> = [];
	var heapsObjects : Array<h2d.Bitmap> = [];

	var circleRadius : Int = 5;

	var mouseDown : Bool = false;

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

	override function init() {

		@:privateAccess hxd.Window.getInstance().window.title = "Performance test. 10000 colliding objects";

		//Add some mouse listener
		function onEvent(event : hxd.Event) {
			if (event.kind == hxd.EventKind.EPush){
				mouseDown = true;
			}

			if (event.kind == hxd.EventKind.ERelease){
				mouseDown = false;
			}

			if (event.kind == hxd.EventKind.EMove){
				if (mouseDown == false) return;

				var pos = new chipmunk.Native.CpVect();
				pos.x = event.relX;
				pos.y = event.relY;
				cpBodyList[cpBodyList.length - 1].cpBodySetPosition(pos);
			}
		}
		hxd.Window.getInstance().addEventTarget(onEvent);

		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);
		
		var redTile = h2d.Tile.fromColor(0xFF0000, circleRadius * 2, circleRadius * 2);
		var greenTile = h2d.Tile.fromColor(0x00FF00, circleRadius * 2, circleRadius * 2);
		
		for (i in 0...10){
			for (j in 0...10){
				var shape = makeBall(i * 50, j * 50, circleRadius);
				var memberbmp = new h2d.Bitmap(greenTile);
				heapsObjects.push(memberbmp);
			}
		}

		// This shape will be moved by mouse
		var shape = makeBall(400, 200, circleRadius);
		var memberbmp = new h2d.Bitmap(redTile);
		heapsObjects.push(memberbmp);

		space.cpSpaceStep(0.02);
	}

	function syncHeapsAndPhysics() {
		// Sync chipmunk physical objects with heaps GUI
		for (i in 0...cpShapesList.length){
			var pos = cpBodyList[i].cpBodyGetPosition();
			heapsObjects[i].x = pos.x;
			heapsObjects[i].y = pos.y;
		}
	}

	public static var frameCount : Int = 0;

	override function update(dt:Float) {

		space.cpSpaceStep(dt);

		if (frameCount == 10){
			// README
			// If I move this to init(), then space.cpSpaceStep(dt); will crash in the first 10 frames. 
			// As I checked, cpSpace get's garbage collected in the first frames.
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
		// Init resources
		//hxd.Res.initEmbed();

		new InteractiveTest();
    }
}