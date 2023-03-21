#include <chrono>
#include <iostream>

#include "pds_Bridson.hpp"

template<typename T>
struct point2d {
	T x, y;

	point2d() = default;
	point2d(T x_, T y_) : x(x_), y(y_) {}
};

int main(int argc, char* argv[]) {
	std::mt19937 rne;

    const auto start = std::chrono::steady_clock::now();
    auto bridsonSamples = blue_noise::pds_Bridson_sampling_2d<point2d<float>, std::mt19937>(
            1600, 900,
            9.6,
            30,
            rne
    );
    const auto end = std::chrono::steady_clock::now();
    std::cout << "generated: " << bridsonSamples.size()
        << " in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
        << std::endl;

	return 0;
}