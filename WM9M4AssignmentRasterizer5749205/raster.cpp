#include <algorithm>
#include <future>
#include <iostream>
#include <numbers>
#include <chrono>

#include "GamesEngineeringBase.h"  // Include the GamesEngineeringBase header
#include "Multithread.h"
#include "matrix.h"
#include "colour.h"
#include "mesh.h"
#include "zbuffer.h"
#include "renderer.h"
#include "RNG.h"
#include "light.h"
#include "triangle.h"

const size_t cpu = std::thread::hardware_concurrency();
ThreadPool threadpool(cpu);

// Main rendering function that processes a mesh, transforms its vertices, applies lighting, and draws triangles on the canvas.
// Input Variables:
// - renderer: The Renderer object used for drawing.
// - mesh: Pointer to the Mesh object containing vertices and triangles to render.
// - camera: Matrix representing the camera's transformation.
// - L: Light object representing the lighting parameters.
static void render(Renderer& renderer, Mesh* mesh, matrix& camera, Light& L) {
    // Combine perspective, camera, and world transformations for the mesh
    matrix p = renderer.perspective * camera * mesh->world;

    #if OPT_TRIANGLE_EARLY_LIGHT_NORM
        // Optimisation - Normalize the light only once, as the direction is fixed!
        L.omega_i.normalise();
    #endif

    #if OPT_RASTER_ENABLE_VERTEX_CACHING
        // Optimisation - Vertex Caching to avoid redundant calculations (e.g. re-calculating same vertices)
        std::vector<Vertex> vertexCache;
        mesh->vertexPreProcessing(vertexCache, p, renderer.canvas.getWidth(), renderer.canvas.getHeight());
    #endif

    // Iterate through all triangles in the mesh
    for (triIndices& ind : mesh->triangles) {
        Vertex t[3];  // Temporary array to store transformed triangle vertices

        #if OPT_RASTER_ENABLE_VERTEX_CACHING
            // Optimisation - Vertex Caching to avoid redundant calculations (e.g. re-calculating same vertices)
            t[0] = vertexCache[ind.v[0]];
            t[1] = vertexCache[ind.v[1]];
            t[2] = vertexCache[ind.v[2]];
        #else
            #if OPT_RASTER_DISABLE_REDUNDANT_HALF_WIDTH_HEIGH_MUL
                float half_width = 0.5f * static_cast<float>(renderer.canvas.getWidth());
                float half_height = 0.5f * static_cast<float>(renderer.canvas.getHeight());
            #endif
            // Transform each vertex of the triangle
            for (unsigned int i = 0; i < 3; i++) {
                t[i].p = p * mesh->vertices[ind.v[i]].p; // Apply transformations
                t[i].p.divideW(); // Perspective division to normalize coordinates

                // Transform normals into world space for accurate lighting
                // no need for perspective correction as no shearing or non-uniform scaling
                t[i].normal = mesh->world * mesh->vertices[ind.v[i]].normal;
                t[i].normal.normalise();

                // Map normalized device coordinates to screen space
                #if OPT_RASTER_DISABLE_REDUNDANT_HALF_WIDTH_HEIGH_MUL
                    t[i].p[0] = (t[i].p[0] + 1.f) * half_width;
                    t[i].p[1] = (t[i].p[1] + 1.f) * half_height;
                #else
                    t[i].p[0] = (t[i].p[0] + 1.f) * 0.5f * static_cast<float>(renderer.canvas.getWidth());
                    t[i].p[1] = (t[i].p[1] + 1.f) * 0.5f * static_cast<float>(renderer.canvas.getHeight());
                #endif
                t[i].p[1] = renderer.canvas.getHeight() - t[i].p[1]; // Invert y-axis

                // Copy vertex colours
                t[i].rgb = mesh->vertices[ind.v[i]].rgb;
            }
        #endif
        // Clip triangles with Z-values outside [-1, 1]
        if (fabs(t[0].p[2]) > 1.f || fabs(t[1].p[2]) > 1.f || fabs(t[2].p[2]) > 1.f) continue;

        // Create a triangle object and render it
        triangle tri(t[0], t[1], t[2]);
        tri.draw(renderer, L, mesh->ka, mesh->kd);
    }
}

