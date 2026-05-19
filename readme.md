<b><font size="6">Xethium Engine</font></b>

## Lightweight cross-platform 3D engine written in C++

**Xethium Engine** is a lightweight, visually rich engine that's still heavily under development.\
It supports Windows and Linux as of May 2026.

## Free and open source
**Xethium Engine** will stay free and open source forever under the [MIT License](https://github.com/ShaderHex/Xethium-Eng/blob/main/LICENSE)
![XETHIUM-SCREENSHOT](screenshots/fps.jpg)
![XETHIUM-SCREENSHOT2](screenshots/material-test.jpg)
![XETHIUM-SCREENSHOT3](screenshots/xethium-test.png)

## Plans for the future
- [x] Framebuffer
- [x] ECS
- [ ] New renderer
- [ ] Optimize the memory usage for ECS
- [ ] Lighting
- [ ] Shadow map
- [ ] CSM
- [ ] Post-processing support
- [ ] Bloom
- [ ] SSAO (after OpenGL 4.5)

## Current Features
- OpenGL renderer
- Framebuffer rendering
- ECS architecture
- Primitive Cube generation and rendering
- Input system
- Cross-platform support

## Known Bugs/Issues
- Depth test is wrong
- Linux build scripts are unstable
- Framebuffer not scaling up correctly with the window

## How to build
You have to install these required tools:
- C++20
- GCC

Run these scripts
```bash
./build-all.bat
```
This builds both the engine and application (in our case testbed)

```bash
cd engine
./build.bat
```
This builds engine specificly

```bash
cd testbed
./build.bat
```
This builds testbed (the app built on top of the engine) specificly