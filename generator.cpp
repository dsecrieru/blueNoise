//
// Created by Daniel Secrieru on 01/04/2023.
//
#include <random>
#include <iostream>

#define BN_PDS_GENERATOR
#include "pds_Bridson.hpp"

struct point2d {
    float x, y;

    point2d() = default;
    point2d(float x_, float y_) : x(x_), y(y_) {}
};

std::random_device rd;

int main() {
    typedef blue_noise::bridson_2d::config<point2d> config_t;
    config_t conf = {
            .w = 100,
            .h = 100,
            .min_dist = 10.0f,
            .k_max_attempts = 30
    };

    using blue_noise::bridson_2d::event_type;

    std::mt19937 rng(rd());

    auto pds_gen = blue_noise::bridson_2d::poisson_disc_sampling(conf, rng);
    while(pds_gen) {
        const auto& event = pds_gen();
        switch (event.type) {
            case event_type::sample_generated: {
                const auto&p = std::get<point2d>(event.data);
                std::cout << p.x << " " << p.y << std::endl;
            }
        }
    }
}