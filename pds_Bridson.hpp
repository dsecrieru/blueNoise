#ifndef pds_bridson_hpp
#define pds_bridson_hpp

#include <numbers>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>

namespace blue_noise {

template<typename point_t>
struct _grid_t {
    typedef std::vector<point_t> points_t;
    typedef points_t::size_type pos_t;
    typedef std::vector<bool> valid_t;

    float cell_size;
    pos_t w;
    pos_t h;
    points_t points;
    valid_t valid;

    struct coord_t {
        pos_t x, y;
        coord_t() = delete;
        coord_t(pos_t x_coord, pos_t y_coord) : x(x_coord), y(y_coord) {}
    };

    _grid_t() = delete;
    _grid_t(float cs, pos_t gw, pos_t gh) : cell_size(cs), w(gw), h(gh), points(w * h), valid(w * h, false) {}

    coord_t cell_coord(const point_t& point) const {
        return coord_t(std::min(static_cast<pos_t>(point.x / cell_size), w - 1),
                       std::min(static_cast<pos_t>(point.y / cell_size), h - 1));
    }

    void add_point(const point_t& point) {
        static int counter = 1;
        auto coord = cell_coord(point);
        points[coord.y * w + coord.x] = point;
        valid[coord.y * w + coord.x] = true;
        ++counter;
        std::cout << counter << std::endl;
    }

    bool valid_at(pos_t grid_x, pos_t grid_y) const {
        return valid[grid_y * w + grid_x];
    }

    float distance(pos_t grid_x, pos_t grid_y, const point_t& point) const {
        auto dx = point.x - points[grid_y * w + grid_x].x;
        auto dy = point.y - points[grid_y * w + grid_x].y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

template<typename T>
T _remove_at(std::vector<T>& vec, typename std::vector<T>::size_type pos) {
	T elem = std::move_if_noexcept(vec[pos]);
	vec[pos] = std::move_if_noexcept((vec.back()));
	vec.pop_back();
	return elem;
}

template<typename T, typename rn_engine_t>
T _remove_random(std::vector<T>& vec, rn_engine_t& rne) {
    std::uniform_int_distribution<typename std::vector<T>::size_type> rng(0, vec.size() - 1);

    return _remove_at(vec, rng(rne));
}

template<typename point_t, typename rn_engine_t>
point_t _generate_random_point_around(const point_t& point, float min_dist, rn_engine_t& rne) {
    std::uniform_real_distribution<float> rng;

    auto radius = min_dist * (rng(rne) + 1.0f);
    auto angle = 2.0f * std::numbers::pi_v<float> * rng(rne);

    return point_t(point.x + radius * std::cos(angle), point.y + radius * sin(angle));
}

template<typename point_t>
bool _is_valid_candidate(const point_t& point, const _grid_t<point_t>& grid, float min_dist) {
    auto grid_coord = grid.cell_coord(point);

    auto start_x = grid_coord.x >= 2 ? grid_coord.x - 2 : 0;
    auto end_x = grid_coord.x <= grid.w - 3 ? grid_coord.x + 2 : grid.w - 1;
    auto start_y = grid_coord.y >= 2 ? grid_coord.y - 2 : 0;
    auto end_y = grid_coord.y <= grid.h - 3 ? grid_coord.y + 2 : grid.h - 1;

    for (int i = start_x; i <= end_x; ++i)
        for (int j = start_y; j <= end_y; ++j) {
            if (grid.valid_at(i, j) && grid.distance(i, j, point) < min_dist - FLT_EPSILON)
                return false;
        }

    return true;
}

/*
 *
 */
template<typename point_t, typename rn_engine_t>
std::vector<point_t> pds_Bridson_sampling_2d(uint16_t width, uint16_t height, float min_dist, uint8_t k_max_point_count, rn_engine_t& rne) {
	std::uniform_real_distribution<float> rng;

	const float cell_size = min_dist / std::numbers::sqrt2_v<float>;
    auto grid_w = std::lround(float(width) / cell_size);
    auto grid_h = std::lround(float(height) / cell_size);

    point_t first_point(rng(rne) * width, rng(rne) * height);

    std::vector<point_t> generated;
    generated.reserve(grid_w * grid_h);
    generated.push_back(first_point);

    _grid_t<point_t> grid(cell_size, grid_w, grid_h);
    grid.add_point(first_point);

    std::vector<point_t> active_list;
	active_list.push_back(first_point);

	int tries = 0;
    while (!active_list.empty()) {
        point_t active_point = _remove_random(active_list, rne);

		for (uint8_t i = 0; i < k_max_point_count; ++i) {
			point_t candidate = _generate_random_point_around(active_point, min_dist, rne);
			if (_is_valid_candidate(candidate, grid, min_dist)) {
                generated.push_back(candidate);
                grid.add_point(candidate);
                active_list.push_back(candidate);
            }
		}

        std::cout << "after try " << tries << ": " << active_list.size() << std::endl;
        ++tries;
        if (tries > 20)
            break;
	}

	return generated;
}

}

#endif