#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "renderer.h"

struct Tile {
    unsigned int x{ 0 }, y{ 0 };
    unsigned int width{ 0 }, height{ 0 };

    unsigned int x_start(Renderer& renderer) const { return x; }
    unsigned int x_end(Renderer& renderer) const { return std::min(x + width, renderer.canvas.getWidth()); }
    unsigned int y_start(Renderer& renderer) const { return y; }
    unsigned int y_end(Renderer& renderer) const { return std::min(y + height, renderer.canvas.getHeight()); }
};

class ThreadPool {
private:
    bool stopThreadPool = false;
    std::condition_variable cv;        // Condition Variable for Signaling Changes
    std::mutex mtx;                    // Mutex for Synchronization
    std::queue<void()> jobQueue;
    std::vector<std::thread> slaves;   // Vector of threads (worker threads/slaves)
public:
    ThreadPool(size_t cpu = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < cpu; ++i) {
            slaves.emplace_back([this] {
                while (true) {
                    std::function<void()> job;
                    {
                        // Safe syncronization for shared data
                        std::unique_lock<std::mutex> lock(mtx);

                        // Wait until a job or thread pool termination
                        cv.wait(lock, [this]() -> bool { return stopThreadPool || !jobQueue.empty(); });
                        if (stopThreadPool && jobQueue.empty()) return;

                        // Obtain the next job
                        job = std::move(jobQueue.front());
                        jobQueue.pop();
                    }
                    job();
                }
            });
        }
    }

    ~ThreadPool() {
        // Safe update to stop thread pool
        {
            std::unique_lock<std::mutex> lock(mtx);
            stopThreadPool = true;
        }
        cv.notify_all();

        // Make sure the workers/slaves finished their job
        for (auto& slave : slaves)
            slave.join();
    }

    void enqueue(std::function<void()> job) {
        // Safe update to enqueue job to the job queue
        {
            std::unique_lock<std::mutex> lock(mtx);
            jobQueue.emplace(std::move(job));
        }
        cv.notify_all();
    }
};

// --- DRAFT IDEAS ---
// Multithreaded Approach - Render Small Chunks of Tiles, e.g. 64x64, instead of the whole canvas
// Note: All the optimisations done on the sequential function will be assumed to be applied here
// Input Variables:
// - renderer: Renderer object for drawing
// - tile: Tile struct for tiling/bining
// - L: Light object for shading calculations
// - ka, kd: Ambient and diffuse lighting coefficients
//void drawChunk(Renderer& renderer, Tile& tile, Light& L, float ka, float kd) {
//    unsigned int startX = tile.x_start(renderer);
//    unsigned int endX = tile.x_end(renderer);
//    unsigned int startY = tile.y_start(renderer);
//    unsigned int endY = tile.y_end(renderer);
//
//    // Skip very small triangles
//    if (area < 1.f) return;
//
//    // Backface Culling
//    if (signedArea < 0.f) return;
//
//    // Invert Area
//    area = 1.f / area;
//
//    // Iterate over the bounding box and check each pixel
//    for (int y = startY; y < endY; y++) {
//        for (int x = startX; x < endX; x++) {
//            // Check if the pixel lies inside the triangle
//            float alpha, beta, gamma;
//            if (getCoordinates(vec2D((float)x, (float)y), alpha, beta, gamma, area)) {
//                // Interpolate depth
//                float depth = interpolate(beta, gamma, alpha, v[0].p[2], v[1].p[2], v[2].p[2]);
//
//                // Perform Z-buffer test and then apply shading
//                if (renderer.zbuffer(x, y) > depth && depth > 0.001f) {
//                    // Update the buffer
//                    renderer.zbuffer(x, y) = depth;
//
//                    // Interpolate color
//                    colour c = interpolate(beta, gamma, alpha, v[0].rgb, v[1].rgb, v[2].rgb);
//                    c.clampColour();
//
//                    // Interpolate normals
//                    vec4 normal = interpolate(beta, gamma, alpha, v[0].normal, v[1].normal, v[2].normal);
//                    normal.normalise();
//
//                    float dot = std::max(vec4::dot(L.omega_i, normal), 0.f);
//                    colour a = (c * kd) * (L.L * dot) + (L.ambient * ka);  // using kd instead of ka for ambient
//
//                    // typical shader end
//                    unsigned char r, g, b;
//                    a.toRGB(r, g, b);
//                    renderer.canvas.draw(x, y, r, g, b);
//                }
//            }
//        }
//    }
//}
//void renderMultithread(Renderer& renderer) {
//    // Idea behind
//    // Warwick issued machine has 16 cores/22 threads & rasterizer screen resolution is 1024x768
//    // We have to partition screen such that a thread works with the partitioned region, per say 64x64
//    // But two threads may try to access the same region... so will have to use mutex etc. to prevent sync issues & race conds.
//    // We may also use ThreadPooling too... if time allows do both!
//
//    unsigned int tile_size = 64;
//    unsigned int tile_x = renderer.canvas.getWidth() / tile_size;
//    unsigned int tile_y = renderer.canvas.getHeight() / tile_size;
//    unsigned int tile_count = tile_x * tile_y;
//
//    // Lamda Expression for work function that implicit captures are captured by reference, with tile_index declarator 
//    auto workFunction = [&](unsigned int tile_index) {
//        unsigned int index_x = tile_index % tile_x;
//        unsigned int index_y = tile_index * tile_x;
//
//        Tile tile;
//        tile.x = index_x * tile_size;
//        tile.y = index_y * tile_size;
//        tile.width = tile_size;
//        tile.height = tile_size;
//
//        // Draw Function Here; Based on the tile struct
//    };
//}