static void renderMT(Renderer& renderer, Mesh* mesh, matrix& camera, Light& L) {
    // Combine perspective, camera, and world transformations for the mesh
    matrix p = renderer.perspective * camera * mesh->world;

    size_t triangle_size = mesh->triangles.size();
    size_t triangle_chunk = (triangle_size) / cpu;

    for (size_t start = 0; start < triangle_size; start += triangle_chunk) {
        size_t end = (start + triangle_chunk > triangle_size) ? start + triangle_chunk : triangle_size;
        threadpool.enqueue([&renderer, mesh, &p, &L, start, end] {
            std::vector<Vertex> vertexCache;
            mesh->vertexPreProcessingMT(vertexCache, p, renderer.canvas.getWidth(), renderer.canvas.getHeight(), start, end);

            // Iterate through all triangles in the mesh
            for (size_t i = start; i < end; i++) {
                triIndices& ind = mesh->triangles[i];
                Vertex t[3];  // Temporary array to store transformed triangle vertices

                t[0] = vertexCache[ind.v[0]];
                t[1] = vertexCache[ind.v[1]];
                t[2] = vertexCache[ind.v[2]];

                // Clip triangles with Z-values outside [-1, 1]
                if (fabs(t[0].p[2]) > 1.f || fabs(t[1].p[2]) > 1.f || fabs(t[2].p[2]) > 1.f) continue;

                // Create a triangle object and render it
                triangle tri(t[0], t[1], t[2]);
                tri.draw(renderer, L, mesh->ka, mesh->kd);
            }
        });
    }
    threadpool.wait();
}

// Test scene function to demonstrate rendering with user-controlled transformations
// No input variables
static void sceneTest() {
    Renderer renderer;
    // create light source {direction, diffuse intensity, ambient intensity}
    Light L{ vec4(0.f, 1.f, 1.f, 0.f), colour(1.f, 1.f, 1.f), colour(0.2f, 0.2f, 0.2f) };
    // camera is just a matrix
    matrix camera = matrix::makeIdentity(); // Initialize the camera with identity matrix

    bool running = true; // Main loop control variable

    std::vector<Mesh*> scene; // Vector to store scene objects

    // Create a sphere and a rectangle mesh
    Mesh mesh = Mesh::makeSphere(1.f, 10, 20);
    //Mesh mesh2 = Mesh::makeRectangle(-2, -1, 2, 1);

    // add meshes to scene
    scene.push_back(&mesh);
    // scene.push_back(&mesh2); 

    float x = 0.0f, y = 0.0f, z = -4.0f; // Initial translation parameters
    mesh.world = matrix::makeTranslation(x, y, z);
    // mesh2.world = matrix::makeTranslation(x, y, z) * matrix::makeRotateX(0.01f);

    // Main rendering loop
    while (running) {
        renderer.canvas.checkInput(); // Handle user input
        renderer.clear(); // Clear the canvas for the next frame

        // Apply transformations to the meshes
        // mesh2.world = matrix::makeTranslation(x, y, z) * matrix::makeRotateX(0.01f);
        mesh.world = matrix::makeTranslation(x, y, z);

        // Handle user inputs for transformations
        if (renderer.canvas.keyPressed(VK_ESCAPE)) break;
        if (renderer.canvas.keyPressed('A')) x += -0.1f;
        if (renderer.canvas.keyPressed('D')) x += 0.1f;
        if (renderer.canvas.keyPressed('W')) y += 0.1f;
        if (renderer.canvas.keyPressed('S')) y += -0.1f;
        if (renderer.canvas.keyPressed('Q')) z += 0.1f;
        if (renderer.canvas.keyPressed('E')) z += -0.1f;

        // Render each object in the scene
        for (auto& m : scene)
            render(renderer, m, camera, L);

        renderer.present(); // Display the rendered frame
    }
}

