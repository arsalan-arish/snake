# 🐛 Snake Game — Game Loop Bug Analysis

> **Context**: Analysis of why the rectangle moves choppily in the C++ SDL3 Snake project.  
> Bugs are listed by severity, most critical first.

---

## Bug 1 — 🔴 Critical: Wrong delta passed to `update()`

**File**: [`src/game.cpp:105`](file:///C:/Dev/Github/snake/src/game.cpp#L100-L106)

```cpp
void Game::update() {
    sprite->update(keyboardState, m_delta_time); // ← WRONG
}
```

You're using a **fixed timestep loop** (`m_update_spf`). The whole *point* of that pattern is that every physics `update()` tick advances by exactly `m_update_spf` — a fixed, predictable duration. But you're passing `m_delta_time` (the raw, variable frame time) instead. This completely defeats the fixed timestep. The snake moves by inconsistent amounts each tick depending on how long rendering took.

**Fix:**
```cpp
sprite->update(keyboardState, m_update_spf); // ← use the fixed step
```

---

## Bug 2 — 🔴 Critical: Integer truncation destroys sub-pixel movement

**Files**: [`include/snake/sprite.h:5-8`](file:///C:/Dev/Github/snake/include/snake/sprite.h#L5-L8), [`src/sprites/snake.cpp:18-21`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L14-L22)

```cpp
struct pos {
    u32 x;  // ← unsigned integer!
    u32 y;
};
```

```cpp
if (k[SDL_SCANCODE_W]) pos.y -= m_speed * delta_time;
```

`m_speed` is `1` and `delta_time` is `~0.0166...` (1/60). So `m_speed * delta_time ≈ 0.016`. When this is assigned to a `u32`, it **truncates to 0**. The snake literally doesn't move at all per physics tick — it only jerks when accumulation causes some rounding artifact.

**Fix — use float positions in [`sprite.h`](file:///C:/Dev/Github/snake/include/snake/sprite.h)**:
```cpp
struct pos {
    f32 x;  // ← float
    f32 y;
};
```

And increase speed to something meaningful (e.g., `m_speed = 200` for 200 px/sec).

---

## Bug 3 — 🟡 Subtle: `m_alpha` computed but never used in rendering

**Files**: [`src/game.cpp:80`](file:///C:/Dev/Github/snake/src/game.cpp#L80), [`src/game.cpp:116-121`](file:///C:/Dev/Github/snake/src/game.cpp#L109-L125)

```cpp
m_alpha = m_accumulator / m_update_spf; // computed...

// render() — but pos is used raw, not interpolated:
SDL_FRect rect = {
    (f32) sprite->pos.x,  // ← no alpha blending applied
    (f32) sprite->pos.y,
    ...
};
```

There's even a `// TODO: Implement alpha blending` comment in `snake.cpp`. Without interpolation, the rendered position snaps discretely each physics step. With VSync at 60fps and physics at 60Hz they *happen* to align — but if they ever drift (frame spike, different monitor), you'll see stutter.

**Fix — store `prev_pos` and lerp in `render()`**:
```cpp
SDL_FRect rect = {
    prev_pos.x + (sprite->pos.x - prev_pos.x) * m_alpha,
    prev_pos.y + (sprite->pos.y - prev_pos.y) * m_alpha,
    ...
};
```

---

## Bug 4 — 🟡 Subtle: `SDL_NS_TO_SECONDS` does integer division

**File**: [`src/game.cpp:46`](file:///C:/Dev/Github/snake/src/game.cpp#L46), [`src/game.cpp:63`](file:///C:/Dev/Github/snake/src/game.cpp#L63)

```cpp
m_prev_time    = SDL_NS_TO_SECONDS(SDL_GetTicksNS()); // integer division!
m_current_time = SDL_NS_TO_SECONDS(SDL_GetTicksNS());
```

`SDL_NS_TO_SECONDS` is a macro that performs **integer division** (`ns / 1,000,000,000`). So `m_delta_time` is always `0` or `1` — never a useful sub-second fraction. The accumulator never meaningfully fills up.

**Fix — divide manually with floating-point**:
```cpp
m_prev_time    = SDL_GetTicksNS() / 1e9;  // double, preserves precision
m_current_time = SDL_GetTicksNS() / 1e9;
```

---

## Bug 5 — 🟠 Minor: `u32` underflow when moving in negative direction

**File**: [`src/sprites/snake.cpp:18,21`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L18-L21)

```cpp
if (k[SDL_SCANCODE_W]) pos.y -= m_speed * delta_time;
if (k[SDL_SCANCODE_A]) pos.x -= m_speed * delta_time;
```

`pos.y` and `pos.x` are `u32`. Subtracting from them when they are at or near `0` causes **unsigned integer underflow**, wrapping to ~4 billion and teleporting the rect off-screen. This is resolved by fixing Bug 2 (switching to `f32`).

---

## Summary Table

| # | Severity | File | Bug |
|---|----------|------|-----|
| 1 | 🔴 Critical | [`game.cpp:105`](file:///C:/Dev/Github/snake/src/game.cpp#L105) | `m_delta_time` passed to fixed-step `update()` instead of `m_update_spf` |
| 2 | 🔴 Critical | [`sprite.h:6-7`](file:///C:/Dev/Github/snake/include/snake/sprite.h#L6-L7) | `pos.x/y` are `u32` — sub-pixel motion truncates to 0; negative motion wraps |
| 3 | 🟡 Subtle | [`game.cpp:80`](file:///C:/Dev/Github/snake/src/game.cpp#L80), [`game.cpp:116`](file:///C:/Dev/Github/snake/src/game.cpp#L116) | `m_alpha` computed but never used for interpolated rendering |
| 4 | 🟡 Subtle | [`game.cpp:63`](file:///C:/Dev/Github/snake/src/game.cpp#L63) | `SDL_NS_TO_SECONDS` does integer division — `delta_time` is always 0 or 1 |
| 5 | 🟠 Minor | [`snake.cpp:18,21`](file:///C:/Dev/Github/snake/src/sprites/snake.cpp#L18-L21) | `u32` underflow UB when moving in negative direction |

> [!IMPORTANT]
> **Bugs 2 and 4 together** are the smoking gun — delta time is always `0` due to integer truncation, and even if it weren't, sub-pixel motion truncates to `0` on a `u32`. Fix these two first.
