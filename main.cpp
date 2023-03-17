#include "pds_Bridson.hpp"

template<typename T>
struct point2d {
	T x, y;

	point2d() = default;
	point2d(T x_, T y_) : x(x_), y(y_) {}
};

int main(int argc, char* argv[]) {
	std::mt19937 rne;

	auto bridsonSamples = blue_noise::pds_Bridson_sampling_2d<point2d<float>, std::mt19937>(
            100, 100,
            10,
            30,
            rne
    );

	return 0;
}