// Utility function to generate a random rotation matrix
// No input variables
static matrix makeRandomRotation() {
    RandomNumberGenerator& rng = RandomNumberGenerator::getInstance();
    unsigned int r = rng.getRandomInt(0, 3);
    float pi = std::numbers::pi_v<float>;

    switch (r) {
        case 0: return matrix::makeRotateX(rng.getRandomFloat(0.f, 2.f * pi));
        case 1: return matrix::makeRotateY(rng.getRandomFloat(0.f, 2.f * pi));
        case 2: return matrix::makeRotateZ(rng.getRandomFloat(0.f, 2.f * pi));
        default: return matrix::makeIdentity();
    }
}

// Just a prototype for scene 3, consisting of a corridor of 400 random rotating cubes
// Deprecated Scene - but will leave on the source code to show where the inspiration came from, for the finalized scene
static void scene3_prototype() {
    Renderer renderer;
    matrix camera = matrix::makeIdentity();
    Light L{ vec4(0.f, 1.f, 1.f, 0.f), colour(1.0f, 1.0f, 1.0f), colour(0.2f, 0.2f, 0.2f) };

    std::vector<Mesh*> scene;

    RandomNumberGenerator& rng = RandomNumberGenerator::getInstance();
    struct rRot { float x; float y; float z; }; // Structure to store random rotation parameters
    std::vector<rRot> rotations;

    // Create a scene with 400 random cubes
    for (unsigned int i = 0; i < 50; i++) {
        Mesh* m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(-2.f, -2.f, (-3 * static_cast<float>(i)));
        rRot r{ rng.getRandomFloat(-.1f, .1f), rng.getRandomFloat(-.1f, .1f), rng.getRandomFloat(-.1f, .1f) };
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(2.f, -2.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(-2.f, 0.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(2.f, 0.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(-2.f, 2.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(2.f, 2.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(0.f, -2.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);

        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        scene.push_back(m);
        m->world = matrix::makeTranslation(0.f, 2.f, (-3 * static_cast<float>(i)));
        rotations.push_back(r);
    }

    float zoffset = 8.f;  // Initial camera Z-offset
    float step = -0.1f;   // Step size for camera movement

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    int cycle = 0;

    bool running = true;

    // Main rendering loop
    while (running) {
        renderer.canvas.checkInput();
        renderer.clear();

        camera = matrix::makeTranslation(0.f, 0.f, -zoffset); // Update camera position

        // Rotate each cube in the grid
        for (unsigned int i = 0; i < rotations.size(); i++)
            scene[i]->world = scene[i]->world * matrix::makeRotateXYZ(rotations[i].x, rotations[i].y, rotations[i].z);

        if (renderer.canvas.keyPressed(VK_ESCAPE)) break;

        zoffset += step;
        if (zoffset < -150.f || zoffset > 8.f) {
            step *= -1.f;
            if (++cycle % 2 == 0) {
                end = std::chrono::high_resolution_clock::now();
                std::cout << cycle / 2 << " :" << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
                start = std::chrono::high_resolution_clock::now();
            }
        }

        for (auto& m : scene)
            render(renderer, m, camera, L);
        renderer.present();
    }

    for (auto& m : scene)
        delete m;
}

// Function to render a scene with multiple objects and dynamic transformations
// No input variables
static void scene1() {
    Renderer renderer;
    matrix camera;
    Light L{ vec4(0.f, 1.f, 1.f, 0.f), colour(1.f, 1.f, 1.f), colour(0.2f, 0.2f, 0.2f) };

    bool running = true;

    std::vector<Mesh*> scene;

    // Create a scene of 40 cubes with random rotations
    for (unsigned int i = 0; i < 20; i++) {
        Mesh* m = new Mesh();
        *m = Mesh::makeCube(1.f);
        m->world = matrix::makeTranslation(-2.f, 0.f, (-3 * static_cast<float>(i))) * makeRandomRotation();
        scene.push_back(m);
        m = new Mesh();
        *m = Mesh::makeCube(1.f);
        m->world = matrix::makeTranslation(2.f, 0.f, (-3 * static_cast<float>(i))) * makeRandomRotation();
        scene.push_back(m);
    }

    float zoffset = 8.f;  // Initial camera Z-offset
    float step = -0.1f;   // Step size for camera movement

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    int cycle = 0;

    // Main rendering loop
    while (running) {
        renderer.canvas.checkInput();
        renderer.clear();

        camera = matrix::makeTranslation(0.f, 0.f, -zoffset); // Update camera position

        // Rotate the first two cubes in the scene
        scene[0]->world = scene[0]->world * matrix::makeRotateXYZ(0.1f, 0.1f, 0.f);
        scene[1]->world = scene[1]->world * matrix::makeRotateXYZ(0.f, 0.1f, 0.2f);

        if (renderer.canvas.keyPressed(VK_ESCAPE)) break;

        zoffset += step;
        if (zoffset < -60.f || zoffset > 8.f) {
            step *= -1.f;
            if (++cycle % 2 == 0) {
                end = std::chrono::high_resolution_clock::now();
                std::cout << cycle / 2 << " :" << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
                start = std::chrono::high_resolution_clock::now();
            }
        }

        for (auto& m : scene) {
            #if OPT_MULTITHREAD_USE_MT
                renderMT(renderer, m, camera, L);
            #else
                render(renderer, m, camera, L);
            #endif
        }
        renderer.present();
    }

    for (auto& m : scene)
        delete m;
}

// Scene with a grid of cubes and a moving sphere
// No input variables
static void scene2() {
    Renderer renderer;
    matrix camera = matrix::makeIdentity();
    Light L{ vec4(0.f, 1.f, 1.f, 0.f), colour(1.0f, 1.0f, 1.0f), colour(0.2f, 0.2f, 0.2f) };

    std::vector<Mesh*> scene;

    struct rRot { float x; float y; float z; }; // Structure to store random rotation parameters
    std::vector<rRot> rotations;

    RandomNumberGenerator& rng = RandomNumberGenerator::getInstance();

    // Create a grid of cubes with random rotations
    for (unsigned int y = 0; y < 6; y++) {
        for (unsigned int x = 0; x < 8; x++) {
            Mesh* m = new Mesh();
            *m = Mesh::makeCube(1.f);
            scene.push_back(m);
            m->world = matrix::makeTranslation(-7.f + (static_cast<float>(x) * 2.f), 5.f - (static_cast<float>(y) * 2.f), -8.f);
            rRot r{ rng.getRandomFloat(-.1f, .1f), rng.getRandomFloat(-.1f, .1f), rng.getRandomFloat(-.1f, .1f) };
            rotations.push_back(r);
        }
    }

    // Create a sphere and add it to the scene
    Mesh* sphere = new Mesh();
    *sphere = Mesh::makeSphere(1.f, 10, 20);
    scene.push_back(sphere);
    float sphereOffset = -6.f;
    float sphereStep = 0.1f;
    sphere->world = matrix::makeTranslation(sphereOffset, 0.f, -6.f);

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    int cycle = 0;

    bool running = true;
    while (running) {
        renderer.canvas.checkInput();
        renderer.clear();

        // Rotate each cube in the grid
        for (unsigned int i = 0; i < rotations.size(); i++)
            scene[i]->world = scene[i]->world * matrix::makeRotateXYZ(rotations[i].x, rotations[i].y, rotations[i].z);

        // Move the sphere back and forth
        sphereOffset += sphereStep;
        sphere->world = matrix::makeTranslation(sphereOffset, 0.f, -6.f);
        if (sphereOffset > 6.f || sphereOffset < -6.f) {
            sphereStep *= -1.f;
            if (++cycle % 2 == 0) {
                end = std::chrono::high_resolution_clock::now();
                std::cout << cycle / 2 << " :" << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
                start = std::chrono::high_resolution_clock::now();
            }
        }

        if (renderer.canvas.keyPressed(VK_ESCAPE)) break;

        for (auto& m : scene) {
            #if OPT_MULTITHREAD_USE_MT
                renderMT(renderer, m, camera, L);
            #else
                render(renderer, m, camera, L);
            #endif
        }
        renderer.present();
    }

    for (auto& m : scene)
        delete m;
}

// These scene will be written to showcase different parallel issues and optimisations
// Helix Benchmark - High density geometry stress test
// No input variables
static void scene3() {
    Renderer renderer;
    matrix camera = matrix::makeIdentity();
    Light L { vec4(0.f, 1.f, 1.f, 0.f), colour(1.0f, 1.0f, 1.0f), colour(0.2f, 0.2f, 0.2f) };
    RandomNumberGenerator& rng = RandomNumberGenerator::getInstance();

    // Structure to store random rotation parameters
    struct rRot { float x; float y; float z; };
    
    // Vectors to store meshes and rotations
    std::vector<Mesh*> scene;
    std::vector<rRot> rotations;

    // Initialize parameters for a scene with 1600 meshes
    int rings = 100;
    int meshesPerRing = 16;
    float ringRadius = 6.f;   // Radius of the ring
    float ringDepth = 3.5f;   // Distance between two rings
    float pi = std::numbers::pi_v<float>;

    // Pre-allocate memory for the scene & rotations
    scene.reserve(rings * meshesPerRing);
    rotations.reserve(rings * meshesPerRing);

    // Mesh generation for the vortex warp, consisting of cubes and spheres, total of 1600 meshes
    for (int i = 0; i < rings; i++) {
        float offset = static_cast<float>(i) * 0.2f;
        for (int j = 0; j < meshesPerRing; j++) {
            Mesh* m = new Mesh();

            // Make every 5th object a sphere, else make it a cube
            if ((i * meshesPerRing + j) % 5 == 0) *m = Mesh::makeSphere(1.f, 15, 15);
            else *m = Mesh::makeCube(1.f);
            scene.push_back(m);

            // Find the current angle for polar coordinates
            float theta = (static_cast<float>(j) / meshesPerRing) * 2 * pi;
            theta += offset;

            // Calculate polar coordinates for the rings, and set the word position of the mesh
            float x = std::cos(theta) * ringRadius;
            float y = std::sin(theta) * ringRadius;
            float z = -ringDepth * static_cast<float>(i);
            m->world = matrix::makeTranslation(x, y, z);

            // Add rotation speed
            rRot r{ rng.getRandomFloat(-.1f, .1f), rng.getRandomFloat(-.1f, .1f) , rng.getRandomFloat(-.1f, .1f) };
            rotations.push_back(r);
        }
    }
    float zoffset = 8.f;   // Initial camera Z-offset
    float step = -0.15f;   // Step size for camera movement
    float maxDepth = -(static_cast<float>(rings) * ringDepth) + 10.f;

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    int cycle = 0;

    bool running = true;

    // Main rendering loop
    while (running) {
        renderer.canvas.checkInput();
        renderer.clear();
        camera = matrix::makeTranslation(0.f, 0.f, -zoffset); // Update camera position

        // Rotate each cube in the grid
        for (unsigned int i = 0; i < rotations.size(); i++)
            scene[i]->world = scene[i]->world * matrix::makeRotateXYZ(rotations[i].x, rotations[i].y, rotations[i].z);
        
        if (renderer.canvas.keyPressed(VK_ESCAPE)) break;
        zoffset += step;

        if (zoffset < maxDepth || zoffset > 8.f) {
            step *= -1.f;
            if (++cycle % 2 == 0) {
                end = std::chrono::high_resolution_clock::now();
                std::cout << cycle / 2 << " :" << std::chrono::duration<double, std::milli>(end - start).count() << "ms\n";
                start = std::chrono::high_resolution_clock::now();
            }
        }
        for (auto& m : scene) {
            #if OPT_MULTITHREAD_USE_MT
                renderMT(renderer, m, camera, L);
            #else
                render(renderer, m, camera, L);
            #endif
        }
        renderer.present();
    }
    for (auto& m : scene)
        delete m;
}

// Entry point of the application
// No input variables
int main() {
    // Uncomment the desired scene function to run
    //scene1();
    //scene2();
    //scene3_prototype();
    scene3();
    //sceneTest();
    return 0;
}