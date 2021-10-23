import chipmunk.Native;
import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;

class LogoSmash extends hxd.App{

	public var space:chipmunk.Native.CpSpace;

	var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	var cpBodyList : Array<chipmunk.Native.CpBody> = [];
	var heapsObjects : Array<h2d.Bitmap> = [];

	//var memberbmp : h2d.Bitmap;

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
		trace("init begin");

		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);

		var redTile = h2d.Tile.fromColor(0xFF0000, 5, 5);
		var greenTile = h2d.Tile.fromColor(0x00FF00, 5, 5);

		for (i in 0...20){
			for (i in 0...20){
				var shape = makeBall(400, 300 + i * 10, circleRadius);
				var memberbmp = new h2d.Bitmap(redTile);
				heapsObjects.push(memberbmp);
			}
		}
		// var shape = makeBall(400, 300, circleRadius);
		// shape = makeBall(400, 310, circleRadius);
		// shape = makeBall(400, 320, circleRadius);
		// shape = makeBall(400, 330, circleRadius);
		// shape = makeBall(400, 340, circleRadius);
		// shape = makeBall(400, 350, circleRadius);
		// shape = makeBall(400, 360, circleRadius);
		// shape = makeBall(400, 370, circleRadius);
		// shape = makeBall(400, 380, circleRadius);
		// shape = makeBall(400, 390, circleRadius);
	
		// shape = makeBall(410, 305, circleRadius);
		// shape = makeBall(410, 315, circleRadius);
		// shape = makeBall(410, 325, circleRadius);
		// shape = makeBall(410, 335, circleRadius);
		// shape = makeBall(410, 345, circleRadius);
		// shape = makeBall(410, 355, circleRadius);
		// shape = makeBall(410, 365, circleRadius);
		// shape = makeBall(410, 375, circleRadius);
		// shape = makeBall(410, 385, circleRadius);
		// shape = makeBall(410, 395, circleRadius);

		// Add a moving ball
		var shape = makeBall(20, 345, circleRadius);
		// Set velocity to the chipmunk object
		var velocity = new chipmunk.Native.CpVect();
		velocity.x = 100;
		velocity.y = 0;
		cpBodyList[cpBodyList.length - 1].cpBodySetVelocity(velocity);
		// -----------------------

		
		//var bmp = new h2d.Bitmap(tile, s2d);
		// var memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);
		
		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		// memberbmp = new h2d.Bitmap(redTile);
		// heapsObjects.push(memberbmp);

		var memberbmp = new h2d.Bitmap(greenTile);
		heapsObjects.push(memberbmp);

		// var redTile = hxd.Res.red_circle.toTile();
		
		
		//super.init();

		//hxd.Res.initEmbed();

		// Init GUI
		//@:privateAccess hxd.Window.getInstance().window.title = "LogoSmash Example";   
		// var scene2d = new h2d.Scene();
        // setScene(scene2d);

		//var tile = h2d.Tile.fromColor(0xFF0000, circleRadius * 2, circleRadius * 2);

		//var redTile = hxd.Res.red_circle.toTile();s
		// var redTile = h2d.Tile.fromColor(0xFF0000, 5, 5);

		// var offsetx = 400;
		// var offsety = 300;
		// var margin = 4;

		//for(i in 0...10){
		//	for(j in 0...10){	
				trace("Add Ball");
				//Compute position for the next object
				// var x = offsetx + i * (circleRadius * 2 + margin); 
				// var y = offsety + j * (circleRadius * 2 + margin); 
				
				//Init chipmunk object for physics
				//var shape = makeBall(0, 0, circleRadius);
				//cpShapesList.push(shape);

				//Init heaps object for GUI
				// var bmp = new h2d.Bitmap(redTile);
				// bmp.x = x; 
				// bmp.y = y;
				// heapsObjects.push(bmp);
		//	}
		//}

		// // Init chipmunk object for phisics moving object
		// var shape = makeBall(20, 320, circleRadius);
	
		// // Set velocity to the chipmunk object
		// var velocity = new chipmunk.Native.CpVect();
		// velocity.x = 200;
		// velocity.y = 0;
		// shape.cpShapeGetBody().cpBodySetVelocity(velocity);
		// cpShapesList.push(shape);
		
		// // Init chipmunk object for GUI moving object
		// //var greenTile = hxd.Res.green_circle.toTile();
		// var greenTile = h2d.Tile.fromColor(0xFF0000, 5, 5);
		// memberbmp = new h2d.Bitmap(greenTile);
		// memberbmp.x = 20; 
		// memberbmp.y = 320;
		// //heapsObjects.push(bmp);

		space.cpSpaceStep(0.02);
		trace("init done");
	}

	public static var frameCount : Int = 0;

	override function update(dt:Float) {

		if (frameCount == 10){
			trace("frameCount == 10");
			//s2d.addChild(memberbmp);
			for (i in 0...heapsObjects.length){
				s2d.addChild( heapsObjects[i]);
			}
		}

		trace("udpate : " + dt);

		space.cpSpaceStep(0.02);
		if (frameCount > 20){
			syncHeapsAndPhysics();
		}

		frameCount += 1;
		trace("udpate done");
	}

	static function main() {
		// Init Chipmunk physics space
		// space = chipmunk.Native.CpSpace.cpSpaceNew();
		// space.cpSpaceSetIterations(1);
		//space.cpSpaceUseSpatialHash(2.0, 10000);
		new LogoSmash();
    }
}