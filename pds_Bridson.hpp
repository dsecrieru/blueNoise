#ifndef pds_bridson_hpp
#define pds_bridson_hpp

#include <numbers>
#include <vector>
#include <optional>
#include <random>
#include <cmath>
#include <functional>

namespace blue_noise {

typedef uint32_t integral_t;
typedef float float_t;

namespace _internal {

    template<typename point2d_t>
    class _grid_t {
    public:
        typedef typename std::vector<std::optional<point2d_t>>::size_type size_type;

    public:
        _grid_t() = delete;
        _grid_t(integral_t width, integral_t height, float_t min_dist)
                : _orig_w(width), _orig_h(height), _cell_size(min_dist / std::numbers::sqrt2_v<float_t>),
                  _w(std::lround(float_t(_orig_w) / _cell_size)), _h(std::lround(float_t(_orig_h) / _cell_size)),
                  _min_dist_squared(min_dist * min_dist + std::numeric_limits<float_t>::epsilon()), _points(_w * _h) {
        }

        [[nodiscard]] size_type w() const { return _w; }
        [[nodiscard]] size_type h() const { return _h; }

        struct coord_t {
            size_type x, y;
            coord_t() = delete;
            coord_t(size_type x_coord, size_type y_coord) : x(x_coord), y(y_coord) {}
        };

        coord_t cell_coord(const point2d_t &point) const {
            return coord_t(std::min(static_cast<size_type>(point.x / _cell_size), _w - 1),
                           std::min(static_cast<size_type>(point.y / _cell_size), _h - 1));
        }

        void add_point(const point2d_t &point) {
            coord_t coord = cell_coord(point);
            _points[coord.y * _w + coord.x] = point;
        }

        bool has_within(const point2d_t &point) const {
            if (point.x < 0 || point.x >= _orig_w || point.y < 0 || point.y >= _orig_h)
                return false;
            return true;
        }

        bool is_valid_candidate(size_type grid_x, size_type grid_y, const point2d_t &point) const {
            const auto &grid_opt = _points[grid_y * _w + grid_x];
            if (!grid_opt.has_value())
                return true;

            const point2d_t &grid_point = grid_opt.value();
            float_t dx = point.x - grid_point.x;
            float_t dy = point.y - grid_point.y;

            return dx * dx + dy * dy > _min_dist_squared;
        }

    private:
        const integral_t _orig_w, _orig_h;
        const float_t _cell_size;
        const size_type _w, _h;
        const float_t _min_dist_squared;
        std::vector<std::optional<point2d_t>> _points;
    };

    template<typename T>
    void _remove_at(std::vector<T> &vec, typename std::vector<T>::size_type pos) {
        vec[pos] = std::move_if_noexcept((vec.back()));
        vec.pop_back();
    }

    template<typename point2d_t, typename rn_engine_t>
    requires std::uniform_random_bit_generator<rn_engine_t>
    point2d_t _generate_random_point_around(const point2d_t &point, float_t min_dist, rn_engine_t &rne) {
        std::uniform_real_distribution<float_t> rng;

        float_t radius = min_dist * (rng(rne) + 1.0f);
        float_t angle_radians = 2.0f * std::numbers::pi_v<float_t> * rng(rne);

        return point2d_t(point.x + radius * std::cos(angle_radians), point.y + radius * sin(angle_radians));
    }

    template<typename point2d_t>
    bool _is_valid_candidate(const point2d_t &point, const _grid_t<point2d_t> &grid) {
        if (!grid.has_within(point))
            return false;

        auto grid_coord = grid.cell_coord(point);

        typedef typename _grid_t<point2d_t>::size_type size_type;
        size_type start_x = grid_coord.x >= 2 ? grid_coord.x - 2 : 0;
        size_type end_x = grid_coord.x <= grid.w() - 3 ? grid_coord.x + 2 : grid.w() - 1;
        size_type start_y = grid_coord.y >= 2 ? grid_coord.y - 2 : 0;
        size_type end_y = grid_coord.y <= grid.h() - 3 ? grid_coord.y + 2 : grid.h() - 1;

        for (size_type i = start_x; i <= end_x; ++i)
            for (size_type j = start_y; j <= end_y; ++j)
                if (!grid.is_valid_candidate(i, j, point))
                    return false;

        return true;
    }

} // namespace internal

/*
 * Implementation of Robert Bridson's Fast Poisson Disk Sampling for the 2-dimensional case.
 *
 * More details in the original paper:
 *     https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
 *
 */
template<typename point2d_t, typename rn_engine_t>
requires std::uniform_random_bit_generator<rn_engine_t>
std::vector<point2d_t> pds_Bridson_sampling_2d(integral_t width, integral_t height,
                                               float_t min_dist, integral_t k_max_candidates,
                                               rn_engine_t& rne) {
    _internal::_grid_t<point2d_t> grid(width, height, min_dist);

    typedef std::vector<point2d_t> points_t;
    points_t active;
    points_t generated;
    generated.reserve(grid.w() * grid.h());

    std::uniform_real_distribution<float_t> rng;
    point2d_t first(rng(rne) * (width - 1), rng(rne) * (height - 1));
    
    grid.add_point(first);
    active.push_back(first);
    generated.push_back(first);

    while (!active.empty()) {
        std::uniform_int_distribution<typename points_t::size_type> idx_rng(0, active.size() - 1);
        typename points_t::size_type random_idx = idx_rng(rne);
        const point2d_t& point = active[random_idx];

        bool found = false;
        for (integral_t i = 0; i < k_max_candidates; ++i) {
            point2d_t candidate = _internal::_generate_random_point_around(point, min_dist, rne);
            if (!_is_valid_candidate(candidate, grid))
                continue;

            grid.add_point(candidate);
            active.push_back(candidate);
            generated.push_back(candidate);

            found = true;
            break;
        }

        if (!found) {
            _internal::_remove_at(active, random_idx);
        }
    }

    return generated;
}

} // blue_noise

#endif