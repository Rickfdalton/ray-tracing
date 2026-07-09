implementing Ray tracing from scratch in c++.

## current: full scene (BVH accelerated, 1000 samples/pixel)
<img width="500" alt="full scene bvh" src="outputs/thumbnails/full_scene_bvh.png" />

## progress

| | | |
|---|---|---|
| **hittable spheres** <br> <img width="220" src="outputs/thumbnails/hittable.png" /> | **diffuse (monte carlo)** <br> <img width="220" src="outputs/thumbnails/diffuse.png" /> | **matte / gamma correction** <br> <img width="220" src="outputs/thumbnails/matte.png" /> |
| **lambertian material** <br> <img width="220" src="outputs/thumbnails/lambertian.png" /> | **metal material** <br> <img width="220" src="outputs/thumbnails/metal.png" /> | **glass / refraction** <br> <img width="220" src="outputs/thumbnails/glass.png" /> |
| **geometric refraction** <br> <img width="220" src="outputs/thumbnails/glass_geometrical_try.png" /> | **schlick approximation** <br> <img width="220" src="outputs/thumbnails/glass_schlick.png" /> | **camera fov** <br> <img width="220" src="outputs/thumbnails/camera_fov.png" /> |
| **defocus blur** <br> <img width="220" src="outputs/thumbnails/defocus_blur.png" /> | **motion blur** <br> <img width="220" src="outputs/thumbnails/full_scene.png" /> | **BVH acceleration** <br> <img width="220" src="outputs/thumbnails/full_scene_bvh.png" /> |

## acceleration structure

Objects are wrapped in a bounding volume hierarchy (`bvh.h`) before rendering: `world = hitable_list(std::make_shared<bvh_node>(world))`. Each `bvh_node` recursively splits the object list along a random axis, sorted by each object's `aabb` (`aabb.h`), until leaves hold 1-2 primitives; every internal node's box is the union of its children's. `hit()` then rejects whole subtrees via a single box test (`bbox.hit`) instead of checking every object, turning scene traversal into O(log n) instead of O(n).

check the `outputs/` directory for the full-resolution `.ppm` renders.

[1] https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf
