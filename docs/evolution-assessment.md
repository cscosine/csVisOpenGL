# csVisOpenGL — Evolution Assessment

Status: draft for discussion · Date: 2026-11-15 · Scope: library at `dev` @ `79cde4b`

This document checks the goals for csVisOpenGL against the code as it is today,
compares it with existing alternatives, and proposes a direction and a phased
roadmap. It is meant to be kept in the repository and updated as decisions are made.

---

## 1. Goals (as stated)

| # | Goal | Short name |
|---|------|------------|
| G1 | Simple way to draw points, lines, poses, meshes… without engine-level complexity | *Simplicity* |
| G2 | Primary domain: robotics / SLAM visualization, but not limited to it | *Domain* |
| G3 | Multiple camera models and interaction modes: orthographic, isometric, first person, orbit… with mouse + keyboard | *Cameras* |
| G4 | Proper 2D usage (today csImgTracker uses it for 2D through a workaround) | *2D* |
| G5 | Port features from older variants of the code: more renderers, picking of entities, mouse/gesture manipulation (move/rotate) | *Porting* |
| G6 | Move to a newer OpenGL version if there is a real advantage | *Modern GPU API* |
| G7 | Usable inside Qt (main use) but also without Qt, in a modular way; GLFW is not wanted | *Host independence* |
| G8 | Visualize data that already lives on the GPU (e.g. CUDA SLAM point clouds, images) without a round trip through the CPU | *GPU-resident data* |
| G9 | Possible base for interactive 2D/3D plotting in C++ (a gap in the C++ ecosystem) | *Plotting* |

---

## 2. Where the code is today

### 2.1 Architecture (as-is)

```
 Qt app
  └─ Frame : QOpenGLWidget              ← owns GL context, event loop hooks, 25 fps timer
       ├─ ShaderFactory                 ← QOpenGLShader, sources from .qrc, "#version 130"
       ├─ VisualizerInterface[]         ← user code: initialize / update / paint* / paintQt
       │     └─ Renderer subclasses     ← one shader + one Vao + one glDrawArrays each
       │     └─ Painters                ← compositions of renderers (axes, grid, cameras…)
       ├─ FrameSignals (QObject)        ← forwards QKeyEvent/QMouseEvent/… as Qt signals
       └─ CameraState  ◄── CameraControllerSignals ◄── OrbitCameraController (FrameSlotsInterface)
```

Renderers available: uniform/per-vertex points, lines, meshes; lit mesh (uniform
or per-vertex color); single texture quad; gradient/solid background; 2D/3D
uncertainty ellipsoids. Painters: axes, grid, box, camera frustums, checkerboard.

### 2.2 Strengths worth keeping

- **Very small and readable** (~4.3 kLOC); easy to understand end to end.
- **Eigen-native API** (`Matrix3Xf`, `Isometry3f`): no conversions for robotics code.
- **Robotics-friendly conventions**: camera pose with x-forward / y-left / z-up.
- **Clear paint passes** (background → opaque → transparent → 2D overlay), a
  sensible model that is easy to explain.
- **Builds through csCMake** with the rest of csfoundation (Paranoid config, asserts).

### 2.3 Structural limitations (relevant to the goals)

| Area | Observation | Impacts |
|------|-------------|---------|
| Qt coupling | Qt is used for **8 different jobs**: window/context, GL function loading (`QOpenGLExtraFunctions`), shader compilation (`QOpenGLShaderProgram`), resources (`.qrc`), images (`QImage`), event plumbing (signals/slots), input event types (`QMouseEvent&` in controller API), text/2D overlay (`QPainter`). | G7 |
| GPU upload timing | `setPoints()` / `setTexture()` upload to the GPU **immediately**, so callers must `makeCurrent()` (csImgTracker does) and cannot feed data from another thread. | G1, G2 |
| Legacy GL features | Relies on a compatibility context: `#version 130`, `glLineWidth > 1`, `GL_POINT_SMOOTH`. Wide lines and smooth points are **not available in core profiles** (and not on macOS). | G6 |
| Mixed GL entry points | Some calls go through `QOpenGLExtraFunctions`, others use global `gl*` symbols (e.g. `TexturesManager`, several `drawConcrete`). | G6, G7 |
| No scene model | No entity IDs, no hierarchy, no visibility/layers; the user’s `VisualizerInterface` *is* the scene. Picking/manipulation (G5) has nothing to attach to. | G5 |
| Camera = projection + controller mixed | `Camera` handles only perspective/orthographic; the single controller (`OrbitCameraController`) is a QObject driven by Qt events. | G3 |
| No 2D mode | csImgTracker emulates 2D with an orbit controller locked to ortho, fixed radius, `zNear=-1, zFar=1`. | G4 |
| Data scaling | One `glDrawArrays` per renderer, `GL_STATIC_DRAW` re-upload on every change, no instancing. Fine for thousands of points, not for streaming large SLAM maps. | G2 |
| Float precision | All geometry and poses are `float`; large (e.g. georeferenced) coordinates will jitter. | G2 |
| Testing | No automated tests and no offscreen rendering. | all |
| Known bugs | See the review: `Frame::event` bypasses `QOpenGLWidget::event`; depth clear 0.0 with `GL_LESS` only works because QPainter resets GL state; shader-compile leak; no copy protection on GL-owning classes. | all |

---

## 3. Considerations, goal by goal

### 3.1 G1 + G2 — Simplicity for robotics / SLAM (and beyond)

**What “simple” should mean here.** The user describes *what* to show, in the
units and types they already use, and does not deal with GL state, shaders,
buffer lifetimes or context currency. A typical SLAM debugging session needs:

| Primitive | Typical data | Notes |
|-----------|-------------|-------|
| Point cloud | 10³–10⁷ points, uniform / per-point color, optional per-point size | streaming, decimation |
| Line segments / line strips | trajectories, correspondences, graph edges | **width in pixels**, dashed optional |
| Poses / frames | `Isometry3` axes triads | thousands (keyframes) → instancing |
| Camera frustums | intrinsics + pose, optional image on the far plane | link to `csCamera` |
| Covariance ellipses/ellipsoids | 2D/3D | already present |
| Meshes | reconstructions, robot models | lit, optional wireframe |
| Image planes / textures | camera images, depth maps with colormap | 2D view too |
| Grids, boxes, arrows, text labels | context and annotations | text needs a solution without QPainter |
| Occupancy grids / height maps | 2D/2.5D maps | texture + colormap |

**Recommended API shape: retained scene + simple handles**, not the current
"subclass a visualizer and call renderers in paint()".

```cpp
auto& scene = viewer.scene();
auto cloud = scene.addPointCloud("map/points");      // returns a handle (entity id)
cloud.setPoints(pts).setColor(Color::gray).setPointSize(3);
auto traj  = scene.addLineStrip("robot/trajectory");
traj.setPoints(positions).setWidth(2);
scene.setTransform("robot", T_world_robot);          // frame hierarchy (tf-like)
```

Key properties:

1. **Data setters are CPU-side and thread-safe**; GPU upload happens at frame
   time on the render thread (double buffering / dirty flags). This removes the
   `makeCurrent()` requirement and allows feeding data from SLAM threads.
2. **Named entities + hierarchy of frames** (like ROS tf): essential for
   robotics, and the foundation for picking (G5).
3. **Styles are data** (color, size, width, visibility, layer), not renderer classes.
4. An optional **immediate-mode “debug draw”** layer (`dbg.line(a,b)`,
   `dbg.axes(T)`, rebuilt every frame) for quick hacks, like im3d / Pangolin.
5. Keep the current **Renderer** level as the *internal* building block (it is
   a reasonable abstraction); it just should not be the user-facing API.

**Precision.** Keep double precision on the CPU side (Eigen `double` poses),
render **camera-relative** (subtract a scene origin / camera position in double
before converting to float). Cheap to do from the start, painful to add later.

**Not in scope (initially):** recording/timeline/playback of logged data. That is
what Rerun is built for (see §4); integrating with it is better than re-implementing it.

### 3.2 G3 — Cameras and interaction

Split three concepts that are mixed today:

| Concept | Responsibility | Variants |
|---------|---------------|----------|
| **Projection** | view space → clip space | perspective (fov), orthographic (scale), *pinhole from intrinsics K* (view through a real camera, image-aligned) |
| **View / pose** | world → view | plain `Isometry3` |
| **Controller** | input events + dt → pose/projection updates | Orbit (turntable, fixed up), Arcball/trackball (free rotation), First person / fly (WASD + mouse look), Pan-zoom 2D, Top-down / map, Follow / attach-to-entity, Fixed (scripted) |

Notes:

- **Isometric** is not a separate projection: it is orthographic + a fixed
  orientation (e.g. yaw 45°, pitch ≈35.26°). Offer it as a *preset* (snap views:
  top, front, side, iso) on top of any orbit/ortho controller.
- **Input abstraction**: controllers receive library-owned event structs
  (`PointerEvent`, `KeyEvent`, `ScrollEvent`, `GestureEvent{pinch, pan, rotate}`)
  plus `dt`; host adapters translate from Qt / SDL. This is the key step that
  makes controllers Qt-independent and testable (feed synthetic events).
- **Input bindings** configurable (which button rotates, modifier keys), because
  orbit conventions differ between tools (Blender, RViz, CAD).
