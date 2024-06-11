#include <HrothCore.hpp>

#include "Panels/InfoPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ViewportPanel.hpp"

namespace HrothCore
{
    class SandboxClient : public IClient
    {
        public:
            SandboxClient()
                : m_EditorCamera(&m_EditorCameraPositioner)
            {
            }
            virtual ~SandboxClient() = default;

            void Init() override
            {
                HC_LOG_INFO("SandboxClient::Init");
                ImGuiLayer::UseDocking(true);
                ImGuiLayer::RegisterPanel<InfoPanel>();
                ImGuiLayer::RegisterPanel<SceneHierarchyPanel>(Engine::Get().GetScenePtr());
                ImGuiLayer::RegisterPanel<ViewportPanel>(Engine::Get().GetScenePtr(), &m_EditorCamera, &m_EditorCameraPositioner);

                Engine::Get().SetCameraPtr(&m_EditorCamera);
            }

            void Shutdown() override
            {
                HC_LOG_INFO("SandboxClient::Shutdown");
            }

            void Update(float dt) override
            {
            }
        
        private:
            Camera m_EditorCamera;
            CameraPositionerEditor m_EditorCameraPositioner;
    };

    Application *CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification specification;
        specification.Name = "Sandbox";
        specification.WorkingDirectory = ASSETS_DIR;
        specification.LogFile = "./Sandbox.log";
        specification.CommandLineArgs = args;

        std::shared_ptr<IClient> client = std::make_shared<SandboxClient>();

        return new Application(specification, client);
    }
}

//     #include <limits>

//     // grid

//     // [1] 12,24,28,31,38,77
//     // [2] 23,35,77,78
//     // [3] 5,6,15,33,51,62
//     // [4] 14,15,22,27,33,58,75,77
//     // [5] 5,8,44,47,72
//     // [6] 3,22,26,32,51,55,57,62,66,74
//     // [7] 51,55,66,78
//     // [8] 14,26,34,41,45,47,61

//     // [1] 6,21,44,45,51,77
//     // [2] 1,2,5,14,21,27,58,64
//     // [3] 5,44,71
//     // [4] 14,21,32,33,63,65,73
//     // [5] 2,21,46,57,61,68,72
//     // [6] 57,63,67
//     // [7] 1,11,14,21,23,42,64
//     // [8] 34,46,53,74

//     // [1] 1,4,17,22,32,35,55,62,76
//     // [2] 2,7,16,25,53,66,78
//     // [3] 1,2,4
//     // [4] 6,13,15,35,43,55,61,74
//     // [5] 44,47,53,62,67
//     // [6] 21
//     // [7] 7,12,22,34,61,63
//     // [8] 18,42,44,72,73,74

//     // [1] 1,5,8,15,24,33,36,42,73
//     // [2] 37,46,51,57,64
//     // [3] 7,42,48,52,53
//     // [4] 18,32,35,42,51
//     // [5] 17,33,42,47,61,64,68
//     // [6] 33,53
//     // [7] 17,27,76,78
//     // [8] 16,28,37,43,46,62

//     // [1] 17,18,25,26,34,64,67,72
//     // [2] 15,48,57
//     // [3] 4,34,51,74,77
//     // [4] 32,58
//     // [5] 18,38,43,65,76
//     // [6] 64,68
//     // [7] 16,17,24,68
//     // [8] 6,17,18,24,33,47,61,67,68,71,73,78

//     // [1] 5,31,45,55
//     // [2] 11,13,53,63,68
//     // [3] 6,7,11,17,25,47,63,65,74
//     // [4] 42,45,51,53,54,68,75
//     // [5] 4,48,68
//     // [6] 2,14,27,37,56,77
//     // [7] 47,72,73,78
//     // [8] 1,6,32,34,46,47,48,74

//     // [1] 4,44,53,57,67
//     // [2] 5,14,32,42,62,72
//     // [3] 1,5,7,11,27,57,65,67
//     // [4] 2,22,33,38,57,64,73
//     // [5] 1,4,33,34,37,45,62
//     // [6] 23,48,65
//     // [7] 21,25,33,35,65
//     // [8] 41,48,64

//     // [1] 14,66,72,73
//     // [2] 5,7,14,36,43
//     // [3] 7,11,25,37,47,64,71,75
//     // [4] 7,13,51
//     // [5] 2,17,27,68
//     // [6] 15,22,34,45,65
//     // [7] 3,12,13,14,44,46,68,72,76
//     // [8] 21,57,65,78

