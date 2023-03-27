#include <random>
#include <chrono>
#include <format>
#include <iostream>
#include <numeric>

#include "pds_Bridson.hpp"

struct point2d {
	float x, y;

	point2d() = delete;
	point2d(float x_, float y_) : x(x_), y(y_) {}
};

const int runs = 10;
std::random_device rd;

void bench(const blue_noise::bridson_2d::config& conf) {
    std::vector<std::size_t> sample_counts;
    std::vector<std::chrono::milliseconds> durations;

    for (int i = 0; i < runs; ++i) {
        std::mt19937 rng(rd());

        const auto start = std::chrono::steady_clock::now();
        auto points = blue_noise::bridson_2d::poisson_disc_sampling<point2d>(conf, rng);
        const auto end = std::chrono::steady_clock::now();

        sample_counts.push_back(points.size());
        durations.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
    }

    const auto avg_count = std::reduce(sample_counts.begin(), sample_counts.end()) / float(runs);
    const auto avg_duration = std::reduce(durations.begin(), durations.end()) / float(runs);

    std::cout << std::format("generated avg of {:6.0f} points in ~{:3}\n", avg_count, avg_duration);
}

int main() {
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
}