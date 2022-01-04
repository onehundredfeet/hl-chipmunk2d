package chipmunk;

abstract Vector( webidl.Types.VoidPtr ) from webidl.Types.VoidPtr to webidl.Types.VoidPtr {

    public var x(get,never) : Float;
    public var y(get,never) : Float;
    
    function get_x() : Float {
        return 0.;
    }
    function get_y() : Float {
        return 0.;
    }
}