- Keep the existing **smoothing** (damped interpolation); it is a good feature.
- Controllers should be **switchable at runtime** while keeping the view
  continuous (orbit ↔ fly switching without a jump).
- **World-up convention** configurable (z-up default for robotics, y-up for
  graphics/meshes).
- Multiple **viewports** (e.g. 3D view + top-down + camera view) sharing one
  scene: easy if the scene is separate from the view, which the redesign gives for free.

#### Coordinate conventions

Three different frames are involved and must be documented explicitly:

| Frame | Convention | Where today |
|---|---|---|
| Viewer camera (the virtual eye) | x forward, y left, z up, matching a z-up robotics world | `Camera::forward()/left()/up()` |
| OpenGL view space | looks along −z, y up, x right | internal, `Camera::updateViewMatrix()` remaps body → GL |
| Real camera / sensor (OpenCV, `csCamera`) | **optical**: z out of the image, x right, y down | not modelled; `PainterCameras` already draws frustums along +z, i.e. it implicitly expects optical poses |

A real camera *model* (projection with distortion) stays out of scope (it
belongs to `csCamera` or the application). The library must however define
**one fixed optical → body rotation** (as ROS does with `camera_link` vs
`camera_optical_frame`) and use it consistently wherever a sensor is drawn
(frustums, image on the far plane) or looked through (intrinsics projection).
The 2D view uses the y-down pixel convention of the optical frame.

#### Near/far planes and zoom

The current code mixes several concepts. Separating camera and controller fixes
ownership, but only if these rules come with the split:

1. **The eye is always at `pivot − forward · distance`**, in perspective *and*
   orthographic mode. Today ortho places the eye on the pivot (`radius · 0`),
   which clips everything behind it and forces negative near planes
   (csImgTracker uses `zNear = −1, zFar = 1`).
2. **"Zoom" is three different things**, owned by different classes:
   *dolly* changes `distance` (controller); *lens zoom* changes fov or
   ortho half-height (projection). Today `scale` also enters the perspective
   frustum and acts as a hidden fov zoom. The controller maps the wheel / keys to
   one of them, per mode, explicitly.
3. **Perspective ↔ ortho switching keeps the framing**: match
   `orthoHalfHeight = distance · tan(fov/2)`.
4. **Near/far is a policy of the projection**, not a constant
   (today fixed at 0.01 / 1000, a 10⁵ ratio that wastes depth precision and clips large maps):
   - fixed (current behaviour);
   - auto from distance (near ≈ distance/1000, far ≈ distance · k);
   - auto from scene bounds (fit the bounding sphere seen from the eye);
   - reverse-Z with infinite far where GL 4.5 / `ARB_clip_control` is available.
5. **Zoom toward the cursor** and **fit to scene / selection**: the controller
   reads scene bounds or a picked depth, which the redesigned scene provides.

| Concern | Owner |
|---|---|
| fov / ortho half-height, near/far policy, aspect | Projection |
| distance, pivot, orientation, input → dolly / lens-zoom mapping, smoothing, limits | Controller |
| scene bounds, picked depth | Scene (read by controller and near/far policy) |

### 3.3 G4 — 2D usage (csImgTracker)

Today csImgTracker uses a 3D orbit controller forced into a 2D shape. A
first-class 2D view should provide:

- **Pixel coordinate system** of the image (origin top-left, y-down, optional
  +0.5 pixel-center convention matching OpenCV / `csCamera`).
- **Pan/zoom controller** around the cursor, zoom limits, “fit to view”,
  “1:1 pixels”, keeping the aspect ratio.
- **Nearest sampling at high zoom** (to inspect pixels) and linear at low zoom;
  optional pixel grid when strongly zoomed.
- **Overlays in image units** (points, lines, boxes, ellipses, text) with sizes
  either in screen pixels or in image pixels.
- **Cursor readout** (image coordinates, pixel value) and 2D picking of overlays.
- Same scene/entity API as 3D. 2D is just another view + controller + an
  `ImageLayer` entity, not a separate library.

csImgTracker is the first real client and a good **acceptance test** for the
redesign: migrating it should *delete* code from it.

### 3.4 G5 — Porting features from older variants

Recommendation: **do not port into the current architecture**. Picking,
selection and gizmo manipulation need entity IDs, a hierarchy, and an input
abstraction that does not exist today; porting first would mean porting twice.

Suggested process when the old variants are available:

1. **Inventory**: for each variant, list features (renderers, interactions,
   utilities) and diff against this skeleton to isolate what is new.
2. **Classify**: *primitive* (new renderer) → easy, port anytime;
   *interaction* (picking, gizmos, gestures) → after the scene + input
   redesign; *app-specific* → leave out or move to an examples folder.
