load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library (
    name = "pricers_model_tests",
    srcs = ["test.cpp"],
    hdrs = ["test.h"],
    deps = [
        "//pricers:pricers",
        "@com_google_googletest//:gtest_main" 
    ],
    visibility = ["//pricers/tests:__pkg__"],
)

cc_test(
  name = "test",
  size = "small",
  srcs = ["test.cpp"],
  deps = [
            "pricers_model_tests",
        ],
)