import hl.Type;
import format.pex.Data.ValueWithVariance;
import hl.Bytes;
import chipmunk.Native.Apple;

// class AppleHL extends chipmunk.Native.Apple{
// 	function
// }

// @:generic
// function NativeArrayToBytes(array : hl.NativeArray<T>) : hl.Bytes {
// 	if (Std.isOfType(array[0], Apple)) {
// 		trace("Apple");
// 	}

// 	trace("Unknown type");

// 	return new hl.Bytes(1);
// }

function BytesToNativeArray(){

}

class Main {

	static function NativeArrayToBytes(array : hl.NativeArray<Apple>) : hl.Bytes{
		var length = array.length * 8;
		var bytes = new hl.Bytes(100);

		var position : Int = 0;
		for (item in array) {
			bytes.setI32(position, item.x);
			position += 4;
			bytes.setI32(position, item.y);
			position += 4;
		}

		return bytes;
	}

	static function BytesToNativeArray(bytes : hl.Bytes, outLength : Int) :  hl.NativeArray<Apple>{
		var array = new hl.NativeArray<Apple>(1);
		var bytePos : Int = 0;
		for (i in 0 ... outLength) {
			array[i] = new Apple();
			array[i].x = bytes.getI32(bytePos);
			bytePos += 4;
			array[i].y = bytes.getI32(bytePos);
			bytePos += 4;
		}

		return array;
	}


    static function main() {
		trace("main");

		// Get raw data from C
		var bytes = chipmunk.Native.Chipmunk2D.getApples();

		// Transform to array
		var array = BytesToNativeArray(bytes, 2);
		
		// Trace
		trace("Original: ");
		trace("array[0].x : " + array[0].x);
		trace("array[0].y : " + array[0].y);

		// Alter raw data in C
		chipmunk.Native.Chipmunk2D.alterApples(bytes);

		array = BytesToNativeArray(bytes, 2);
		// Trace again
		trace("Altered: ");
		trace("array[0].x : " + array[0].x);
		trace("array[0].y : " + array[0].y);
    }
}