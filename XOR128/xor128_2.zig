pub const XOR128 = extern struct {
    x: u64,
    y: u64,
    z: u64,
    w: u64,
};

export fn xor128_new(xor128: *XOR128, seed: u64) u32 {
    if (seed == 0) return 1;
    xor128.* = .{
        .x = seed,
        .y = 0x12345678,
        .z = 0x9abcdef0,
        .w = 0xfedcba98,
    };
    return 0;
}
export fn xor128_next(xor: *XOR128) u64 {
    const t = xor.x ^ (xor.x << 11);
    xor.x = xor.y;
    xor.y = xor.z;
    xor.z = xor.w;
    xor.w = (xor.w ^ (xor.w >> 19)) ^ (t ^ (t >> 8));
    return xor.w;
}

export fn xor128_xfloat(xor: *XOR128) f64 {
    return @as(f64, @floatFromInt(xor128_next(xor))) / @as(f64, 0xffffffffffffffff);
}