//     int preComputedGrid[8][8][16] =
//     {
//         {
//             {12,24,28,31,38,77},
//             {23,35,77,78},
//             {5,6,15,33,51,62},
//             {14,15,22,27,33,58,75,77},
//             {5,8,44,47,72},
//             {3,22,26,32,51,55,57,62,66,74},
//             {51,55,66,78},
//             {14,26,34,41,45,47,61}
//         },
//         {
//             {6,21,44,45,51,77},
//             {1,2,5,14,21,27,58,64},
//             {5,44,71},
//             {14,21,32,33,63,65,73},
//             {2,21,46,57,61,68,72},
//             {57,63,67},
//             {1,11,14,21,23,42,64},
//             {34,46,53,74}
//         },
//         {
//             {1,4,17,22,32,35,55,62,76},
//             {2,7,16,25,53,66,78},
//             {1,2,4},
//             {6,13,15,35,43,55,61,74},
//             {44,47,53,62,67},
//             {21},
//             {7,12,22,34,61,63},
//             {18,42,44,72,73,74}
//         },
//         {
//             {1,5,8,15,24,33,36,42,73},
//             {37,46,51,57,64},
//             {7,42,48,52,53},
//             {18,32,35,42,51},
//             {17,33,42,47,61,64,68},
//             {33,53},
//             {17,27,76,78},
//             {16,28,37,43,46,62}
//         },
//         {
//             {17,18,25,26,34,64,67,72},
//             {15,48,57},
//             {4,34,51,74,77},
//             {32,58},
//             {18,38,43,65,76},
//             {64,68},
//             {16,17,24,68},
//             {6,17,18,24,33,47,61,67,68,71,73,78}
//         },
//         {
//             {5,31,45,55},
//             {11,13,53,63,68},
//             {6,7,11,17,25,47,63,65,74},
//             {42,45,51,53,54,68,75},
//             {4,48,68},
//             {2,14,27,37,56,77},
//             {47,72,73,78},
//             {1,6,32,34,46,47,48,74}
//         },
//         {
//             {4,44,53,57,67},
//             {5,14,32,42,62,72},
//             {1,5,7,11,27,57,65,67},
//             {2,22,33,38,57,64,73},
//             {1,4,33,34,37,45,62},
//             {23,48,65},
//             {21,25,33,35,65},
//             {41,48,64}
//         },
//         {
//             {14,66,72,73},
//             {5,7,14,36,43},
//             {7,11,25,37,47,64,71,75},
//             {7,13,51},
//             {2,17,27,68},
//             {15,22,34,45,65},
//             {3,12,13,14,44,46,68,72,76},
//             {21,57,65,78}
//         }
//     };

//     uint64_t grid[8][8] = {0};
//     uint64_t max = std::numeric_limits<uint64_t>::max();

//     void recursflip(uint64_t input, int n, uint64_t &flipping)
//     {
//         if (input == max)
//         {
//             std::cout << "Flipping: " << flipping << std::endl;
//             return;
//         }
//         if (n == 64)
//         {
//             return;
//         }

//         recursflip(input, n + 1, flipping);
//         uint64_t newInput = input ^ grid[n / 8][n % 8];
//         uint64_t newFlipping = flipping | ((uint64_t)1 << n);
//         recursflip(newInput, n + 1, flipping);
//     }

//     void convertComputedGrid()
//     {
//         int result[8][8][16];
//         for (int i = 0; i < 8; i++)
//         {
//             for (int j = 0; j < 8; j++)
//             {
//                 for (int k = 0; k < 16; k++)
//                 {
//                     result[i][j][k] = -1;
//                     // convert output from 10 20 30.. to 8 16 24..
//                     if (preComputedGrid[i][j][k])
//                     {
//                         int a = preComputedGrid[i][j][k];
//                         int dizaine = a / 10;
//                         int unite = a % 10;
//                         result[i][j][k] = (dizaine * 8) + (unite - 1);
//                     }
//                 }
//             }
//         }

//         for (int i = 0; i < 8; i++)
//         {
//             for (int j = 0; j < 8; j++)
//             {
//                 for (int k = 0; k < 16; k++)
//                 {
//                     if (result[i][j][k] != -1)
//                     {
//                         int a = result[i][j][k];
//                         grid[a / 8][a % 8] += (uint64_t)1 << (63 - (i * 8 + j));
//                     }
//                 }
//             }
//         }
//     }