3. **Port into the new layers**, with a sample and (where possible) an
   offscreen test for each.

Technical notes for the expected features:

- **Picking**: render entity/element IDs into an integer color attachment
  (`R32UI`) and read back the pixel under the cursor, which gives pixel-exact
  results, works for points/lines with width, and is GL 3.3 compatible. Use CPU
  ray casting only as a fallback / for hover on huge scenes.
- **Manipulation**: translate/rotate gizmos (handles per axis/plane) acting on
  an entity’s frame; emit change callbacks so the application owns the truth.
- **Gestures**: pinch/pan/rotate come from the host (Qt delivers
  `QNativeGestureEvent` / touch events); map them to the library’s
  `GestureEvent` in the adapter.

### 3.5 G6 — Newer OpenGL (or something else)?

**What newer GL actually brings (vs. the 3.0/compat level used today):**

| Feature | GL version | Value for this library |
|---------|-----------|------------------------|
| Core profile, VAOs, instancing, integer textures, MRT | 3.3 | **High**: instancing (poses, frustums), ID-buffer picking, clean state |
| Debug output (`KHR_debug`) | 4.3 (ext. widely available) | **High** for development |
| Compute shaders, SSBOs | 4.3 | Medium: GPU decimation/coloring of huge clouds; not needed initially |
| Persistent mapped buffers | 4.4 | Medium: efficient streaming of large point clouds |
| Direct State Access | 4.5 | Low–medium: cleaner code, no functional gain |
| `glClipControl` (reverse-Z) | 4.5 | Medium: precision with large near/far ranges (maps) |

**Constraints:**

- **macOS** has deprecated OpenGL since 2018 and is capped at **4.1**.
- **OpenGL ES 3.0 / WebGL2** (embedded boards, browser) ≈ GL 3.3 core feature set,
  without geometry shaders and without 4.x features.
- **Wide lines and smooth points must be done in shaders anyway**, at any modern
  level: expand each segment/point into a camera-facing quad in the vertex
  shader using instancing (no geometry shader → works on GLES/WebGL/macOS).
  This is the most visible quality improvement and is version-independent.

**Recommendation:**

1. **Baseline: OpenGL 3.3 core / GLES 3.0 common subset**, with optional 4.3+
   features (debug output, reverse-Z, persistent buffers) enabled at runtime
   when available. This is the best portability/benefit trade-off; the real
   improvements (instanced lines/points, picking, streaming, MSAA) do not need 4.5.
2. Use a **standalone GL loader** (e.g. `glad`) and an internal function table
   instead of `QOpenGLExtraFunctions`. That removes one Qt dependency (G7)
   without changing how it looks to Qt users: inside a `QOpenGLWidget` the
   library simply renders into the current context.
3. **Hide the GPU API behind a thin internal interface** (buffers, textures,
   pipelines/shaders, draw), sized to what the renderers need, *not* a general
   RHI. This keeps a later second backend possible without paying for it now.
4. **Do not go to Vulkan directly**: large complexity cost, no benefit at this scale.
5. **Re-evaluate later** a second backend only if a concrete need appears:
   - **WebGPU (Dawn / wgpu-native, via `webgpu.h`)**: modern API, runs on
     Vulkan/Metal/D3D12 and in the browser; the most attractive long-term
     “after GL” option for a small library.
   - **Qt RHI (`QRhi`, `QRhiWidget` since Qt 6.7)**: Qt’s own abstraction over
     Vulkan/Metal/D3D/GL; natural for Qt Quick integration, but it ties the
     renderer to Qt (works against G7) and Qt gives limited compatibility
     guarantees for this API.
   - **sokol_gfx / bgfx**: proven thin abstractions (see §4.2) if a
     ready-made layer is preferred over an own interface.

### 3.6 G7 — Host independence (Qt first, but not only Qt; no GLFW)

**Target layering:**

```
┌──────────────────────────── host adapters (optional, separate targets) ───────────────────────────┐
│ csVis::qt      QOpenGLWidget (and/or QQuickItem) adapter, Qt event → csVis events, QImage helpers  │
│ csVis::sdl     SDL3 window + GL context adapter (standalone apps, no Qt)                            │
│ csVis::headless  EGL/OSMesa offscreen context → images (tests, CI, batch rendering)                 │
└────────────────────────────────────────────────────────────────────────────────────────────────────┘
┌──────────────── csVis::gl  (render backend: GL 3.3 core / GLES 3.0, glad loader, shaders embedded) ──┐
└────────────────────────────────────────────────────────────────────────────────────────────────────┘
┌──────────────── csVis::core (no Qt, no GL): scene, entities, frames, styles, cameras, controllers,   ┐
│                 input event types, picking logic, image struct, colors/colormaps                     │
└────────────────────────────────────────────────────────────────────────────────────────────────────┘
```

