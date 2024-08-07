load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library (
    name = "math_library",
    srcs = ["matrix.cpp"],
    hdrs = ["matrix.h", "gaussians.h"],
    visibility = ["//visibility:public"],
)

cc_test(
  name = "matrix_test",
  size = "small",
  srcs = ["matrix_test.cpp"],
  deps = [
            "@com_google_googletest//:gtest_main",
            "math_library"
        ],
)