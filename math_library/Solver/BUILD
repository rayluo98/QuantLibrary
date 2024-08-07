load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library (
    name = "optimization_engines",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h", "*.hpp"]),
    visibility = ["//visibility:public"],
)

cc_binary (
    name = "solver_test",
    srcs = ["SolverTest.cpp"],
    deps = [
            "optimization_engines",
            "//math_library/AAD:AAD",
        ],
)