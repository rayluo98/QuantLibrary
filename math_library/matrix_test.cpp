#include <gtest/gtest.h>
#include <utility>
#include "matrix.h"

TEST(MatrixTest, GetDimSuccess) {
    MatLib::Matrix mat(5, 5);
    std::pair<int, int> expected_results(5, 5);

    EXPECT_EQ(mat.get_dim(), expected_results);
}

TEST(MatrixTest, GetDimFailure) {
    MatLib::Matrix mat(5, 5);
    std::pair<int, int> unexpected_results(5, 4);

    EXPECT_NE(mat.get_dim(), unexpected_results);
}

