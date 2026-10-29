I apologize. You're completely right to call that out—I got distracted digging through files when your question was a direct
  architectural design question.

  To answer your meta-question first: I don't have a persistent background indexing daemon running across prompts. Every read inspects
  disk directly to verify ground truth, but I overdid it here instead of just answering your question directly.
  ──────
  ### How Collision & Object Interactions Fit into the Game Loop

  In the model where Environment iterates through sprites and calls update() followed by render(), having sprites handle their own
  collisions inside update() immediately breaks down:

  1. Order dependence: If Sprite A updates and resolves collision against Sprite B before Sprite B has moved, Sprite A reacts to stale
  state.
  2. Coupling & O(N²) complexity: Sprites shouldn't hold references to all other sprites just to calculate their own movement.

  The standard engine architecture breaks the tick into distinct phases managed by the Environment (or a Physics Subsystem within it), not
  individual sprites:

    ┌─────────────────────────────────────────────────────────────┐
    │                       FIXED UPDATE TICK                     │
    │                                                             │
    │  1. Kinematic Update  ──>  2. Collision  ──>  3. Resolution │
    │     (Position/Input)          Detection         & Events    │
    └──────────────────────────────┬──────────────────────────────┘
                                   │
                                   ▼
                            4. Render Phase
  ──────
  ### Phase Breakdown

  #### 1. Kinematics Update (Individual Sprites)

  Each sprite updates its intended movement without worrying about obstacles:

  • Applies forces/velocity: new_pos = pos + velocity * dt
  • Updates animation frame, timers, etc.
  • Saves prev_rect = rect (essential for interpolation during rendering).

  #### 2. Collision Detection (Environment-Level)

  The Environment takes over to find who is touching whom:

  • Broadphase: Don't compare every sprite against every sprite (O(N²)). Use a Spatial Hash Grid, Quadtree, or simple bounding box sweep
  to prune pairs down to only nearby candidates.
  • Layer/Mask Filtering: Ignore collisions that can't happen (e.g., Particle vs Particle, Pickup vs Wall).
  • Narrowphase: Test the remaining candidate pairs with exact geometry (e.g., AABB vs AABB for axis-aligned rectangles, Circle vs Circle,
  or SAT for rotated boxes). Produce a list of Contact Pairs with penetration depth and contact normals.

  #### 3. Collision Resolution & Interaction

  Collisions fall into two distinct categories:

  ##### A. Solid / Physical Collisions (Walls, Obstacles)

  • Who handles it: The Environment / Physics Solver, not the individual sprites.
  • What happens: Positional depenetration (pushing objects apart along the collision normal) and velocity adjustment (stopping or
  bouncing).

  ##### B. Gameplay Interactions / Triggers (Pickups, Projectiles, Damage)

  • Who handles it: The sprites themselves via callbacks or an Event Queue.
  • Common approaches:
      1. Direct Callback:
        struct Collision {
            Sprite* other;
            vec2 normal;
        };
        // Environment calls:
        spriteA->on_collision(collisionWithB);
        spriteB->on_collision(collisionWithA);

      2. Event / Message Bus:
      The environment pushes a CollisionEvent{ entity_a, entity_b } to an event queue. Subsystems (ScoreManager, Inventory, Health)
      process these events.

  ──────
  ### Recommended Environment Architecture

  Splitting the update into distinct stages inside your Environment or Game class:

    void Environment::update(f64 dt)
    {
        // 1. Independent Motion / Input
        for (auto& sprite : m_sprites) {
            sprite->prev_rect = sprite->rect;
            sprite->update(dt); 
        }
    
        // 2. Broadphase + Narrowphase Detection
        auto collisions = m_collision_system.detect_collisions(m_sprites);

        // 3. Physical Depenetration (Walls, Obstacles)
        for (auto& col : collisions) {
            if (col.is_solid) {
                resolve_penetration(col.a, col.b, col.normal, col.depth);
            }
        }

        // 4. Gameplay Logic / Notification
        for (auto& col : collisions) {
            col.a->on_collide(*col.b);
            col.b->on_collide(*col.a);
        }
    }

  ### Key Takeaway: Separation of Visuals vs Colliders

  As your game grows, avoid putting collision logic directly into the renderable Sprite. A Sprite is a graphical representation (texture,
  render rect, UVs). Collision belongs to a Collider / Bounding Box component. This allows you to have:

  • Triggers without graphics (e.g., checkpoints, kill-zones).
  • Visual sprites with no collision (e.g., smoke particles, background tiles).
  • Hitboxes smaller than the sprite texture for tighter gameplay.