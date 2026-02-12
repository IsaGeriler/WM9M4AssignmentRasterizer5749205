#pragma once

#include <iostream>
#include <numbers>
#include <vector>

#include "colour.h"
#include "matrix.h"
#include "vec4.h"

// Represents a vertex in a 3D mesh, including its position, normal, and color
struct Vertex {
    vec4 p;         // Position of the vertex in 3D space
    vec4 normal;    // Normal vector for the vertex
    colour rgb;     // Color of the vertex
};

// Stores indices of vertices that form a triangle in a mesh
struct triIndices {
    unsigned int v[3]; // Indices into the vertex array

    // Constructor to initialize the indices of a triangle
    triIndices(unsigned int v1, unsigned int v2, unsigned int v3) {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }
};

// Class representing a 3D mesh made up of vertices and triangles
class Mesh {
public:
    colour col;       // Uniform color for the mesh
    float kd;         // Diffuse reflection coefficient
    float ka;         // Ambient reflection coefficient
    matrix world;     // Transformation matrix for the mesh
    std::vector<Vertex> vertices;       // List of vertices in the mesh
    std::vector<triIndices> triangles;  // List of triangles in the mesh

    // Set the uniform color and reflection coefficients for the mesh
    // Input Variables:
    // - _c: Uniform color
    // - _ka: Ambient reflection coefficient
    // - _kd: Diffuse reflection coefficient
    void setColour(colour _c, float _ka, float _kd) {
        col = _c;
        ka = _ka;
        kd = _kd;
    }

    // Default constructor initializes default color and reflection coefficients
    Mesh() {
        col.set(1.0f, 1.0f, 1.0f);
        ka = kd = 0.75f;
    }

    // Add a vertex and its normal to the mesh
    // Input Variables:
    // - vertex: Position of the vertex
    // - normal: Normal vector for the vertex
    void addVertex(const vec4& vertex, const vec4& normal) {
        Vertex v = { vertex, normal, col };
        vertices.push_back(v);
    }

    // Add a triangle to the mesh
    // Input Variables:
    // - v1, v2, v3: Indices of the vertices forming the triangle
    void addTriangle(int v1, int v2, int v3) {
        triangles.emplace_back(v1, v2, v3);
    }

    // Display the vertices and triangles of the mesh
    void display() const {
        std::cout << "Vertices and Normals:\n";
        for (size_t i = 0; i < vertices.size(); ++i) {
            std::cout << i << ": Vertex (" << vertices[i].p[0] << ", " << vertices[i].p[1] << ", " << vertices[i].p[2] << ", " << vertices[i].p[3] << ")"
                << " Normal (" << vertices[i].normal[0] << ", " << vertices[i].normal[1] << ", " << vertices[i].normal[2] << ", " << vertices[i].normal[3] << ")\n";
        }

        std::cout << "\nTriangles:\n";
        for (const auto& t : triangles) {
            std::cout << "(" << t.v[0] << ", " << t.v[1] << ", " << t.v[2] << ")\n";
        }
    }

    // Create a rectangle mesh given two opposite corners
    // Input Variables:
    // - x1, y1: Coordinates of one corner
    // - x2, y2: Coordinates of the opposite corner
    // Returns a Mesh object representing the rectangle
    static Mesh makeRectangle(float x1, float y1, float x2, float y2) {
        Mesh mesh;
        mesh.vertices.clear();
        mesh.triangles.clear();

        // Define the four corners of the rectangle
        vec4 v1(x1, y1, 0);
        vec4 v2(x2, y1, 0);
        vec4 v3(x2, y2, 0);
        vec4 v4(x1, y2, 0);

        // Calculate the normal vector using the cross product of two edges
        vec4 edge1 = v2 - v1;
        vec4 edge2 = v4 - v1;
        vec4 normal = vec4::cross(edge1, edge2);
        normal.normalise();

        // Add vertices with the calculated normal
        mesh.addVertex(v1, normal);
        mesh.addVertex(v2, normal);
        mesh.addVertex(v3, normal);
        mesh.addVertex(v4, normal);

        // Add two triangles forming the rectangle
        mesh.addTriangle(0, 2, 1);
        mesh.addTriangle(0, 3, 2);

        return mesh;
    }

