#include <platform/platform.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <engine/scene.hpp>
#include <engine/components/camera.hpp>
#include <engine/components/light.hpp>
#include <engine/components/rotator.hpp>
#include <engine/components/material.hpp>

#include <iostream>

#define SCREEN_WIDTH 956
#define SCREEN_HEIGHT 540
#define APPLICATION_TITLE "eris - engine"

int main()
{
    Platform platform;
    Scene scene;

    std::cout << "======================================\n";
    if (!platform.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, APPLICATION_TITLE))
    {
        std::cerr << "Failed to initialize platform\n";
        return -1;
    }

    if (!scene.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "MultiLightScene"))
    {
        std::cerr << "Failed to initialize scene\n";
        return -1;
    }
    std::cout << "======================================\n";

    platform.ListenEvent([&scene](int w, int h)
                         { scene.Resize(w, h); });

    auto source0 = scene.CreateEntity("DirectionalLight");
    source0->transform.position = vec3(2.0f);
    auto l0 = source0->AddComponent<Light>();
    l0->type = LightType::Directional;
    l0->color = vec4(vec3(1.0f), 0.3f);
    
    auto source1 = scene.CreateEntity("PointLight");
    source1->transform.position = vec3(-2.0f, 2.0f, 2.0f);
    auto l1 = source1->AddComponent<Light>();
    l1->type = LightType::Point;
    l1->color = vec4(vec3(1.0f, 0.3f, 0.2f), 0.75f);

    auto source2 = scene.CreateEntity("SpotLight");
    source2->transform.position = vec3(4.0, 3.0f, 0.0f);
    auto l2 = source2->AddComponent<Light>();
    l2->type = LightType::Spot;
    l2->direction = vec3(-2.0f, -1.0f, 0.0f);
    l2->innerCutOff = 12.5f;
    l2->outerCutOff = 17.5f;

    l2->constant = 1.0f;
    l2->linear = 0.09f;
    l2->quadratic = 0.032f;

    l2->color = vec4(1.0f, 0.95f, 0.8f, 1.0f);

    auto cameraEntity = scene.CreateEntity("MainCamera");
    auto camera = cameraEntity->AddComponent<Camera>();
    cameraEntity->transform.position = {0.0f, 3.0f, 8.0f};
    cameraEntity->transform.rotation = quat::FromEuler({-15.0f, 0.0f, 0.0f});

    auto cube = scene.LoadModel("assets/models/cube.fbx");
    cube->transform.position = {0.0f, 1.0f, 0.0f};
    cube->AddComponent<Rotator>(); // rotation for specular test

    auto floor = scene.LoadModel("assets/models/plane.fbx");
    floor->transform.position = {0.0f, 0.0f, 0.0f};

    scene.Start();
    while (!platform.ShouldClose())
    {
        platform.PollEvents();
        scene.Update(platform.GetDeltaTime());
        platform.SwapBuffers();
    }

    std::cout << "Exiting...\n";
    return 0;
}
