#ifndef pds_bridson_hpp
#define pds_bridson_hpp

#include <numbers>
#include <vector>
#include <random>
#include <cmath>

namespace blue_noise {

typedef uint16_t int_t;
typedef float float_t;

template<typename point2d_t>
class _grid_t {
public:
    typedef std::vector<point2d_t> points_t;
    typedef typename points_t::size_type size_type;

    size_type w, h;
    int_t orig_w, orig_h;

public:
    _grid_t() = delete;
    _grid_t(int_t width, int_t height, float_t cell_size) : orig_w(width), orig_h(height), _cell_size(cell_size) {
        w = std::lround(float_t(orig_w) / _cell_size);
        h = std::lround(float_t(orig_h) / _cell_size);
        
        _points.resize(w * h);
        _has_point.resize(w * h, false);
    }

    struct coord_t {
        size_type x, y;
        coord_t() = delete;
        coord_t(size_type x_coord, size_type y_coord) : x(x_coord), y(y_coord) {}
    };

    coord_t cell_coord(const point2d_t& point) const {
        return coord_t(std::min(static_cast<size_type>(point.x / _cell_size), w - 1),
                       std::min(static_cast<size_type>(point.y / _cell_size), h - 1));
    }

    void add_point(const point2d_t& point) {
        auto coord = cell_coord(point);
        _points[coord.y * w + coord.x] = point;
        _has_point[coord.y * w + coord.x] = true;
    }

    bool has_point_at(size_type grid_x, size_type grid_y) const {
        return _has_point[grid_y * w + grid_x];
    }

    float_t distance(size_type grid_x, size_type grid_y, const point2d_t& point) const {
        auto dx = point.x - _points[grid_y * w + grid_x].x;
        auto dy = point.y - _points[grid_y * w + grid_x].y;
        return std::sqrt(dx * dx + dy * dy);
    }

private:
    float_t _cell_size;
    points_t _points;
    std::vector<bool> _has_point;
};

template<typename T>
void _remove_at(std::vector<T>& vec, typename std::vector<T>::size_type pos) {
    vec[pos] = std::move_if_noexcept((vec.back()));
    vec.pop_back();
}

template<typename point2d_t, typename rn_engine_t>
point2d_t _generate_random_point_around(const point2d_t& point, float_t min_dist, rn_engine_t& rne) {
    std::uniform_real_distribution<float_t> rng;

    auto radius = min_dist * (rng(rne) + 1.0f);
    auto angle_radians = 2.0f * std::numbers::pi_v<float_t> * rng(rne);

    return point2d_t(point.x + radius * std::cos(angle_radians), point.y + radius * sin(angle_radians));
}

template<typename point2d_t>
bool _is_valid_candidate(const point2d_t& point, const _grid_t<point2d_t>& grid, float_t min_dist) {
    if (point.x < 0 || point.x >= grid.orig_w || point.y < 0 || point.y >= grid.orig_h)
        return false;

    auto grid_coord = grid.cell_coord(point);

    auto start_x = grid_coord.x >= 2 ? grid_coord.x - 2 : 0;
    auto end_x = grid_coord.x <= grid.w - 3 ? grid_coord.x + 2 : grid.w - 1;
    auto start_y = grid_coord.y >= 2 ? grid_coord.y - 2 : 0;
    auto end_y = grid_coord.y <= grid.h - 3 ? grid_coord.y + 2 : grid.h - 1;

    for (int i = start_x; i <= end_x; ++i)
        for (int j = start_y; j <= end_y; ++j) {
            if (grid.has_point_at(i, j) && grid.distance(i, j, point) < min_dist - FLT_EPSILON)
                return false;
        }

    return true;
}

/*
 *
 */
template<typename point2d_t, typename rn_engine_t>
std::vector<point2d_t> pds_Bridson_sampling_2d(int_t width, int_t height, float_t min_dist, int_t k_max_candidates, rn_engine_t& rne) {
    const float_t cell_size = min_dist / std::numbers::sqrt2_v<float_t>;

    _grid_t<point2d_t> grid(width, height, cell_size);

    typedef std::vector<point2d_t> points_t;
    points_t active;
    points_t generated;
    generated.reserve(grid.w * grid.h);

    std::uniform_real_distribution<float_t> rng;
    point2d_t first(rng(rne) * (width - 1), rng(rne) * (height - 1));
    
    grid.add_point(first);
    active.push_back(first);
    generated.push_back(first);

    while (!active.empty()) {
        std::uniform_int_distribution<typename points_t::size_type> idx_rng(0, active.size() - 1);
        typename points_t::size_type random_idx = idx_rng(rne);
        point2d_t point = active[random_idx];

        bool found = false;
        for (int_t i = 0; i < k_max_candidates; ++i) {
            point2d_t candidate = _generate_random_point_around(point, min_dist, rne);
            if (!_is_valid_candidate(candidate, grid, min_dist))
                continue;

            grid.add_point(candidate);
            active.push_back(candidate);
            generated.push_back(candidate);

            found = true;
            break;
        }

        if (!found) {
            _remove_at(active, random_idx);
        }
    }

    return generated;
} // pds_Bridson_sampling_2d

} // blue_noise

#endif