#include <random>
#include <chrono>
#include <format>
#include <iostream>

#include "pds_Bridson.hpp"

struct point2d {
	float x, y;

	point2d() = delete;
	point2d(float x_, float y_) : x(x_), y(y_) {}
};

void bench(const blue_noise::bridson_2d::config& conf) {
    std::random_device rd;
    std::mt19937 rne(rd());

    const auto start = std::chrono::steady_clock::now();
    auto points = blue_noise::bridson_2d::poisson_disc_sampling<point2d>(conf, rne);
    const auto end = std::chrono::steady_clock::now();

    std::cout << std::format("generated {:6} points in {:3}ms\n",
                             points.size(),
                             std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}

int main(int argc, char* argv[]) {
    blue_noise::bridson_2d::config conf1k = {
            .w = 1600,
            .h = 900,
            .min_dist = 30.1f,
            .k_max_attempts = 30
    };
    bench(conf1k);

    blue_noise::bridson_2d::config conf10k = {
        .w = 1600,
        .h = 900,
        .min_dist = 9.48f,
        .k_max_attempts = 30
    };
    bench(conf10k);

    blue_noise::bridson_2d::config conf100k = {
            .w = 1600,
            .h = 900,
            .min_dist = 2.99f,
            .k_max_attempts = 30
    };
    bench(conf100k);

	return 0;
}