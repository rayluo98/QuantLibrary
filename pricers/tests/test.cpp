#include <algorithm>
#include <numeric>
#include <gtest/gtest.h>
#include "../BSModel.h"
#include "test.h"

TEST(PricerTests, GaussDistributionLooksNormal) {
	const size_t number_of_samples = 1000;
	std::vector<double> samples;
	samples.reserve(number_of_samples);
	for (size_t i = 0; i < number_of_samples; ++i) {
		samples.push_back(Gauss());
	}
	double total_sum = std::accumulate(samples.begin(), samples.end(), 0.0);
	double average = total_sum / number_of_samples;
	EXPECT_LE(std::abs(average), 3 / std::sqrt(number_of_samples));

	std::transform(
		samples.begin(),
		samples.end(),
		samples.begin(),
		[](double sample) {
			return sample * sample;
		}
	);
	double second_moment = std::accumulate(samples.begin(), samples.end(), 0.0) / number_of_samples;
	double variance = second_moment - average * average;
	EXPECT_LE(variance, 1.1);
	EXPECT_GE(variance, 0.9);
}