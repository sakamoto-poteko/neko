/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/

#include <iostream>
#include <cstdio>

#include <floatfann.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "chise trained_data" << std::endl;
        return 1;
    }

    const int tests = 15;
    fann_type* calc_out[tests];
    fann_type input[tests][10] = {
        // dingyang x5
        { 47.41304, 13.46296, 3.52174, 17.47826, 7, 1.80000, 1.70370, 10.50000, 7.1728, 8.8043 },
        { 33.49367, 11.38064, 2.94304, 9.50000, 2, 2.39474, 1.68984, 9.25000, 6.3462, 5.3196 },
        { 33.03571, 11.85897, 2.78571, 10.03571, 6, 2.85714, 2.15385, 14.60000, 6.1154, 5.3214 },
        { 32.92000, 11.71530, 2.81000, 10.25000, 2, 2.12000, 1.49254, 12.92308, 6.4840, 5.8700,  },
        { 47.69880, 13.42034, 3.55422, 12.75904, 13, 3.25000, 2.02439, 14.30769, 7.5492, 7.2651,  },
        // limiaomiao x3
        { 25.48276, 10.55714, 2.41379, 10.06897, 4, 4.00000, 3.62500, 12.87500, 5.4429, 5.1724,  },
        { 29.21739, 10.33846, 2.82609, 11.56522, 8, 4.00000, 2.87500, 9.00000, 5.4462, 5.7391,  },
        { 34.07895, 10.52846, 3.23684, 10.26316, 9, 3.33333, 2.00000, 8.60000, 5.4146, 5.1579,  },
        // suji x2
        { 16.09975, 9.09296, 1.77057, 7.61347, 1, 2.39007, 1.86512, 9.52273, 5.0831, 4.3267,  },
        { 26.50000, 9.35294, 2.83333, 10.61111, 3, 3.50000, 1.28571, 5.50000, 5.3529, 6.2222,  },
        // wanglu x5
        { 25.25225, 9.69896, 2.60360, 8.70270, 8, 3.05000, 4.82609, 10.00000, 5.0865, 4.5225,  },
        { 25.96296, 9.22368, 2.81481, 9.24691, 4, 3.31818, 3.85714, 8.04167, 4.8509, 4.6790,  },
        { 27.94286, 9.78000, 2.85714, 10.58571, 9, 2.71429, 4.37500, 8.20000, 5.3350, 5.7429,  },
        { 21.76682, 8.80944, 2.47085, 8.65919, 6, 3.29412, 5.30952, 8.71739, 4.8584, 4.7578,  },
        { 22.92523, 9.08519, 2.52336, 8.54206, 7, 3.27778, 4.86364, 8.54167, 5.0259, 4.8037,  },
    };
    const char* author[tests] = {
        "Dingyang",
        "Dingyang",
        "Dingyang",
        "Dingyang",
        "Dingyang",
        "Limiaomiao",
        "Limiaomiao",
        "Limiaomiao",
        "Suji",
        "Suji",
        "Wanglu",
        "Wanglu",
        "Wanglu",
        "Wanglu",
        "Wanglu",
    };

    fann* ann = fann_create_from_file(argv[1]);

    for (int i = 0; i < tests; ++i) {
        calc_out[i] = fann_run(ann, input[i]);

        fann_type val = *(calc_out[i]);

        std::printf("Test %d: %-12.10f, %s, ", i, val, author[i]);

        if (val > 0.95f) {
            std::printf("Almost Certain\n");
        } else {
            if (val > 0.5) {
                std::printf("Probable\n");
            } else {
                if (val > 0.10) {
                    std::printf("Probably Not\n");
                } else {
                    std::printf("Almost Certainly Not\n");
                }
            }
        }
    }

    return 0;
}
