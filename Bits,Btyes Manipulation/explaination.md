Let’s break it down step by step for a 32-bit single-precision float.

---

### IEEE 754 Float (32-bit) Layout

* **1 bit**: Sign (S)
* **8 bits**: Exponent (E)
* **23 bits**: Fraction/Mantissa (M)

```
[ S | EEEEEEEE | MMMMMMMMMMMMMMMMMMMMMMM ]
```

Where:

* `S = 0` → positive, `S = 1` → negative
* `E` is stored with a bias of **127**
* `M` represents the fractional part after the leading 1 (for normal numbers)

---

### Step 1: Extract fields

If `x` is a 32-bit integer:

* `S = (x >> 31) & 1`
* `E = (x >> 23) & 0xFF`
* `M = x & 0x7FFFFF`

---

### Step 2: Interpret

* If `E = 255` and `M ≠ 0` → **NaN**
* If `E = 255` and `M = 0` → **Infinity** (sign depends on `S`)
* If `E = 0` and `M = 0` → **Zero** (sign depends on `S`)
* If `E = 0` and `M ≠ 0` → **Subnormal number**

  $$
  \text{value} = (-1)^S \times 2^{1-127} \times \frac{M}{2^{23}}
  $$
* Otherwise (normal number):

  $$
  \text{value} = (-1)^S \times 2^{E-127} \times \left(1 + \frac{M}{2^{23}}\right)
  $$

---

### Example

Take integer bits: `0x40490FDB`
Binary:

```
0100 0000 0100 1001 0000 1111 1101 1011
```

* `S = 0` (positive)
* `E = 10000000₂ = 128`
* `M = 0x490FDB = 4788187`

Now compute:

$$
(-1)^0 \times 2^{128-127} \times \left(1 + \frac{4788187}{8388608}\right)
$$

$$
= 2^1 \times \left(1 + 0.570796\right)
= 2 \times 1.570796 \approx 3.141592
$$

That’s **π**!

---

✅ That’s the exact **mathematical decoding** of float bits from an integer.

---

Great question 👌 — let’s slow down and explain **how we pull bits out of an integer**.

---

### 1. Numbers are stored as binary bits

A 32-bit `int` or `uint32_t` is just a sequence of 32 **bits** (0 or 1).
Example:

```c
uint32_t bits = 0x40490FDB; // hexadecimal
```

Hex is just a compact way to write binary.
`0x40490FDB` in binary is:

```
01000000 01001001 00001111 11011011
```

(32 bits total)

---

### 2. Extracting bits with **shift** and **mask**

* **Shift (`>>`)**: moves bits to the right, throwing away the lower ones.
* **Mask (`&`)**: keeps only certain bits by AND-ing with a bit pattern.

#### Example: Get the sign bit (highest bit)

```c
int sign = (bits >> 31) & 1;
```

* `>> 31` moves the top bit all the way down.
* `& 1` keeps only that one bit.
  So `sign` is either `0` or `1`.

#### Example: Get the exponent (next 8 bits)

```c
int exponent = (bits >> 23) & 0xFF;
```

* `>> 23` moves bits 23–30 down to the lowest position.
* `0xFF` (`11111111₂`) keeps only those 8 bits.

#### Example: Get the mantissa (last 23 bits)

```c
uint32_t mantissa = bits & 0x7FFFFF;
```

* `0x7FFFFF` = `23` ones in binary: `00000000 01111111 11111111 11111111`
* That masks away the top 9 bits, leaving only the fraction.

---

### 3. Visual diagram

```
[ S |   Exponent (8 bits)   |          Mantissa (23 bits)         ]
 ^       ^--- shift/mask ---> exponent         ^-- mask --> mantissa
 sign = (bits >> 31) & 1
 exp  = (bits >> 23) & 0xFF
 man  = bits & 0x7FFFFF
```

---

### 4. Why shift and mask?

Because we’re **carving out fields** inside a single 32-bit number:

* Shifting aligns the desired field to the **lowest bits**.
* Masking zeroes out everything except the field.

---
# Function to Convert int To float32