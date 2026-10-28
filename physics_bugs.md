# 🐍 Snake Physics Bug Analysis — `snake.cpp`

A detailed analysis of subtle physics bugs, numerical issues, and game loop interactions in `src/sprites/snake.cpp`.

---

## 1. 🔴 Force Vector Overwrite (Superposition Principle Violation)

**File:** [`src/sprites/snake.cpp:37-40`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L37-L40)

```cpp
dvec2 force;

auto& k = keyboardState;
if (k[SDL_SCANCODE_W]) { force = {0.0, -2000.0}; change = true; }
if (k[SDL_SCANCODE_S]) { force = {0.0, 2000.0};  change = true; }
if (k[SDL_SCANCODE_D]) { force = {2000.0, 0.0};  change = true; }
if (k[SDL_SCANCODE_A]) { force = {-2000.0, 0.0}; change = true; }
```

### Problems
1. **Assignment (`=`) instead of vector accumulation (`+=`):**
   In Newtonian physics, the net force on an object is the vector sum of all applied forces:
   $$\vec{F}_{\text{net}} = \sum \vec{F}_i$$
   Because each condition performs a raw assignment, subsequent checks overwrite prior ones.
2. **No diagonal movement:**
   Pressing `W` and `D` simultaneously sets `force` to `(0, -2000)` and then immediately overwrites it to `(2000, 0)`. The vertical component is completely lost.
3. **Opposing forces do not cancel:**
   Pressing `W` and `S` together should result in zero net force ($\vec{F} = \vec{0}$). Instead, `S` overrides `W`, moving the snake down. Pressing `D` and `A` makes `A` override `D`.
4. **Hardcoded input priority:**
   Because of `if` statement order, priority is strictly `A > D > S > W`. `W` has the lowest priority and will never be applied if any other directional key is held.
5. **Uninitialized variable:**
   `dvec2 force;` is uninitialized on line 34. If `change == false`, it holds garbage data.

### Fix
Accumulate the vector components:
```cpp
dvec2 force{0.0, 0.0};
if (k[SDL_SCANCODE_W]) { force.y -= 2000.0; change = true; }
if (k[SDL_SCANCODE_S]) { force.y += 2000.0; change = true; }
if (k[SDL_SCANCODE_D]) { force.x += 2000.0; change = true; }
if (k[SDL_SCANCODE_A]) { force.x -= 2000.0; change = true; }
```

---

## 2. 🔴 Infinite Inertia & Unbounded Acceleration (Missing Damping/Friction)

**File:** [`src/sprites/snake.cpp:42-48`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L42-L48)

```cpp
if (change) {
    m_acceleration  = force / m_mass;
    m_velocity     += m_acceleration * dt;
    m_momentum     += force * dt;
} else { 
    m_acceleration = {0.0, 0.0};
}
```

### Problems
1. **No friction / drag when idle:**
   When keys are released (`change == false`), `m_acceleration` is set to `(0, 0)`, but **`m_velocity` is never reduced**. By Newton's First Law, an object in motion continues moving at constant velocity indefinitely. The snake glides forever across the screen like an air hockey puck on frictionless ice.
2. **Unbounded acceleration:**
   There is no drag ($\vec{F}_{\text{drag}} = -c \vec{v}$) or terminal velocity. Holding a key for just 2 seconds with $a = 2000\text{ px/s}^2$ accelerates the snake to $4000\text{ px/s}$, ejecting it off-screen in fractions of a second.

### Fix
Apply linear drag / damping each physics tick:
```cpp
// Apply friction/drag (e.g. exponential damping)
const f64 damping = 8.0; // Higher = stops faster
m_velocity -= m_velocity * damping * dt;
```

---

## 3. 🟡 Dead / Broken Velocity Cutoff Threshold

**File:** [`src/sprites/snake.cpp:50-51`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L50-L51)

```cpp
if (abs(m_velocity.x) < 0.0001  && !change) m_velocity.x = 0.0;
if (abs(m_velocity.y) < 0.0001  && !change) m_velocity.y = 0.0;
```

### Problems
1. **Step size vs threshold mismatch:**
   At 60 Hz physics ticks ($dt \approx 0.0167\text{ s}$), one physics tick changes velocity by:
   $$\Delta v = a \cdot dt = 2000 \times \frac{1}{60} \approx 33.33\text{ px/s}$$
   Because there is no smooth continuous decay towards zero, velocity remains in discrete steps of $\sim 33.33$.
2. The threshold `0.0001` is $\sim 330,000\times$ smaller than a single physics step. Moving velocity will **never** naturally land in the range $(-0.0001, 0.0001)$. This condition is effectively dead code.

### Fix
With damping applied, use a realistic threshold (e.g., `1.0` or `5.0` px/s):
```cpp
if (glm::length(m_velocity) < 1.0 && !change) {
    m_velocity = {0.0, 0.0};
}
```

---

## 4. 🟡 Momentum Desynchronization ($\vec{p} \ne m\vec{v}$)

