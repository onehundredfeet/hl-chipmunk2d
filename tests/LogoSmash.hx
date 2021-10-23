import chipmunk.Native;
import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;

class LogoSmash extends hxd.App{

	public var spaces:Array<chipmunk.Native.CpSpace> = [];
	//public var space:chipmunk.Native.CpSpace;

	// var cpShapesList : Array<chipmunk.Native.CpShape> = [];
	// var heapsObjects : Array<h2d.Bitmap> = [];

	// var circleRadius : Int = 4;

	// public function makeBall(x : Float, y : Float, radius : Float) : chipmunk.Native.CpShape{
	// 	var body = chipmunk.Native.CpBody.cpBodyNew(1.0, Math.POSITIVE_INFINITY);

	// 	// var pos = new chipmunk.Native.CpVect();
	// 	// pos.x = x;
	// 	// pos.y = y;
	// 	// body.cpBodySetPosition(pos);
		
	// 	var cpvzero = new chipmunk.Native.CpVect();
	// 	cpvzero.x = 0;
	// 	cpvzero.y = 0;
	// 	var shape = chipmunk.Native.CpCircleShape.cpCircleShapeNew(body, radius, cpvzero);
	// 	shape.cpShapeSetElasticity(0.0);
	// 	shape.cpShapeSetFriction(0.0);
		
	// 	space.cpSpaceAddBodyVoid(body);
	// 	space.cpSpaceAddShapeVoid(shape);

	// 	return shape;
	// }

	// function syncHeapsAndPhysics() {
	// 	// Sync chipmunk physical objects with heaps GUI
	// 	for (i in 0...cpShapesList.length){
	// 		var cpShape = cpShapesList[i];
	// 		var cpBody = cpShape.cpShapeGetBody();
	// 		var heapsObj = heapsObjects[i];

	// 		var pos = cpBody.cpBodyGetPosition();
	// 		heapsObj.x = pos.x;
	// 		heapsObj.y = pos.y;
	// 	}
	// }

	override function init() {
		trace("init begin");

		
		

		//var tile = hxd.Res.red_circle.toTile();
		

		// while (true) {
		// 	trace("udpate : " + 0.02);
		// 	space.cpSpaceStep(0.02);
		// 	// syncHeapsAndPhysics();
		// 	trace("udpate done");
		// }

		

		
		//super.init();

		// hxd.Res.initEmbed();

		// // Init GUI
		// @:privateAccess hxd.Window.getInstance().window.title = "LogoSmash Example";   
		// var scene2d = new h2d.Scene();
        // setScene(scene2d);


		// var tile = h2d.Tile.fromColor(0xFF0000, circleRadius * 2, circleRadius * 2);
		

		// var offsetx = 400;
		// var offsety = 300;
		// var margin = 4;

		// for(i in 0...10){
		// 	for(j in 0...10){	
		// 		//Compute position for the next object
		// 		var x = offsetx + i * (circleRadius * 2 + margin); 
		// 		var y = offsety + j * (circleRadius * 2 + margin); 
				
		// 		//Init chipmunk object for physics
		// 		var shape = makeBall(0, 0, circleRadius);
		// 		cpShapesList.push(shape);

		// 		//Init heaps object for GUI
		// 		var bmp = new h2d.Bitmap(tile, s2d);
		// 		bmp.x = x; 
		// 		bmp.y = y;
		// 		heapsObjects.push(bmp);
		// 	}
		// }

		// // Init chipmunk object for phisics moving object
		// var shape = makeBall(20, 320, circleRadius);
	
		// // Set velocity to the chipmunk object
		// var velocity = new chipmunk.Native.CpVect();
		// velocity.x = 200;
		// velocity.y = 0;
		// shape.cpShapeGetBody().cpBodySetVelocity(velocity);
		// cpShapesList.push(shape);
		
		// // Init chipmunk object for GUI moving object
		// var greenTile = hxd.Res.green_circle.toTile();
		// var bmp = new h2d.Bitmap(greenTile, s2d);
		// bmp.x = 20; 
		// bmp.y = 320;
		// heapsObjects.push(bmp);

		trace("init done");
	}

	public static var frameCount : Int = 0;
	public static var mybitmap : h2d.Bitmap;

	override function update(dt:Float) {
		if (spaces.length == 0){

			var space = chipmunk.Native.CpSpace.cpSpaceNew();
			space.cpSpaceSetIterations(1);
	
			var body = chipmunk.Native.CpBody.cpBodyNew(1.0, 1);  // Math.POSITIVE_INFINITY);
			space.cpSpaceAddBodyVoid(body);
			
			space.cpSpaceStep(0.02);
			spaces.push(space);

			var tile = h2d.Tile.fromColor(0xFF0000, 5, 5);
			//var bmp = new h2d.Bitmap(tile, s2d);
			mybitmap = new h2d.Bitmap(tile);
			
			//s2d.addChild(mybitmap);
		}

		if (frameCount == 10){
			s2d.addChild(mybitmap);
		}

		trace("udpate : " + dt);
		var space = spaces[0];

		if (space == null){
			trace("space is null");
		}
		space.cpSpaceStep(0.02);
		// syncHeapsAndPhysics();
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