//
// Created by Daniel Secrieru on 01/04/2023.
//
#include <random>
#include <chrono>
#include <format>
#include <iostream>
#include <numeric>

#define BN_PDS_GENERATOR
#include "pds_Bridson.hpp"

struct point2d {
    float x, y;

    point2d() = default;
    point2d(float x_, float y_) : x(x_), y(y_) {}
};

typedef blue_noise::bridson_2d::config<point2d> config_t;
using blue_noise::bridson_2d::event_type;

const int runs = 10;
std::random_device rd;

void bench(const config_t& conf, int expected) {
    std::vector<point2d> samples;
    samples.reserve(expected);

    std::vector<std::size_t> sample_counts;
    std::vector<std::chrono::milliseconds> durations;

    for (int i = 0; i < runs; ++i) {
        samples.clear();

        std::mt19937 rng(rd());

        const auto start = std::chrono::steady_clock::now();
        auto pds_gen = blue_noise::bridson_2d::poisson_disc_sampling(conf, rng);
        while(pds_gen) {
            const auto& event = pds_gen();
            switch (event.type) {
                case event_type::sample_generated:
                    samples.push_back(std::get<point2d>(event.data));
                    break;
                default:
                    break;
            }
        }
        const auto end = std::chrono::steady_clock::now();

        sample_counts.push_back(samples.size());
        durations.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
    }

    const auto avg_count = std::reduce(sample_counts.begin(), sample_counts.end()) / float(runs);
    const auto avg_duration = std::reduce(durations.begin(), durations.end()) / float(runs);

    std::cout << std::format("generated approx {:6.0f} points in ~{:3}\n", avg_count, avg_duration);
}

int main() {
    config_t conf1k = {
            .w = 1600,
            .h = 900,
            .min_dist = 30.1f,
            .k_max_attempts = 30
    };
    bench(conf1k, 1000);

    config_t conf10k = {
            .w = 1600,
            .h = 900,
            .min_dist = 9.48f,
            .k_max_attempts = 30
    };
    bench(conf10k, 10000);

    config_t conf100k = {
            .w = 1600,
            .h = 900,
            .min_dist = 2.99f,
            .k_max_attempts = 30
    };
    bench(conf100k, 100000);
}