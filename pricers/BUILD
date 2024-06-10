load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library (
    name = "pricers",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h", "*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
        "//assets:assets",
    ],
)