How each current Qt use is replaced in the core:

| Qt use today | Replacement in core | Qt adapter keeps |
|---|---|---|
| `QOpenGLWidget` context/window | host provides a current context + size + `requestRedraw()` callback | `QOpenGLWidget` subclass |
| `QOpenGLExtraFunctions` | glad / own function table | — |
| `QOpenGLShaderProgram` | own small shader/program class | — |
| `.qrc` shader resources | shaders embedded as C++ strings at build time (CMake) | — |
| `QImage` | `ImageView{w, h, stride, format, data}` | `fromQImage()` helper |
| signals/slots for events & camera | plain virtual interfaces / `std::function` callbacks | optional QObject wrapper emitting signals |
| `QMouseEvent&` etc. in controller API | `csVis::PointerEvent`, `KeyEvent`, … | translation in adapter |
| `QPainter` overlay (text) | SDF/bitmap font atlas renderer in GL; optional Dear ImGui overlay | keep a `paintQt(QPainter&)` hook for Qt users |

**Hosts without GLFW:**

- **SDL3** (zlib license, stable since 2025): mature, cross-platform window +
  input + GL context, gamepad/touch support. **Recommended** non-Qt host.
- **sokol_app**: minimal header-only alternative; less flexible than SDL.
- **Headless EGL** (Linux) / OSMesa: no window, render to image. **Strongly
  recommended early**: it enables automated tests (golden-image comparisons in
  CI), server-side rendering and screenshots for reports.
- Native per-platform windows: not worth the maintenance.

### 3.7 G8 — Data already on the GPU (CUDA)

When data is produced on the GPU (CUDA SLAM, depth estimation, …) there is
**no need to go back through the CPU**: use **CUDA ↔ OpenGL interop**.

- **Vertex data (point clouds, lines)**: register the library’s GL buffer once
  (`cudaGraphicsGLRegisterBuffer`); each frame map it, get the device pointer,
  write or copy device-to-device, unmap.
- **Images / depth maps**: register the GL texture once
  (`cudaGraphicsGLRegisterImage`), map it, `cudaMemcpy2DToArray` from the device
  image. Float depth gets a colormap in the shader, again without the CPU.

Design consequences:

- **Data-source abstraction** in the scene: CPU-side data (§3.1) or external
  GPU data. The CUDA part lives in an **optional `csVis::cuda` target**; the core
  never depends on CUDA.
- **Two filling modes**:
  - *write in place*: the library allocates the buffer and hands CUDA a mapped
    pointer; the producer kernel writes directly into it (fewest copies);
  - *copy*: the user passes a device pointer; the library does a
    device-to-device copy at frame time (simpler, decouples memory ownership).
- **Flexible vertex layouts** (stride/offset, float3 or float4, interleaved or
  separate arrays) so producers never need a repack kernel.
- **Synchronization**: map/unmap synchronize implicitly; use double buffering
  plus CUDA streams/events so the producer writes buffer N+1 while the viewer
  draws buffer N.
- **Pitfalls**: GL and CUDA must run on the **same GPU** (on hybrid
  Optimus/PRIME laptops GL often runs on the iGPU and interop fails or goes
  through the host): detect it and fall back to a host copy. Interop calls must
  happen on the thread where the GL context is current.
- Works **headless and on Jetson** via EGL on NVIDIA, matching the headless host.
- **Backend impact (§3.5)**: a strong argument to **stay on OpenGL**. Vulkan has
  interop via external memory; WebGPU has no standard CUDA interop today, so a
  future WebGPU backend would need the host-copy fallback for GPU-resident data.

### 3.8 G9 — Plotting on top of csVis

The C++ ecosystem lacks a plotting library that is at the same time
Qt-embeddable, GPU-accelerated, interactive and permissively licensed:

| Library | Gap |
|---|---|
| ImPlot / ImPlot3D | best GPU interactive plotting, but bound to Dear ImGui (awkward inside Qt) |
| QCustomPlot | Qt-embeddable, but QPainter (CPU) and GPL/commercial |
| Qwt, JKQtPlotter | Qt-embeddable, CPU-based; slow with millions of points |
| Qt Graphs | GPL/commercial; not science-oriented |
| Matplot++, sciplot | gnuplot backend; not embeddable / not interactive |
| VTK charts | heavy |

**Assessment: csVis 2D is a good foundation, but plotting should be a separate
module (e.g. `csPlot`) on top of it, not part of csVis.** csVis provides the
data plane: GPU drawing of millions of points/lines, streaming (including CUDA
data, §3.7), pan/zoom, picking for hover tooltips, multiple viewports for subplots.
Most of a plotting library is not rendering, though: tick generation (nice
numbers, log and time axes), labels, legends, colorbars, subplot layout and
margins, autoscaling, markers and dash styles, and above all **text quality**
(which becomes a hard requirement, not a nice-to-have).

