src = ['../m209/Keywheel.cc',
       '../c52/C52Keywheel.cpp',
       '../c52/C52.cpp',
       '../c52/C52GenKey.cpp',
       '../m209/Keywheel.h',
       '../c52/C52.hpp',
       'test_c52.cpp']

test_c52 = executable('test_c52', src,
                       dependencies : boostdep,
                       include_directories : incdir,
                       install: false)

test('test_c52', test_c52,
      env: ['C52_KEYLIST_DIR='+meson.source_root()+'/data/',
            'MESON_SOURCE_ROOT='+meson.source_root()],
      timeout : 1000)