**Files:** [`src/sprites/snake.cpp:45`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L45), [`include/snake/sprites/snake.h:21`](file:///C:/Dev/Github/snake/include/snake/sprites/snake.h#L21)

```cpp
m_momentum += force * dt;
```

### Problems
1. Linear momentum is defined as $\vec{p} = m\vec{v}$. It is a derived physical property, not an independent state variable that should be integrated in parallel.
2. If `m_velocity` is stopped, clamped, or zeroed out, `m_momentum` is not updated, creating inconsistent internal physics state.
3. `m_momentum` is never read or used anywhere in the codebase.

### Fix
Either remove `m_momentum` or compute it dynamically on demand:
```cpp
[[nodiscard]] dvec2 momentum() const { return m_velocity * m_mass; }
```

---

## 5. 🟠 Orthogonal Inertial Bleed (Uncontrollable Turning)

**File:** [`src/sprites/snake.cpp:44`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L44)

### Problems
When moving along one axis (e.g., horizontally at $+800\text{ px/s}$ with `D`) and the player presses `W` to go up:
* Vertical force is applied, accelerating the snake upward: $v_y \leftarrow v_y - 2000 \cdot dt$.
* The horizontal velocity $v_x$ remains $+800\text{ px/s}$ indefinitely.
* The snake does not turn; it slides diagonally. To cancel the rightward motion, the player is forced to manually tap `A` for the exact duration they held `D`.

### Fix
For snake or arcade controls, either:
* Damp perpendicular velocity aggressively when turning, or
* Set velocity directly based on heading direction rather than unconstrained 2D acceleration.

---

## 6. 🟠 Floating-Point Precision Mismatch & Numerical Drift

**Files:** [`src/sprites/snake.cpp:53-54`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L53-L54), [`include/snake/sprite.h:15`](file:///C:/Dev/Github/snake/include/snake/sprite.h#L15)

```cpp
this->rect.x += m_velocity.x * dt;
this->rect.y += m_velocity.y * dt;
```

### Problems
1. `m_velocity`, `m_acceleration`, and `dt` use 64-bit precision (`glm::dvec2` / `f64`).
2. `this->rect.x` and `this->rect.y` are 32-bit floats (`float` from `SDL_FRect`).
3. There is no internal persistent 64-bit position vector. Storing authoritative simulation coordinates in `float` truncates sub-pixel accumulation on every physics tick, leading to cumulative floating-point drift.

### Fix
Store authoritative position in `dvec2 m_pos` and sync to `this->rect`:
```cpp
m_pos += m_velocity * dt;
this->rect.x = static_cast<f32>(m_pos.x);
this->rect.y = static_cast<f32>(m_pos.y);
```

---

## 7. 🔴 Extrapolation Bug in Render Interpolation (Game Loop Interaction)

**File:** [`src/game.cpp:120-125`](file:///C:/Dev/Github/snake/src/game.cpp#L120-L125)

```cpp
SDL_FRect rect = {
    sprite->rect.x + (sprite->rect.x - sprite->prev_rect.x) * m_alpha,
    sprite->rect.y + (sprite->rect.y - sprite->prev_rect.y) * m_alpha,
    sprite->rect.w,
    sprite->rect.h
};
```

### Problems
1. **Extrapolating ahead of physics instead of interpolating between steps:**
   `sprite->rect` has already been updated to the new state at $t + \Delta t$.
   Linear interpolation (lerp) between $t$ and $t + \Delta t$ is:
   $$\text{pos} = \text{prev\_rect} + (\text{rect} - \text{prev\_rect}) \times \alpha$$
   By starting at `sprite->rect.x` instead of `sprite->prev_rect.x`, the renderer extrapolates **up to one full physics step into the future**.
   Whenever the snake changes direction or stops, this causes visual overshoot and jitter (snapping backward to real physics position on the next tick).
2. **Uninitialized `prev_rect`:**
   In [`include/snake/sprite.h`](file:///C:/Dev/Github/snake/include/snake/sprite.h), `prev_rect` is never initialized in the constructor. On the first frame before `update()` runs, `prev_rect` contains garbage/uninitialized memory, causing `rect` to evaluate to NaNs or wild coordinates.

### Fix
In [`src/game.cpp`](file:///C:/Dev/Github/snake/src/game.cpp):
```cpp
SDL_FRect rect = {
    sprite->prev_rect.x + (sprite->rect.x - sprite->prev_rect.x) * m_alpha,
    sprite->prev_rect.y + (sprite->rect.y - sprite->prev_rect.y) * m_alpha,
    sprite->rect.w,
    sprite->rect.h
};
```
And initialize `prev_rect(rect)` in [`include/snake/sprite.h`](file:///C:/Dev/Github/snake/include/snake/sprite.h#L9-L12).

---

## Summary Table

| # | Bug | File & Lines | Severity | Impact |
|---|-----|--------------|----------|--------|
| 1 | Force overwrite instead of vector sum | [`snake.cpp:37-40`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L37-L40) | 🔴 High | No diagonal motion; opposing inputs don't cancel; hardcoded priority |
| 2 | No friction/drag & unbounded acceleration | [`snake.cpp:42-48`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L42-L48) | 🔴 High | Slides perpetually; accelerates infinitely off-screen |
| 3 | Velocity cutoff threshold unreachable | [`snake.cpp:50-51`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L50-L51) | 🟡 Medium | Dead code; `m_velocity` never resets to 0 |
| 4 | Momentum integrated separately from velocity | [`snake.cpp:45`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L45) | 🟡 Medium | Violates $p = mv$; state desynchronization |
| 5 | Orthogonal inertial bleed on turns | [`snake.cpp:44`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L44) | 🟠 Medium | Turning doesn't stop lateral movement; slides diagonally |
| 6 | Physics in `f64` accumulated into `f32` rect | [`snake.cpp:53-54`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L53-L54) | 🟠 Minor | Precision loss and truncation drift |
| 7 | Render formula extrapolates instead of lerping | [`game.cpp:120-125`](file:///C:/Dev/Github/snake/src/game.cpp#L120-L125) | 🔴 High | Visual jitter and overshoot when velocity changes |
