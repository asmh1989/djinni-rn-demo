{
  "targets": [
    {
      "target_name": "libstml_objc",
      "type": "static_library",
      "dependencies": [
        "./deps/djinni/support-lib/support_lib.gyp:djinni_objc",
        "deps/cryptopp.gyp:cryptopp",
      ],
      "sources": [
        "<!@(python deps/djinni/example/glob.py generated-src/objc  '*.h' '*.mm' '*.m')",
        "<!@(python deps/djinni/example/glob.py generated-src/cpp   '*.cpp' '*.hpp')",
        "<!@(python deps/djinni/example/glob.py src '*.cpp' '*.hpp' '*.h')",
        "deps/json11/json11.cpp",
        "deps/json11/json11.hpp",
      ],
      "include_dirs": [
        "generated-src/objc",
        "generated-src/cpp",
        "src",
        "deps/asio/include",
        "deps/json11"
      ],
      'configurations': {
        'Debug': {
          # do _not_ put defines here, unless you add them in cflags as well
          # ios/clang doesn't respect them :(
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': 0,
            'GCC_PREPROCESSOR_DEFINITIONS' : ['DEBUG=1', "ASIO_STANDALONE"],
          },
        },
        'Release': {
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': 3,
            'GCC_PREPROCESSOR_DEFINITIONS' : ['NDEBUG=1', "ASIO_STANDALONE"],

          },
        },
      },
    },
    # {
    #   "target_name": "libstml_jni",
    #   "type": "shared_library",
    #   "dependencies": [
    #     "deps/djinni/support-lib/support_lib.gyp:djinni_jni",
    #     "deps/cryptopp.gyp:cryptopp",
    #   ],
    #   "ldflags" : [ "-llog", "-Wl,--build-id,--gc-sections,--exclude-libs,ALL" ],
    #   "sources": [
    #     "./deps/djinni/support-lib/jni/djinni_main.cpp",
    #     "<!@(python deps/djinni/example/glob.py generated-src/jni   '*.cpp')",
    #     "<!@(python deps/djinni/example/glob.py generated-src/cpp   '*.cpp')",
    #     "<!@(python deps/djinni/example/glob.py src '*.cpp')",
    #   ],
    #   "include_dirs": [
    #     "generated-src/jni",
    #     "generated-src/cpp",
    #     "src",
    #     "deps/asio/include",
    #   ],
    # },
  ],
}
