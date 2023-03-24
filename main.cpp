#include <random>
#include <chrono>
#include <iostream>

#include "pds_Bridson.hpp"

struct point2d {
	float x, y;

	point2d() = delete;
	point2d(float x_, float y_) : x(x_), y(y_) {}
};

int main(int argc, char* argv[]) {
	std::mt19937 rne;

    blue_noise::bridson2d::config conf = {
        .w = 1600,
        .h = 900,
        .min_dist = 10.0f,
        .k_max_attempts = 30
    };

    const auto start = std::chrono::steady_clock::now();
    auto points = blue_noise::bridson2d::poisson_disc_sampling<point2d>(conf, rne);
    const auto end = std::chrono::steady_clock::now();

    std::cout << "generated: " << points.size()
        << " in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
        << std::endl;

	return 0;
}