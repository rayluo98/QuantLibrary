load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_test(
  name = "matrix2_test",
  size = "small",
  srcs = ["matrix2_test.cpp"],
  deps = [
            "@com_google_googletest//:gtest_main",
            "//math_library:math_library"
        ],
)