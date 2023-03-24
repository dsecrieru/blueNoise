#ifndef pds_bridson_hpp
#define pds_bridson_hpp

#include <numbers>
#include <vector>
#include <stack>
#include <optional>
#include <algorithm>

namespace blue_noise::bridson2d {

typedef int32_t integral_t;
typedef float float_t;

struct config {
    float_t w = 1.0f;
    float_t h = 1.0f;
    float_t min_dist = 0.01f;
    integral_t k_max_attempts = 30;
};

/*
 * Implementation of Robert Bridson's Fast Poisson Disk Sampling for the 2-dimensional case.
 *
 * More details in the original paper:
 *     https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
 *
 */
template<typename point_t, typename random_engine_t>
    requires std::uniform_random_bit_generator<random_engine_t>
std::vector<point_t> poisson_disc_sampling(const config& conf, random_engine_t& rne) {
    const auto cell_size = conf.min_dist / std::numbers::sqrt2_v<float_t>;
    const auto grid_w = static_cast<integral_t>(std::ceil(conf.w / cell_size));
    const auto grid_h = static_cast<integral_t>(std::ceil(conf.h / cell_size));
    const auto min_dist_sq = conf.min_dist * conf.min_dist - std::numeric_limits<float_t>::epsilon();

    std::vector<point_t> ret_points;
    ret_points.reserve(grid_w * grid_h);

    std::vector<std::optional<point_t>> grid(grid_w * grid_h);

    auto grid_x = [=](const point_t& p) {
        return std::min(static_cast<integral_t>(p.x / cell_size), grid_w - 1);
    };
    auto grid_y = [=](const point_t& p) {
        return std::min(static_cast<integral_t>(p.y / cell_size), grid_h - 1);
    };
    auto set_cell = [cell_size, grid_w, &grid, &grid_x, &grid_y](const point_t& p) {
        grid[grid_y(p) * grid_w + grid_x(p)] = p;
    };
    const auto& get_cell = [grid_w, &grid](integral_t x, integral_t y) {
        return grid[y * grid_w + x];
    };

    auto dist_sq = [](const point_t& p1, const point_t& p2) {
        auto diff_x = p1.x - p2.x;
        auto diff_y = p1.y - p2.y;
        return diff_x * diff_x + diff_y * diff_y;
    };

    auto is_valid = [&](const point_t& p) {
        if (p.x < 0 || p.x >= conf.w || p.y < 0 || p.y >= conf.h)
            return false;

        auto x = grid_x(p);
        auto y = grid_y(p);

        if (get_cell(x, y).has_value())
            return false;

        integral_t start_x = std::max(x - 2, 0);
        integral_t end_x = std::min(x + 2, grid_w - 1);
        integral_t start_y = std::max(y - 2, 0);
        integral_t end_y = std::min(y + 2, grid_h - 1);

        for (auto i = start_y; i <= end_y; ++i)
            for (auto j = start_x; j <= end_x; ++j) {
                const auto& cell = get_cell(j, i);
                if (cell.has_value() && dist_sq(cell.value(), p) < min_dist_sq)
                    return false;
            }

        return true;
    };

    std::uniform_real_distribution<float> rng;

    auto new_candidate_around = [&](const point_t& p) {
        auto radius = conf.min_dist * (rng(rne) + 1.0f);
        auto angle_radians = 2.0f * std::numbers::pi_v<float_t> * rng(rne);

        return point_t(p.x + radius * std::cos(angle_radians), p.y + radius * sin(angle_radians));
    };

    std::stack<point_t> active;

    point_t first(rng(rne) * (conf.w - 1.0f), rng(rne) * (conf.h - 1.0f));

    set_cell(first);
    active.push(first);
    ret_points.push_back(first);

    while (!active.empty()) {
        auto point = active.top();
        active.pop();

        for (integral_t i = 0; i < conf.k_max_attempts; ++i) {
            auto candidate = new_candidate_around(point);

            if (!is_valid(candidate))
                continue;

            set_cell(candidate);
            active.push(candidate);
            ret_points.push_back(candidate);
        }
    }

    return ret_points;
}

} // blue_noise::bridson2d

#endif