Requirements to put into the **2D view design now**, so plotting stays possible:

- **Anisotropic zoom** (independent x/y scales); the image view locks aspect, a plot must not.
- **Axis transforms** (log, symlog) applied in the shader.
- **Double-precision offsets**: e.g. timestamps around 1.7·10⁹ s do not fit in
  float; same camera-relative idea as §3.1, in 2D.
- **Sizes in screen units or data units** for markers and line widths.
- **Viewport clipping** for subplots.

**Positioning**: target *interactive and real-time* plots (signals, residuals,
timings, histograms, heatmaps, live SLAM statistics), where matplotlib is weak
and a GPU library excels. Do **not** chase publication-quality vector output
(PDF/SVG): a GPU renderer does not naturally produce it; export data and use
matplotlib for papers.

**3D plots** (surfaces, 3D scatter, vector fields) are close to what the 3D
scene already does; the missing parts are a 3D axes box with ticks, labels that
face the viewer, colormaps and a colorbar. 3D is also a weak spot of matplotlib
(slow, no proper depth handling), so the value is high.

**Naming.** If the backend becomes abstract, `csVisOpenGL` becomes a misnomer.
Consider `csVis` with components `core`, `gl`, `qt`, `sdl`, `headless`
(the csCMake component model already fits this), keeping
`csVisOpenGL` as a compatibility alias during migration.

---

## 4. Existing alternatives

### 4.1 Complete visualization tools / libraries

| Tool | What it is | Embed in Qt widget | Without Qt | Backend | License | Robotics/SLAM fit | Weight |
|------|-----------|:---:|:---:|---------|---------|-------------------|--------|
| **csVisOpenGL (this)** | small Eigen/Qt/GL drawing lib | ✅ native | ❌ (today) | GL compat | own | good primitives, weak scale | tiny |
| **Rerun** | log-and-view tool: SDK (C++/Python/Rust) streams to a separate viewer, timeline, entity paths | ❌ (separate process / web viewer) | ✅ | wgpu (Rust) | MIT / Apache-2.0 | **excellent** (built for it), timeline & recording | SDK light, viewer is an external app |
| **Pangolin** | lightweight GL viewer used by ORB-SLAM, DSO, … | ⚠️ not designed for it | ✅ own windowing | GL | MIT | very good, the de-facto SLAM viewer | light |
| **Polyscope** | research viewer: register structures (point clouds, curves, meshes), quantities, picking, ImGui UI | ⚠️ possible with effort, owns main loop by default | ✅ (GLFW default, EGL headless) | GL 3.3 | MIT | good for geometry processing; less for live robotics | light-medium |
| **RViz / RViz2** | ROS visualizer, plugins | ✅ (it *is* Qt) | ❌ | Ogre | BSD | excellent **inside ROS** only | heavy (ROS) |
| **Foxglove / Lichtblick** | web-based robotics data visualization (MCAP/ROS) | ❌ | ✅ (browser/electron) | WebGL | Foxglove: proprietary since 2024; Lichtblick: MPL-2.0 fork | excellent for logs/ROS | app, not a library |
| **Open3D visualizer** | point cloud/mesh lib with GUI | ⚠️ | ✅ | Filament (+ legacy GL) | MIT | good for clouds/meshes | heavy |
| **VTK (+PCL visualizer)** | scientific visualization toolkit | ✅ `QVTKOpenGLNativeWidget` | ✅ | GL (WebGPU in progress) | BSD-3 | capable, but verbose API | very heavy |
| **Qt Quick 3D** | Qt’s 3D scene module | ✅ (Qt Quick) | ❌ | Qt RHI | GPL-3 / commercial | generic 3D, not robotics-oriented | medium (Qt) |
| **Magnum** | modular C++ graphics middleware | ✅ (examples) | ✅ (SDL2, EGL, …) | GL (Vulkan WIP) | MIT | generic; you still build the viewer | medium |
| **Viser / meshcat** | Python/web 3D viewers | ❌ | ✅ (browser) | WebGL (three.js) | Apache-2.0 / MIT | nice for Python robotics | n/a for C++ |

### 4.2 Rendering abstraction layers (if not writing an own GL layer)

