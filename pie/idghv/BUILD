load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "demo",
    srcs = glob(["*.cc"] + ["*.h"]),
    deps = [
      "//lib:pie",
      "@ntl//:ntl_",
      "@gmp//:gmp_",
      "//lib/idghv:idghv"
      ],
)