#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <string>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

class Skybox {
public:
    Model model{};
    Shader shader{};
    TextureCubemap cubemap{};
    bool useHDR = false;
    std::string filePath;

    Skybox() = default;

    void Load(const char* path, bool hdr = false) {
        useHDR = hdr;
        filePath = path;

        Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        model = LoadModelFromMesh(cube);

        shader = LoadShader(TextFormat("project/shaders/skybox.vs", GLSL_VERSION),
                            TextFormat("project/shaders/skybox.fs", GLSL_VERSION));

        int loc_env = GetShaderLocation(shader, "environmentMap");
        int cubemapSlot = MATERIAL_MAP_CUBEMAP; 
        SetShaderValue(shader, loc_env, &cubemapSlot, SHADER_UNIFORM_INT);

        int loc_gamma = GetShaderLocation(shader, "doGamma");
        int gamma = useHDR ? 1 : 0;
        SetShaderValue(shader, loc_gamma, &gamma, SHADER_UNIFORM_INT);

        int loc_vflip = GetShaderLocation(shader, "vflipped");
        int vflip = useHDR ? 1 : 0;
        SetShaderValue(shader, loc_vflip, &vflip, SHADER_UNIFORM_INT);

        model.materials[0].shader = shader;

        if (useHDR) {
            Texture2D panorama = LoadTexture(filePath.c_str());
            Shader shdrCubemap = LoadShader(TextFormat("project/shaders/cubemap.vs", GLSL_VERSION),
                                            TextFormat("project/shaders/cubemap.fs", GLSL_VERSION));
            int cubemapSlot[1] = {0};
	    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), cubemapSlot, SHADER_UNIFORM_INT);


            cubemap = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
            model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = cubemap;

            UnloadShader(shdrCubemap);
            UnloadTexture(panorama);
        } else {
            Image img = LoadImage(filePath.c_str());
            cubemap = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
            model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = cubemap;
            UnloadImage(img);
        }
    }

    void Draw(Camera3D& cam) {
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        DrawModel(model, {0,0,0}, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
    }

    void Unload() {
        UnloadShader(shader);
        UnloadTexture(cubemap);
        UnloadModel(model);
    }

private:
    static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format) {
        TextureCubemap cubemap = {0};
        rlDisableBackfaceCulling();

        unsigned int rbo = rlLoadTextureDepth(size, size, true);
        cubemap.id = rlLoadTextureCubemap(0, size, format, 1);

        unsigned int fbo = rlLoadFramebuffer();
        rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

        if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO [ID %i] created successfully", fbo);

        rlEnableShader(shader.id);

        Matrix matProj = MatrixPerspective(90.0*DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProj);

        Matrix fboViews[6] = {
            MatrixLookAt({0,0,0}, {1,0,0}, {0,-1,0}),
            MatrixLookAt({0,0,0}, {-1,0,0}, {0,-1,0}),
            MatrixLookAt({0,0,0}, {0,1,0}, {0,0,1}),
            MatrixLookAt({0,0,0}, {0,-1,0}, {0,0,-1}),
            MatrixLookAt({0,0,0}, {0,0,1}, {0,-1,0}),
            MatrixLookAt({0,0,0}, {0,0,-1}, {0,-1,0})
        };

        rlViewport(0, 0, size, size);
        rlActiveTextureSlot(0);
        rlEnableTexture(panorama.id);

        for (int i = 0; i < 6; i++) {
            rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);
            rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
            rlEnableFramebuffer(fbo);

            rlClearScreenBuffers();
            rlLoadDrawCube();
        }

        rlDisableShader();
        rlDisableTexture();
        rlDisableFramebuffer();
        rlUnloadFramebuffer(fbo);
        rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
        rlEnableBackfaceCulling();

        cubemap.width = size;
        cubemap.height = size;
        cubemap.mipmaps = 1;
        cubemap.format = format;

        return cubemap;
    }
};
