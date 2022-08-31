load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "mfv",
    srcs = glob(["*.cc"]),
    hdrs = glob(["include/*.h"]),
    deps = ["//lib:pie"],
    visibility = ["//visibility:public"],
)