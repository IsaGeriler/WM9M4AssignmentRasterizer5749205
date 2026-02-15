# WM9M4AssignmentRasterizer5749205
## Scene One

![SceneOneRasterizer](https://github.com/user-attachments/assets/549b0258-30fd-46ca-93db-9c35bf76e352)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 1 (ms) | Improvement (%) | Scene 1 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 4871.46 | 2734.35 | AddAfterSleep | 1620.09 | AddAfterSleep |
| 2 | 4867.98 | 2748.80 | AddAfterSleep | 1539.91 | AddAfterSleep |
| 3 | 4880.26 | 2740.37 | AddAfterSleep | 1543.76 | AddAfterSleep |
| 4 | 4898.30 | 2709.12 | AddAfterSleep | 1530.03 | AddAfterSleep |
| 5 | 4895.61 | 2714.70 | AddAfterSleep | 1533.52 | AddAfterSleep |
| 6 | 4952.78 | 2718.36 | AddAfterSleep | 1536.74 | AddAfterSleep |
| 7 | 5026.94 | 2690.74 | AddAfterSleep | 1542.29 | AddAfterSleep |
| 8 | 5020.52 | 2696.04 | AddAfterSleep | 1525.77 | AddAfterSleep |
| 9 | 5055.14 | 2691.37 | AddAfterSleep | 1538.62 | AddAfterSleep |
| 10 | 4991.21 | 2679.63 | AddAfterSleep | 1541.78 | AddAfterSleep |
| **Average** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** |

## Scene Two

![SceneTwoRasterizer](https://github.com/user-attachments/assets/720040c3-6398-421f-8289-795ef4b8a22d)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 2 (ms) | Improvement (%) | Scene 2 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 1949.27 | 1059.08 | AddAfterSleep | 384.986 | AddAfterSleep |
| 2 | 1929.76 | 1041.73 | AddAfterSleep | 375.543 | AddAfterSleep |
| 3 | 1931.39 | 1064.11 | AddAfterSleep | 379.647 | AddAfterSleep |
| 4 | 1912.26 | 1077.40 | AddAfterSleep | 373.048 | AddAfterSleep |
| 5 | 1930.76 | 1064.25 | AddAfterSleep | 370.201 | AddAfterSleep |
| 6 | 1922.41 | 1072.35 | AddAfterSleep | 371.621 | AddAfterSleep |
| 7 | 1920.44 | 1072.56 | AddAfterSleep | 371.630 | AddAfterSleep |
| 8 | 1899.17 | 1079.56 | AddAfterSleep | 375.807 | AddAfterSleep |
| 9 | 1921.60 | 1089.46 | AddAfterSleep | 376.506 | AddAfterSleep |
| 10 | 1918.74 | 1074.97 | AddAfterSleep | 376.31 | AddAfterSleep |
| **Average** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** |

## Scene Three
<img width="1919" height="1199" alt="Screenshot 2026-02-14 030617" src="https://github.com/user-attachments/assets/1206b170-9314-4f24-b141-53c1229cd0d1" />

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser
*Helix Benchmark - High density geometry stress test (vortex warp, consisting of cubes and spheres, total of 1600 meshes, where every 5th object is a sphere)*

| Iteration | Base Rasterizer (ms) | Optimised Scene 3 (ms) | Improvement (%) | Scene 3 MT - 6 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 85651.10 | 43837.30 | AddAfterSleep | 15747.60 | AddAfterSleep |
| 2 | 84637.10 | 43298.80 | AddAfterSleep | 15679.80 | AddAfterSleep |
| 3 | 85277.80 | 43805.00 | AddAfterSleep | 15739.70 | AddAfterSleep |
| 4 | 87442.80 | 43533.40 | AddAfterSleep | 15715.40 | AddAfterSleep |
| 5 | 86084.20 | 43707.30 | AddAfterSleep | 15609.40 | AddAfterSleep |
| 6 | 87019.30 | 44223.80 | AddAfterSleep | 15723.40 | AddAfterSleep |
| 7 | 87043.40 | 43783.90 | AddAfterSleep | 15881.60 | AddAfterSleep |
| 8 | 87075.20 | 43766.30 | AddAfterSleep | 15542.20 | AddAfterSleep |
| 9 | 87030.50 | 43867.60 | AddAfterSleep | 15784.80 | AddAfterSleep |
| 10 | 86662.90 | 44389.80 | AddAfterSleep | 15789.90 | AddAfterSleep |
| **Average** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** | **AddAfterSleep** |
