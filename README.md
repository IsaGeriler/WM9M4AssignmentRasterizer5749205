# WM9M4AssignmentRasterizer5749205
## Scene One

![SceneOneRasterizer](https://github.com/user-attachments/assets/549b0258-30fd-46ca-93db-9c35bf76e352)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 1 (ms) | Improvement (%) | Scene 1 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 4871.46 | 2734.35 | 43.87 | 1620.09 | 66.74 |
| 2 | 4867.98 | 2748.80 | 43.53 | 1539.91 | 68.37 |
| 3 | 4880.26 | 2740.37 | 43.85 | 1543.76 | 68.37 |
| 4 | 4898.30 | 2709.12 | 44.69 | 1530.03 | 68.76 |
| 5 | 4895.61 | 2714.70 | 44.55 | 1533.52 | 68.68 |
| 6 | 4952.78 | 2718.36 | 45.11 | 1536.74 | 68.97 |
| 7 | 5026.94 | 2690.74 | 46.47 | 1542.29 | 69.32 |
| 8 | 5020.52 | 2696.04 | 46.30 | 1525.77 | 69.61 |
| 9 | 5055.14 | 2691.37 | 46.76 | 1538.62 | 69.56 |
| 10 | 4991.21 | 2679.63 | 46.31 | 1541.78 | 69.11 |
| **Average** | **4946.02** | **2712.35** | **45.16** | **1545.25** | **68.76** |

## Scene Two

![SceneTwoRasterizer](https://github.com/user-attachments/assets/720040c3-6398-421f-8289-795ef4b8a22d)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 2 (ms) | Improvement (%) | Scene 2 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 1949.27 | 1059.08 | 45.67 | 384.99 | 80.25 |
| 2 | 1929.76 | 1041.73 | 46.02 | 375.54 | 80.54 |
| 3 | 1931.39 | 1064.11 | 44.90 | 379.65 | 80.34 |
| 4 | 1912.26 | 1077.40 | 43.66 | 373.05 | 80.49 |
| 5 | 1930.76 | 1064.25 | 44.88 | 370.20 | 80.83 |
| 6 | 1922.41 | 1072.35 | 44.22 | 371.62 | 80.67 |
| 7 | 1920.44 | 1072.56 | 44.15 | 371.63 | 80.65 |
| 8 | 1899.17 | 1079.56 | 43.16 | 375.81 | 80.21 |
| 9 | 1921.60 | 1089.46 | 43.30 | 376.51 | 80.41 |
| 10 | 1918.74 | 1074.97 | 43.98 | 376.31 | 80.39 |
| **Average** | **1923.58** | **1069.55** | **44.40** | **375.53** | **80.48** |

## Scene Three
<img width="1919" height="1199" alt="Screenshot 2026-02-14 030617" src="https://github.com/user-attachments/assets/1206b170-9314-4f24-b141-53c1229cd0d1" />

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser
*Helix Benchmark - High density geometry stress test (vortex warp, consisting of cubes and spheres, total of 1600 meshes, where every 5th object is a sphere)*

| Iteration | Base Rasterizer (ms) | Optimised Scene 3 (ms) | Improvement (%) | Scene 3 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 85651.10 | 43837.30 | 48.82 | 15747.60 | 81.61 |
| 2 | 84637.10 | 43298.80 | 48.84 | 15679.80 | 81.47 |
| 3 | 85277.80 | 43805.00 | 48.63 | 15739.70 | 81.54 |
| 4 | 87442.80 | 43533.40 | 50.22 | 15715.40 | 82.03 |
| 5 | 86084.20 | 43707.30 | 49.23 | 15609.40 | 81.87 |
| 6 | 87019.30 | 44223.80 | 49.18 | 15723.40 | 81.93 |
| 7 | 87043.40 | 43783.90 | 49.70 | 15881.60 | 81.75 |
| 8 | 87075.20 | 43766.30 | 49.74 | 15542.20 | 82.15 |
| 9 | 87030.50 | 43867.60 | 49.60 | 15784.80 | 81.86 |
| 10 | 86662.90 | 44389.80 | 48.78 | 15789.90 | 81.78 |
| **Average** | **86392.43** | **43821.32** | **49.28** | **15721.38** | **81.80** |