| Layer | Backends | License | Notes |
|-------|----------|---------|-------|
| **sokol_gfx** | GL 3.3 / GLES3 / Metal / D3D11 / WebGPU | zlib | header-only, tiny; can render into an externally created GL context (works inside `QOpenGLWidget`); shader cross-compile tool `sokol-shdc` |
| **bgfx** | GL, GLES, Vulkan, Metal, D3D11/12, WebGPU | BSD-2 | mature; own shader language and build step; somewhat “owns” the frame |
| **WebGPU native (Dawn / wgpu-native)** | Vulkan / Metal / D3D12 (+ browser) | BSD-3 / MIT-Apache | modern standard API (`webgpu.h`); larger dependency; best future-proofing |
| **Qt RHI** | Vulkan / Metal / D3D / GL | Qt (LGPL) | free inside Qt; ties rendering to Qt |
| **Diligent / Filament** | many | Apache-2.0 | engine-scale; overkill here |

### 4.3 Comparison against the goals

| Goal | Rerun | Pangolin | Polyscope | RViz2 | VTK | Own lib (redesigned) |
|------|:----:|:----:|:----:|:----:|:----:|:----:|
| G1 Simplicity | ✅ | ✅ | ✅ | ⚠️ (ROS) | ❌ | ✅ |
| G2 Robotics/SLAM | ✅✅ | ✅ | ⚠️ | ✅ (ROS) | ⚠️ | ✅ |
| G3 Cameras/controllers | ✅ | ⚠️ | ⚠️ | ✅ | ✅ | ✅ (you decide) |
| G4 2D image tooling | ✅ | ⚠️ | ❌ | ⚠️ | ✅ | ✅ |
| G5 Picking/manipulation | ⚠️ selection only | ⚠️ | ✅ picking | ✅ | ✅ | ✅ (to build) |
| G7 Embedded in a Qt app | ❌ | ❌ | ⚠️ | ✅ | ✅ | ✅ |
| G7 Without Qt, no GLFW | ✅ | ✅ | ⚠️ GLFW default | ❌ | ✅ | ✅ (SDL3/EGL) |
| Eigen-native API | ⚠️ adapters | ⚠️ | ⚠️ | ❌ | ❌ | ✅ |
| Dependency weight | external viewer app | light | light | very heavy | very heavy | minimal |

### 4.4 Verdict on alternatives

- **No alternative covers “embeddable Qt widget + standalone + Eigen-native +
  lightweight + robotics primitives + 2D image tooling” at the same time.** The
  closest are Pangolin (not Qt-embeddable) and Polyscope (own main loop, GLFW,
  research focus). VTK and RViz2 meet the Qt part but are heavy or ROS-bound.
- **Rerun is complementary, not a competitor**: it covers logging, recording,
  timeline and offline inspection, which this library should *not* re-implement.
  Recommended: an optional `csVis ↔ Rerun` bridge later (same entity naming
  and frame conventions), or simply use Rerun next to csVis for recordings.
- **Continuing the own library is justified**, provided it stays small and
  focused. Take design ideas from:
  - Rerun: entity paths (`"robot/camera/points"`), typed archetypes.
  - Polyscope: “register a structure, then attach quantities (colors, scalars)”.
  - Pangolin: lean immediate-mode helpers for SLAM.
  - im3d: immediate-mode debug drawing and gizmos.
- **For the GPU layer**: an own thin GL 3.3/GLES3 layer is the simplest start.
  If a second backend is ever needed, prefer **sokol_gfx** (smallest step,
  works inside a Qt GL context) or **WebGPU native** (most future-proof) over
  writing Vulkan/Metal backends by hand.

---

## 5. Risks and open questions

| Topic | Risk / question | Suggested stance |
|-------|-----------------|------------------|
| Scope creep | Turning a drawing library into an engine or a Rerun clone | Explicit non-goals: no timeline/recording, no physics, no scene file formats, no PBR |
| Backend abstraction | Designing a general RHI too early | Keep the internal GPU interface sized to the renderers; second backend only on concrete need |
| Old variants | Unknown quality/overlap; may hard-wire Qt | Inventory before porting (§3.4) |
| csImgTracker migration | Breaking an existing user | Keep a compatibility shim (`Frame`, `VisualizerInterface`) for one release |
| Threading model | Data from SLAM threads vs. GL thread | CPU-side staging + upload at frame time (§3.1) |
| Text rendering without Qt | Font handling is a rabbit hole | Start with one embedded SDF font (ASCII + basic Latin); Dear ImGui for UI-like needs |
| macOS / mobile | GL deprecated / GLES only | Stay within GL 3.3 core ∩ GLES 3.0; revisit WebGPU if Apple matters |
| Licensing | Qt Quick 3D is GPL/commercial; Foxglove proprietary | Keep core dependency-free; Qt via LGPL modules only |
| CUDA interop | Multi-GPU laptops, thread affinity, stalls on map/unmap | Detect and fall back to host copy; double buffering + streams |
| Plotting scope | A plotting module can absorb all the effort | Separate `csPlot` module, after the 2D view is stable; interactive plots only, no vector export |

