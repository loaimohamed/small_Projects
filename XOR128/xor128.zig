const XOR128 = extern struct {
    x: u64,
    y: u64,
    z: u64,
    w: u64,

    pub fn new(seed: u64) !XOR128 {
        if (seed == 0) {
            return error.NoSeedProvided;
        }

        return XOR128{ .x = seed, .y = 0x12345678, .z = 0x9abcdef0, .w = 0xfedcba98 };
    }

    // Change `this: XOR128` to `this: *XOR128` to take a mutable pointer.
    pub fn next(this: *XOR128) u64 {
        const t: u64 = this.x ^ (this.x << 11);
        this.x = this.y;
        this.y = this.z;
        this.z = this.w;
        this.w = (this.w ^ (this.w >> 19)) ^ (t ^ (t >> 8));
        return this.w;
    }

    pub fn xfloat(this: *XOR128) f64 {
        // You were on the right track here, but the error was stemming from the next() call.
        // Once next() is fixed, this line will work correctly.
        // std.math.maxInt(u64)
        return @as(f64, @floatFromInt(this.next())) / @as(f64, 0xffffffffffffffff);
    }
};

// pub fn main() !void {
//     print("Nice\n", .{});

//     // Use `var` to create a mutable instance of XOR128.
//     var xor = try XOR128.new(123456789);
//     print("Look at this: {!}\n", .{xor.next()});
//     print("Look at this float: {}\n", .{xor.xfloat()});
// }
