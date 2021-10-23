import chipmunk.Native;
import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;

class LogoSmash extends hxd.App{

	public var space:chipmunk.Native.CpSpace;
	public var shape:chipmunk.Native.CpShape;
	public var body:chipmunk.Native.CpBody;

	var bmp : h2d.Bitmap;

    static function main() {
		trace("LogoSmash");
		new LogoSmash();
    }

	public function makeBall(x : Float, y :Float) : chipmunk.Native.CpShape{
		body = chipmunk.Native.CpBody.cpBodyNew(1.0, Math.POSITIVE_INFINITY);

		var pos = new chipmunk.Native.CpVect();
		pos.x = x;
		pos.y = y;
		body.cpBodySetPosition(pos);

		var cpvzero = new chipmunk.Native.CpVect();
		cpvzero.x = 0;
		cpvzero.y = 0;
		var shape = chipmunk.Native.CpCircleShape.cpCircleShapeNew(body, 0.95, cpvzero);
		shape.cpShapeSetElasticity(0.0);
		shape.cpShapeSetFriction(0.0);
		
		return shape;
	}

	override function init() {

		super.init();
		
		@:privateAccess hxd.Window.getInstance().window.title = "LogoSmash Example";

        var scene2d = new h2d.Scene();
        setScene(scene2d);

		var tile = h2d.Tile.fromColor(0xFF0000, 5, 5);
		bmp = new h2d.Bitmap(tile, s2d);
		bmp.x = s2d.width * 0.5; 
		bmp.y = s2d.height * 0.5;

		// var shapes = [bullet.Shape.createSphere(0.5), bullet.Shape.createBox(1,1,1)];
		// for( i in 0...100 ) {
		// 	var id = Std.random(shapes.length);
		// 	var b = new bullet.Body(shapes[id], 0.5, world);
		// 	var m = b.initObject();
		// 	m.x = Math.random() * 10;
		// 	m.y = Math.random() * 10;
		// 	m.z = 10 + Math.random() * 10;
		// 	b.loadPosFromObject();

		// 	var mt = new h3d.Matrix();
		// 	mt.identity();
		// 	mt.colorHue(Math.random() * Math.PI * 2);
		// 	m.material.color.set(0.5, 0.3, 0);
		// 	m.material.color.transform(mt);
		// }

		space = chipmunk.Native.CpSpace.cpSpaceNew();
		space.cpSpaceSetIterations(1);
		space.cpSpaceUseSpatialHash(2.0, 10000);

		var bodyCount:Int = 0;

		shape = makeBall(bmp.x, bmp.y);
		space.cpSpaceAddBody(shape.cpShapeGetBody());
		space.cpSpaceAddShape(shape);
		
		var velocity = new chipmunk.Native.CpVect();
		velocity.x = 100;
		velocity.y = 0;
		body.cpBodySetVelocity(velocity);

		// bodyCount++;

		// for(i in 0...10){
		// 	for(j in 0...10){
		// 		// if(!get_pixel(x, y)) continue;
				
		// 		// cpFloat x_jitter = 0.05*frand();
		// 		// cpFloat y_jitter = 0.05*frand();
				
		// 		var shape = makeBall(0, 0);
		// 		space.cpSpaceAddBody(shape.cpShapeGetBody());
		// 		space.cpSpaceAddShape(shape);
				
		// 		bodyCount++;
		// 	}
		// }
	}

	override function update(dt:Float) {
		space.cpSpaceStep(dt);
		bmp.x += 1;
		var pos = body.cpBodyGetPosition();
		bmp.x = pos.x;
		bmp.y = pos.y;
		//trace('shape pos: ${pos.x}, ${pos.y}');
	}

}