//     bool gaussianElimination(std::vector<std::vector<int>>& A, std::vector<int>& b, std::vector<int>& x)
//     {
//         int n = A.size();
//         int m = A[0].size();

//         // Augment A with b to form an augmented matrix
//         for (int i = 0; i < n; ++i) {
//             A[i].push_back(b[i]);
//         }

//         // Gaussian elimination
//         for (int col = 0; col < m; ++col) {
//             int pivot = -1;
//             for (int row = col; row < n; ++row) {
//                 if (A[row][col] == 1) {
//                     pivot = row;
//                     break;
//                 }
//             }

//             if (pivot == -1) continue;

//             swap(A[col], A[pivot]);

//             for (int row = 0; row < n; ++row) {
//                 if (row != col && A[row][col] == 1) {
//                     for (int j = col; j <= m; ++j) {
//                         A[row][j] ^= A[col][j];
//                     }
//                 }
//             }
//         }

//         // Back-substitution
//         x.assign(m, 0);
//         for (int row = 0; row < n; ++row) {
//             int sum = 0;
//             int pivot = -1;
//             for (int col = 0; col < m; ++col) {
//                 sum += A[row][col];
//                 if (A[row][col] == 1) pivot = col;
//             }
//             if (sum == 0 && A[row][m] == 1) return false; // No solution
//             if (pivot != -1) x[pivot] = A[row][m];
//         }

//         return true;
//     }

//     void fillA(std::vector<std::vector<int>> &A)
//     {
//         int result[8][8][16];
//         for (int i = 0; i < 8; i++)
//         {
//             for (int j = 0; j < 8; j++)
//             {
//                 for (int k = 0; k < 16; k++)
//                 {
//                     result[i][j][k] = -1;
//                     // convert output from 10 20 30.. to 8 16 24..
//                     if (preComputedGrid[i][j][k])
//                     {
//                         int a = preComputedGrid[i][j][k];
//                         int dizaine = a / 10;
//                         int unite = a % 10;
//                         result[i][j][k] = (dizaine * 8) + (unite - 1);
//                     }
//                 }
//             }
//         }

//         for (int i = 0; i < 8; i++)
//         {
//             for (int j = 0; j < 8; j++)
//             {
//                 for (int k = 0; k < 16; k++)
//                 {
//                     if (result[i][j][k] != -1)
//                     {
//                         int a = result[i][j][k];
//                         A[a][i * 8 + j] = 1;
//                     }
//                 }
//             }
//         }
//     }

//     void maintest() {
//         // Example grid state (64 cells)

//         /*
//         11001001
//         01100101
//         11011010
//         11001110
//         11100111
//         11010110
//         00000110
//         01101010
//         */
//         std::vector<int> b = 
//         {
//             1, 1, 0, 0, 1, 0, 0, 1,
//             0, 1, 1, 0, 0, 1, 0, 1,
//             1, 1, 0, 1, 1, 0, 1, 0,
//             1, 1, 0, 0, 1, 1, 1, 0,
//             1, 1, 1, 0, 0, 1, 1, 1,
//             1, 1, 0, 1, 0, 1, 1, 0,
//             0, 0, 0, 0, 0, 1, 1, 0,
//             0, 1, 1, 0, 1, 0, 1, 0
//         };
//         // Example flip operations matrix (64x64)
//         std::vector<std::vector<int>> A(64, std::vector<int>(64, 0));

//         fillA(A);

//         // Fill A and b based on your specific problem

//         std::vector<int> x; // Solution std::vector

//         if (gaussianElimination(A, b, x)) {
//             std::cout << "Solution found:" << std::endl;
//             for (int i = 0; i < 64; ++i) {
//                 if (x[i] == 1) {
//                     std::cout << "Flip cell " << i / 8 << "," << i % 8 << std::endl;
//                 }
//             }
//         } else {
//             std::cout << "No solution exists." << std::endl;
//         }

//         abort();
//     }

//     // void maintest()
//     // {
//     //     uint64_t input = 4277061146352132219;
//     //     uint64_t flipping = 0;
        
//     //     recursflip(input, 0, flipping);

//     //     abort();
//     // }

// }

