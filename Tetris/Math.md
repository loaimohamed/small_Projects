Nice — let’s dig deep and *careably* (math + geometry) explain exactly **why** things went wrong and how to reason about it so it never surprises you again.

# Rotation basics (matrix and polar)

A 2D rotation by angle $a$ (radians) about the **origin** is given by the matrix

$$
R(a)=\begin{pmatrix}\cos a & -\sin a\\[4pt]\sin a & \cos a\end{pmatrix}.
$$

Apply it to a point $p=(x,y)$ to get the rotated point

$$
R(a)p = \big(x\cos a - y\sin a,\; x\sin a + y\cos a\big).
$$

If instead you want an endpoint starting at $s=(s_x,s_y)$ with length $l$ and angle $a$ **measured from the positive x-axis**, the polar formula is the simplest:

$$
p_{\text{polar}} = s + l(\cos a,\; \sin a).
$$

That directly gives the endpoint of the length-$l$ vector starting at $s$.

# Rotating about a point vs rotating about the origin

To rotate an arbitrary point $p$ **around** the point $s$\*\* (not the origin)\*\* by angle $a$, you must:

1. translate so $s$ becomes the origin: $p\mapsto p-s$,
2. rotate with $R(a)$,
3. translate back: $+\;s$.

So the formula is

$$
p_{\text{around }s} = s + R(a)\,(p-s).
$$

This is the correct, standard composition “translate → rotate → translate back.”

# The common bug you had (rotate then translate)

A very common mistake is to instead do

$$
p_{\text{wrong}} = R(a)\,p + s.
$$

That is: rotate **about the origin** and then add $s$. This is *not* the same as rotating around $s$ except in special cases (for example $s=(0,0)$).

Algebraically compare the two (let $p = s + (l,0)$ — the initial endpoint lying to the right of $s$):

* Correct (rotate about $s$):

$$
p_{\text{correct}} = s + R(a)\big(p-s\big) = s + R(a)(l,0) = s + l(\cos a,\;\sin a).
$$

* Wrong (rotate about origin then translate):

$$
p_{\text{wrong}} = R(a)p + s = R(a)\big(s+(l,0)\big) + s = R(a)s + R(a)(l,0) + s.
$$

Take the difference:

$$
p_{\text{wrong}} - p_{\text{correct}} = R(a)s.
$$

So the wrong result is shifted by the vector $R(a)s$ — which is generally nonzero whenever $s\neq(0,0)$. That extra $R(a)s$ both *moves* the endpoint and can *change its direction* relative to $s$.

# Angle difference (a clean geometric insight)

Look at the vector from $s$ to each endpoint:

* Correct vector from $s$ is $v_{\text{correct}} = R(a)(l,0)$. Its direction = $a$ (because $(l,0)$ has direction $0$, rotating by $a$ gives direction $a$).

* Wrong vector from $s$ is

  $$
  v_{\text{wrong}} = p_{\text{wrong}} - s = R(a)p = R(a)\big(s+(l,0)\big).
  $$

  The direction of $v_{\text{wrong}}$ is the direction of $(s+(l,0))$ *plus* $a$ (rotating a vector adds the angle $a$). So

  $$
  \text{angle}(v_{\text{wrong}}) = \text{angle}\big(s+(l,0)\big) + a.
  $$

Therefore the angle offset between wrong and correct is exactly

$$
\Delta\theta = \text{angle}\big(s+(l,0)\big).
$$

In words: **the angular error is the angle that the original point $s+(l,0)$ makes with the x-axis.** It does *not* depend on $a$. So you will see a *constant* angular offset for all rotations $a$ (until you change $s$ or $l$).

**Numeric example (like your Desmos values):**
Let $s=(3,3)$, $l=3$. Then $s+(l,0)=(6,3)$.

$$
\text{angle}(6,3)=\operatorname{atan2}(3,6)=\operatorname{atan2}(0.5)\approx 26.565^\circ.
$$

So the wrong method shifts the vector by roughly $26.565^\circ$ relative to the correct one (for these $s,l$ values). That explains a constant angular offset you observed (it might show numerically as \~26.57° for these numbers — if you thought it was 45° that was just an observation; the actual offset depends on $s,l$ and is $\arctan(\frac{s_y}{s_x+l})$).

# Other pitfalls to watch for

1. **Degrees vs radians.** Desmos trig functions use radians. If you have a slider `q` in degrees do: `a = q * π / 180`. Otherwise `cos(45)` treats 45 as *radians* (huge error).
2. **Sign convention (clockwise vs counterclockwise).** The matrix $R(a)$ above is CCW. If you see a flipped rotation direction, check whether you used `sin` sign swapped (common typo).
3. **Screen coordinates.** Some graphics systems have $y$ growing *downwards*; that flips sign of sine in appearance. Desmos uses the standard math convention $y$ up.
4. **Swapped or missing translations.** Make sure to do `p - s` before rotation and `+ s` after — order matters.

# How to fix in Desmos (short & explicit)

Use either the polar endpoint formula (simple and robust):

```
s = (3,3)
l = 3
q = 0          // degrees slider
a = q * π / 180
nx = s.x + l*cos(a)
ny = s.y + l*sin(a)
```

or rotate an arbitrary point `p` around `s`:

```
f_x(x,y) = s.x + (x - s.x)*cos(a) - (y - s.y)*sin(a)
f_y(x,y) = s.y + (x - s.x)*sin(a) + (y - s.y)*cos(a)
```

Both are correct; the polar formula is easiest when you only need a length `l` from `s`.

# Debug checklist (quick)

* Set `q = 0`. The endpoint should be at `(s.x + l, s.y)`. If not, something else is wrong.
* Set `s = (0,0)` and compare `p_correct` vs `p_wrong`. If they match now, the bug is the rotation-center/translation order.
* Try `q = 90` (with `a = q*pi/180`) and verify `cos(a)=0, sin(a)=1` numerically in Desmos.

---

If you want, I can plug your exact left-panel lines from the screenshot into a small worked Desmos-ready block and show the numeric angles for several `q` values so you can watch the constant offset appear. Want me to do that? 😊
