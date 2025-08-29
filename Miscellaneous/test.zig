const Point = extern struct {
    x: f64,
    y: f64,
};

export fn addPoint_Z(a: *const Point, b: *const Point) Point {
    return Point{
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

export fn combinePoint_Z(p: *const Point, c: i64) i64 {
    const c2 = @as(f64, @bitCast(c));
    return @as(i64, @bitCast(p.x + p.y * c2));
}