Open questions to decide:

1. Is **Qt Quick** (QML) integration needed, or only Qt Widgets?
2. Is a **browser/WebAssembly** target a real future need? (If yes, it pushes toward GLES3/WebGL2 now and WebGPU later.)
3. Expected **data scale** (max points/poses on screen, update rate)? This decides whether persistent buffers / compute are needed soon.
4. Should **`csCamera`** (intrinsics/distortion) be a dependency for frustums and "view through camera", or should csVis have its own minimal pinhole type with an adapter?
5. **Rename** to `csVis` (components `core/gl/qt/sdl/headless`) or keep `csVisOpenGL`?
6. Which **GPU producers** matter (CUDA only, or also others) and which targets (desktop dGPU, Jetson)?
7. Is **plotting** a goal to plan for now (2D view requirements in §3.8) or only an option to keep open?

---

## 6. Proposed roadmap

Each phase leaves the library usable and csImgTracker working.

**Phase 0: Stabilize (small).**
Fix the known bugs (`QOpenGLWidget::event`, depth-clear/depth-func, shader-leak,
non-copyable GL owners, missing includes, warnings). Add a Catch2 test target
for camera math and controllers. Write a meaningful README.

**Phase 1: Decouple the core from Qt.**
Introduce `core` (camera, projections, controllers, input events, image view,
colors) with no Qt dependency; move the GL code onto a glad loader and embedded
shaders; Qt becomes an adapter target. Keep `Frame` / `VisualizerInterface` as
compatibility API. Add the **headless EGL host** and first golden-image tests.

**Phase 2: Modern rendering at the GL 3.3 core / GLES 3.0 baseline.**
Core-profile context; shader-expanded **wide lines** and **round points**
(pixel- or world-sized); instancing for poses/frustums; streaming buffers;
MSAA; camera-relative rendering; text via SDF atlas; GL debug output in Debug/Paranoid.

**Phase 3: Scene + cameras + 2D.**
Retained scene with entity paths, frame hierarchy, styles, visibility; thread-safe
data setters; data-source abstraction (CPU / external GPU). Projection /
controller split with the near/far and zoom rules of §3.2 and documented
coordinate conventions. Controller set: orbit, arcball, fly/first-person, 2D pan-zoom,
follow; view presets (top/front/side/iso); multiple viewports. **2D image view**
designed with the plotting requirements of §3.8 (anisotropic zoom, axis
transforms, double offsets); migrate **csImgTracker** to it. Optional
**`csVis::cuda`** interop target (§3.7).

**Phase 4: Interaction + porting.**
ID-buffer picking, selection/hover, translate/rotate gizmos, gesture events.
Port renderers/features from the older variants following §3.4.

**Phase 5: Reach (on demand).**
SDL3 host for standalone tools; optional Dear ImGui overlay; optional Rerun
bridge; evaluate a WebGPU (or sokol_gfx) backend only if macOS/web/embedded
targets become real requirements (keeping the CUDA host-copy fallback in mind).

**Phase 6: Plotting (separate module `csPlot`).**
Ticks, axes (linear/log/time), labels, legends, colorbars, subplot layout,
autoscaling, hover tooltips; then 3D axes box for surfaces/scatter. Interactive
and real-time focus.

---

## 7. Summary

- The current code is a **good small seed** (clear passes, Eigen-native,
  robotics conventions) but is **structurally coupled to Qt in eight places**,
  uses **legacy GL** for its most visible features, and has **no scene model**,
  which blocks picking and manipulation.
- **Newer OpenGL by itself brings little.** The real gains come from moving to
  a **core-profile 3.3 / GLES 3.0** baseline with modern techniques (instancing,
  shader-expanded lines/points, ID picking, streaming), which also keeps macOS,
  embedded and web options open.
- **Qt independence** is achievable with a `core` / `gl` / host-adapter split;
  **SDL3** and **headless EGL** are the recommended non-Qt hosts (no GLFW).
- **No existing tool fits all goals**; continuing the own library is justified
  if it stays focused. **Rerun is complementary** for logging and timelines.
- Port the older variants **after** the scene and input redesign, not before.
- Camera/controller separation must come with explicit **rules** for
  coordinate frames, zoom semantics and near/far, or the current confusion
  moves into the new classes.
- **GPU-resident (CUDA) data** can be shown without CPU round trips via
  CUDA–GL interop, in an optional module. This also favours staying on OpenGL.
- The 2D view is a **good foundation for an interactive plotting module**
  (`csPlot`), provided anisotropic zoom, axis transforms and double offsets are
  designed in from the start.
