{ 'targets': [
    {
      'target_name': 'cryptopp',
      'type': 'static_library',
      'sources': [
        "<!@(python ../deps/djinni/example/glob.py cryptopp '*.cpp')",
      ],
      'sources!':[
        'cryptopp/test.cpp',
        'cryptopp/bench1.cpp',
        'cryptopp/bench2.cpp',
        'cryptopp/validat1.cpp',
        'cryptopp/validat2.cpp',
        'cryptopp/validat3.cpp',
        'cryptopp/adhoc.cpp',
        'cryptopp/datatest.cpp',
        'cryptopp/regtest.cpp',
        'cryptopp/fipsalgt.cpp',
        'cryptopp/dlltest.cpp',
        'cryptopp/fipstest.cpp',
        'cryptopp/pch.cpp',
        'cryptopp/simple.cpp',
        'cryptopp/winpipes.cpp',
        'cryptopp/cryptlib_bds.cpp',
      ],
      'configurations': {
        'Debug': {
          # do _not_ put defines here, unless you add them in cflags as well
          # ios/clang doesn't respect them :(
          'cflags' : [ '-g', '-O0', '-DDEBUG=1' ],
          'xcode_settings': {
            'ONLY_ACTIVE_ARCH': 'YES',
            'GCC_OPTIMIZATION_LEVEL': 0,
            'WARNING_CFLAGS' : ['-Wmost', '-Wno-four-char-constants', '-Wno-unknown-pragmas', '$(inherited)'],
            'OTHER_CPLUSPLUSFLAGS' : ['-march=native', '-fPIC'],
          },
        },
        'Release': {
          'xcode_settings': {
            'DEAD_CODE_STRIPPING': 'YES',
            'GCC_OPTIMIZATION_LEVEL': 3,
            'WARNING_CFLAGS' : ['-Wmost', '-Wno-four-char-constants', '-Wno-unknown-pragmas', '$(inherited)'],
            'OTHER_CPLUSPLUSFLAGS' : ['-march=native', '-DNDEBUG', '-fPIC'],
          },
        },
      },
      'all_dependent_settings': {
        'include_dirs': [
          '.',
          'cryptopp',
        ]
      },
      'include_dirs': [
        'cryptopp',
      ],
    },
  ]
}
