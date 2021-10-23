import chipmunk.Native;
import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;

class LogoSmash extends hxd.App{

	static var space:chipmunk.Native.CpSpace;

	var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	var heapsObjects : Array<h2d.Bitmap> = [];

	var circleRadius : Int = 4;

	public function makeBall(x : Float, y : Float, radius : Float) : chipmunk.Native.CpShape{
		var body = chipmunk.Native.CpBody.cpBodyNew(1.0, Math.POSITIVE_INFINITY);

		var pos = new chipmunk.Native.CpVect();
		pos.x = x;
		pos.y = y;
		body.cpBodySetPosition(pos);
		
		var cpvzero = new chipmunk.Native.CpVect();
		cpvzero.x = 0;
		cpvzero.y = 0;
		var shape = chipmunk.Native.CpCircleShape.cpCircleShapeNew(body, radius, cpvzero);
		shape.cpShapeSetElasticity(0.0);
		shape.cpShapeSetFriction(0.0);
		
		space.cpSpaceAddBody(body);
		space.cpSpaceAddShape(shape);

		return shape;
	}

	function syncHeapsAndPhysics() {
		// Sync chipmunk physical objects with heaps GUI
		for (i in 0...cpShapesList.length){
			var cpShape = cpShapesList[i];
			var cpBody = cpShape.cpShapeGetBody();
			var heapsObj = heapsObjects[i];

			var pos = cpBody.cpBodyGetPosition();
			heapsObj.x = pos.x;
			heapsObj.y = pos.y;
		}
	}

	override function init() {
		super.init();

		hxd.Res.initEmbed();

		// Init GUI
		@:privateAccess hxd.Window.getInstance().window.title = "LogoSmash Example";   
		var scene2d = new h2d.Scene();
        setScene(scene2d);

		
		

		//var tile = h2d.Tile.fromColor(0xFF0000, circleRadius * 2, circleRadius * 2);
		var tile = hxd.Res.red_circle.toTile();

		var offsetx = 400;
		var offsety = 300;
		var margin = 4;

		for(i in 0...10){
			for(j in 0...10){	
			// Compute position for the next object
			var x = offsetx + i * (circleRadius * 2 + margin); 
			var y = offsety + j * (circleRadius * 2 + margin); 
			
			// Init chipmunk object for physics
			var shape = makeBall(x, y, circleRadius);
			cpShapesList.push(shape);

			// Init heaps object for GUI
			var bmp = new h2d.Bitmap(tile, s2d);
			bmp.x = x; 
			bmp.y = y;
			heapsObjects.push(bmp);
			}
		}

		// Init chipmunk object for phisics moving object
		var shape = makeBall(20, 320, circleRadius);
	
		// Set velocity to the chipmunk object
		var velocity = new chipmunk.Native.CpVect();
		velocity.x = 200;
		velocity.y = 0;
		shape.cpShapeGetBody().cpBodySetVelocity(velocity);
		cpShapesList.push(shape);
		
		// Init chipmunk object for GUI moving object
		var greenTile = hxd.Res.green_circle.toTile();
		var bmp = new h2d.Bitmap(greenTile, s2d);
		bmp.x = 20; 
		bmp.y = 320;
		heapsObjects.push(bmp);
	}

	override function update(dt:Float) {
		space.cpSpaceStep(dt);
		syncHeapsAndPhysics();
	}

	static function main() {
		// Init Chipmunk physics space
		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);
		//space.cpSpaceUseSpatialHash(2.0, 10000);

		new LogoSmash();
    }
}