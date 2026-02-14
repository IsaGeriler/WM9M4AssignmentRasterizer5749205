# WM9M4AssignmentRasterizer5749205
## Scene One

![SceneOneRasterizer](https://github.com/user-attachments/assets/549b0258-30fd-46ca-93db-9c35bf76e352)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 1 (ms) | Improvement (%) | Scene 1 MT - 8 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 5515.64 | 2723.09 | ~50.63% | AddAfterSleep | AddAfterSleep |
| 2 | 5625.70 | 2697.57 | ~52.05% | AddAfterSleep | AddAfterSleep |
| 3 | 5614.01 | 2660.69 | ~52.61% | AddAfterSleep | AddAfterSleep |
| 4 | 5703.97 | 2667.99 | ~53.23% | AddAfterSleep | AddAfterSleep |
| 5 | 5648.51 | 2693.08 | ~52.32% | AddAfterSleep | AddAfterSleep |
| 6 | 5695.27 | 2675.17 | ~53.03% | AddAfterSleep | AddAfterSleep |
| 7 | 5631.64 | 2676.00 | ~52.48% | AddAfterSleep | AddAfterSleep |
| 8 | 5637.51 | 2665.40 | ~52.72% | AddAfterSleep | AddAfterSleep |
| 9 | 5594.10 | 2680.03 | ~52.09% | AddAfterSleep | AddAfterSleep |
| 10 | 5449.28 | 2675.08 | ~50.91% | AddAfterSleep | AddAfterSleep |
| 11 | 5535.58 | 2675.51 | ~51.67% | AddAfterSleep | AddAfterSleep |
| 12 | 5547.96 | 2676.49 | ~51.76% | AddAfterSleep | AddAfterSleep |
| 13 | 5624.83 | 2686.54 | ~52.24% | AddAfterSleep | AddAfterSleep |
| 14 | 5654.42 | 2663.25 | ~52.90% | AddAfterSleep | AddAfterSleep |
| 15 | 5661.52 | 2675.75 | ~52.74% | AddAfterSleep | AddAfterSleep |
| **Average** | **5609.33** | **2679.44** | **~52.23%** | **AddAfterSleep** | **AddAfterSleep** |

## Scene Two

![SceneTwoRasterizer](https://github.com/user-attachments/assets/720040c3-6398-421f-8289-795ef4b8a22d)

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser

| Iteration | Base Rasterizer (ms) | Optimised Scene 2 (ms) | Improvement (%) | Scene 2 MT - 8 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 1995.87 | 1056.05 | ~47.09% | AddAfterSleep | AddAfterSleep |
| 2 | 2138.83 | 1092.39 | ~48.93% | AddAfterSleep | AddAfterSleep |
| 3 | 2105.74 | 1113.55 | ~47.12% | AddAfterSleep | AddAfterSleep |
| 4 | 2103.47 | 1139.45 | ~45.83% | AddAfterSleep | AddAfterSleep |
| 5 | 2120.90 | 1104.10 | ~47.94% | AddAfterSleep | AddAfterSleep |
| 6 | 2113.35 | 1174.28 | ~44.43% | AddAfterSleep | AddAfterSleep |
| 7 | 2098.33 | 1179.57 | ~43.79% | AddAfterSleep | AddAfterSleep |
| 8 | 2115.13 | 1114.82 | ~47.29% | AddAfterSleep | AddAfterSleep |
| 9 | 2131.63 | 1121.07 | ~47.41% | AddAfterSleep | AddAfterSleep |
| 10 | 2116.24 | 1166.73 | ~44.87% | AddAfterSleep | AddAfterSleep |
| 11 | 2092.96 | 1177.93 | ~43.72% | AddAfterSleep | AddAfterSleep |
| 12 | 2103.61 | 1158.31 | ~44.94% | AddAfterSleep | AddAfterSleep |
| 13 | 2134.69 | 1130.79 | ~47.03% | AddAfterSleep | AddAfterSleep |
| 14 | 2089.59 | 1138.93 | ~45.49% | AddAfterSleep | AddAfterSleep |
| 15 | 2083.39 | 1136.72 | ~45.44% | AddAfterSleep | AddAfterSleep |
| **Average** | **2109.58** | **1133.65** | **~46.26%** | **AddAfterSleep** | **AddAfterSleep** |

## Scene Three
<img width="1919" height="1199" alt="Screenshot 2026-02-14 030617" src="https://github.com/user-attachments/assets/1206b170-9314-4f24-b141-53c1229cd0d1" />

### Performance Comparison: Base Rasteriser vs. Optimised Rasteriser
*Helix Benchmark - High density geometry stress test (vortex warp, consisting of cubes and spheres, total of 1600 meshes, where every 5th object is a sphere)*

| Iteration | Base Rasterizer (ms) | Optimised Scene 3 (ms) | Improvement (%) | Scene 3 MT - 8 Threads (ms) | Improvement (%) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | 89154.10 | 47523.10 | ~46.70% | AddAfterSleep | AddAfterSleep |
| 2 | 89405.30 | 46234.40 | ~48.29% | AddAfterSleep | AddAfterSleep |
| 3 | 89405.50 | 46302.10 | ~48.21% | AddAfterSleep | AddAfterSleep |
| 4 | 87892.40 | 46311.70 | ~47.31% | AddAfterSleep | AddAfterSleep |
| 5 | 87780.30 | 46449.20 | ~47.08% | AddAfterSleep | AddAfterSleep |
| 6 | 87546.20 | 47006.60 | ~46.31% | AddAfterSleep | AddAfterSleep |
| 7 | 87593.70 | 48569.40 | ~44.55% | AddAfterSleep | AddAfterSleep |
| 8 | 85141.60 | 46538.60 | ~45.34% | AddAfterSleep | AddAfterSleep |
| 9 | 90429.20 | 45809.50 | ~49.34% | AddAfterSleep | AddAfterSleep |
| 10 | 89372.80 | 46374.50 | ~48.11% | AddAfterSleep | AddAfterSleep |
| 11 | 86920.40 | 45187.60 | ~48.01% | AddAfterSleep | AddAfterSleep |
| 12 | 87006.40 | 48036.80 | ~44.79% | AddAfterSleep | AddAfterSleep |
| 13 | 87270.50 | 47571.10 | ~45.49% | AddAfterSleep | AddAfterSleep |
| 14 | 90809.60 | 45215.00 | ~50.21% | AddAfterSleep | AddAfterSleep |
| 15 | 83413.90 | 45294.90 | ~45.70% | AddAfterSleep | AddAfterSleep |
| **Average** | **87942.79** | **46561.63** | **~47.05%** | **AddAfterSleep** | **AddAfterSleep** |
