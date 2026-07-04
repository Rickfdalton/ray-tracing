implementing Ray tracing from scratch in c++.

## current: full scene (motion blur)
<img width="500" alt="full scene" src="outputs/thumbnails/full_scene.png" />

## progress

| | | |
|---|---|---|
| **hittable spheres** <br> <img width="220" src="outputs/thumbnails/hittable.png" /> | **diffuse (monte carlo)** <br> <img width="220" src="outputs/thumbnails/diffuse.png" /> | **matte / gamma correction** <br> <img width="220" src="outputs/thumbnails/matte.png" /> |
| **lambertian material** <br> <img width="220" src="outputs/thumbnails/lambertian.png" /> | **metal material** <br> <img width="220" src="outputs/thumbnails/metal.png" /> | **glass / refraction** <br> <img width="220" src="outputs/thumbnails/glass.png" /> |
| **geometric refraction** <br> <img width="220" src="outputs/thumbnails/glass_geometrical_try.png" /> | **schlick approximation** <br> <img width="220" src="outputs/thumbnails/glass_schlick.png" /> | **camera fov** <br> <img width="220" src="outputs/thumbnails/camera_fov.png" /> |
| **defocus blur** <br> <img width="220" src="outputs/thumbnails/defocus_blur.png" /> | | |

check the `outputs/` directory for the full-resolution `.ppm` renders.

[1] https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf
