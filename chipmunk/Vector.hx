package chipmunk;

abstract Vector( webidl.Types.VoidPtr ) from webidl.Types.VoidPtr to webidl.Types.VoidPtr {

    public var x(get,never) : Float;
    public var y(get,never) : Float;
    
    inline function get_x() : Float {
        return chipmunk.Native.Vect.x(this);
    }
    inline function get_y() : Float {
        return chipmunk.Native.Vect.y(this);
    }
}