    // Generate a cube mesh
    // Input Variables:
    // - size: Length of one side of the cube
    // Returns a Mesh object representing the cube
    static Mesh makeCube(float size) {
        Mesh mesh;
        mesh.vertices.clear();
        mesh.triangles.clear();

        // Division is an expensive operation
        // float halfSize = size / 2.0f;
        float halfSize = size * 0.5f;

        // Define cube vertices (8 corners)
        vec4 positions[8] = {
            vec4(-halfSize, -halfSize, -halfSize),
            vec4(halfSize, -halfSize, -halfSize),
            vec4(halfSize, halfSize, -halfSize),
            vec4(-halfSize, halfSize, -halfSize),
            vec4(-halfSize, -halfSize, halfSize),
            vec4(halfSize, -halfSize, halfSize),
            vec4(halfSize, halfSize, halfSize),
            vec4(-halfSize, halfSize, halfSize)
        };

        // Define face normals
        vec4 normals[6] = {
            vec4(0, 0, -1, 0),
            vec4(0, 0, 1, 0),
            vec4(-1, 0, 0, 0),
            vec4(1, 0, 0, 0),
            vec4(0, -1, 0, 0),
            vec4(0, 1, 0, 0)
        };

        // Add vertices and triangles for each face
        int faceIndices[6][4] = {
            {1, 0, 3, 2},
            {4, 5, 6, 7},
            {3, 0, 4, 7},
            {5, 1, 2, 6},
            {0, 1, 5, 4},
            {2, 3, 7, 6}
        };

        for (int i = 0; i < 6; ++i) {
            int v0 = faceIndices[i][0];
            int v1 = faceIndices[i][1];
            int v2 = faceIndices[i][2];
            int v3 = faceIndices[i][3];

            // Add vertices with their normals
            mesh.addVertex(positions[v0], normals[i]);
            mesh.addVertex(positions[v1], normals[i]);
            mesh.addVertex(positions[v2], normals[i]);
            mesh.addVertex(positions[v3], normals[i]);

            // Add two triangles for the face
            int baseIndex = i * 4;
            mesh.addTriangle(baseIndex, baseIndex + 2, baseIndex + 1);
            mesh.addTriangle(baseIndex, baseIndex + 3, baseIndex + 2);
        }
        return mesh;
    }

    // Generate a sphere mesh
    // Input Variables:
    // - radius: Radius of the sphere
    // - latitudeDivisions: Number of divisions along the latitude
    // - longitudeDivisions: Number of divisions along the longitude
    // Returns a Mesh object representing the sphere
    static Mesh makeSphere(float radius, int latitudeDivisions, int longitudeDivisions) {
        // Mesh mesh;
        if (latitudeDivisions < 2 || longitudeDivisions < 3) {
            throw std::invalid_argument("Latitude divisions must be >= 2 and longitude divisions >= 3");
        }

        Mesh mesh;
        mesh.vertices.clear();
        mesh.triangles.clear();

        // Store pi, and inverse latitudeDivisions & longitudeDivisions
        float pi = std::numbers::pi_v<float>;
        float invLatitudeDivisions = 1.f / latitudeDivisions;
        float invLongitudeDivisions = 1.f / longitudeDivisions;

        // Create vertices
        for (int lat = 0; lat <= latitudeDivisions; ++lat) {
            // float theta = pi * lat / invLatitudeDivisions;
            float theta = pi * lat * invLatitudeDivisions;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int lon = 0; lon <= longitudeDivisions; ++lon) {
                // float phi = 2 * pi * lon / invLongitudeDivisions;
                float phi = 2 * pi * lon * invLongitudeDivisions;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                vec4 position(
                    radius * sinTheta * cosPhi,
                    radius * sinTheta * sinPhi,
                    radius * cosTheta,
                    1.0f
                );

                vec4 normal = position;
                normal.normalise();
                normal[3] = 0.f;

                mesh.addVertex(position, normal);
            }
        }

        // Create indices for triangles
        for (int lat = 0; lat < latitudeDivisions; ++lat) {
            for (int lon = 0; lon < longitudeDivisions; ++lon) {
                int v0 = lat * (longitudeDivisions + 1) + lon;
                int v1 = v0 + 1;
                int v2 = (lat + 1) * (longitudeDivisions + 1) + lon;
                int v3 = v2 + 1;

                mesh.addTriangle(v0, v1, v2);
                mesh.addTriangle(v1, v3, v2);
            }
        }
        return mesh;
    }

    // Optimisation - Vertex Caching to avoid redundant calculations (e.g. re-calculating same vertices)
    void vertexPreProcessing(std::vector<Vertex>& vertexCache, matrix& p, unsigned int width, unsigned int height) {
        // Allocate memory in the cache, according to the vertices size
        vertexCache.resize(vertices.size());
        #if OPT_RASTER_DISABLE_REDUNDANT_HALF_WIDTH_HEIGH_MUL
            float half_width = 0.5f * static_cast<float>(width);
            float half_height = 0.5f * static_cast<float>(height);
        #endif

        // Transform each vertex of the triangle
        for (unsigned int i = 0; i < vertices.size(); i++) {
            Vertex vertex;
            vertex.p = p * vertices[i].p;  // Apply transformations
            vertex.p.divideW();            // Perspective division to normalize coordinates

            // Transform normals into world space for accurate lighting
            // no need for perspective correction as no shearing or non-uniform scaling
            vertex.normal = world * vertices[i].normal;
            vertex.normal.normalise();

            // Map normalized device coordinates to screen space
            #if OPT_RASTER_DISABLE_REDUNDANT_HALF_WIDTH_HEIGH_MUL
                vertex.p[0] = (vertex.p[0] + 1.f) * half_width;
                vertex.p[1] = (vertex.p[1] + 1.f) * half_height;
            #else
                vertex.p[0] = (vertex.p[0] + 1.f) * 0.5f * static_cast<float>(width);
                vertex.p[1] = (vertex.p[1] + 1.f) * 0.5f * static_cast<float>(height);
            #endif
            vertex.p[1] = height - vertex.p[1]; // Invert y-axis

            // Copy vertex colours
            vertex.rgb = vertices[i].rgb;
            vertexCache[i] = vertex;
        }
    }
};