package(default_visibility = ["//visibility:public"])

cc_library(
    name = "contracts",
    srcs = [],
    hdrs = ["contracts.h"],
)

cc_library(
    name = "circular_range",
    srcs = [],
    hdrs = ["circular_range.h"],
    deps = [":contracts"],
)

cc_test(
    name = "circular_range_test",
    srcs = ["circular_range_test.cpp"],
    deps = [
        ":circular_range",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "factory",
    srcs = [],
    hdrs = ["factory.h"],
)

cc_test(
    name = "factory_test",
    srcs = ["factory_test.cpp"],
    deps = [
        ":factory",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "max_size_vector",
    srcs = [],
    hdrs = ["max_size_vector.h"],
    deps = [":contracts"],
)

cc_test(
    name = "max_size_vector_test",
    srcs = ["max_size_vector_test.cpp"],
    deps = [
        ":max_size_vector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "meta_enum",
    srcs = [],
    hdrs = ["meta_enum.h", "meta_enum_iterator.h", "detail/meta_enum_detail.h"],
    deps = [":contracts"],
)

cc_test(
    name = "meta_enum_test",
    srcs = ["meta_enum_test.cpp"],
    deps = [
        ":meta_enum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "named_type",
    srcs = [],
    hdrs = ["named_type.h"],
    deps = [":contracts"],
)

cc_test(
    name = "named_type_test",
    srcs = ["named_type_test.cpp", "stream_container.h"],
    deps = [
        ":named_type",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "stop_watch",
    srcs = [],
    hdrs = ["stop_watch.h"],
)

cc_test(
    name = "stop_watch_test",
    srcs = ["stop_watch_test.cpp"],
    deps = [
        ":stop_watch",
        "@com_google_googletest//:gtest_main",
    ],
)