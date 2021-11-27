import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;
import hvector.*;

class Main {

    static function main() {
		trace("main");
		test_cpShape();
		// test_cpCentroidForPoly();
		// test_cpMomentForPoly();
    }

	static function test_cpShape(){
		var pointQueryInfo = new chipmunk.Native.PointQueryInfo();

		pointQueryInfo.point.x = 1.0;
		pointQueryInfo.point.y = 2.0;
		trace("Direct point.x setter result : " + pointQueryInfo.point.x + ", "  + pointQueryInfo.point.y);

		var point = pointQueryInfo.point;
		point.x = 1;
		point.y = 2;
		pointQueryInfo.point = point;
		trace("Setter via aux reference: " + pointQueryInfo.point.x + ", "  + pointQueryInfo.point.y);
	}

	static function test_cpCentroidForPoly(){
		trace('test_cpCentroidForPoly');

		var verts = Float2Array.allocate(4);

		verts[0] = new Float2(0.5, 1.6);
		verts[1] = new Float2(1, 1);
		verts[2] = new Float2(0, 1);
		verts[3] = new Float2(1, 0);
	
		trace (verts);

		var center = chipmunk.Native.Chipmunk2D.cpCentroidForPoly(4, verts);
		trace('centroid: ${center.x}  x  ${center.y}');
	}

	static function test_cpMomentForPoly(){
		trace('test_cpMomentForPoly');

		var vecArray = Float2Array.allocate(10);
		var offset = new chipmunk.Native.Vect();
		
		for (i in 0...10) {
			vecArray[i] = new Float2(i, i);
		}
	
		trace (vecArray);
		var moment = chipmunk.Native.Chipmunk2D.cpMomentForPoly( 1., vecArray.length, vecArray, offset, 1. );

		trace('moment: $moment');
	}
}