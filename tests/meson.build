src = ['../m209/Keywheel.cc',
       '../m209/M209.cc',
       '../m209/M209GenKey.cc',
       '../m209/Keywheel.h',
       '../m209/M209.h',
       '../m209/AppendixII.cpp',
       'test_m209.cpp',
       '../KeyListDataBase/KeyListDataBase.cpp',
       '../KeyListDataBase/KeyListDataBase.hpp']

test_m209 = executable('test_m209', src,
                       dependencies : boostdep,
                       include_directories : incdir,
                       install: false)

test('test_m209', test_m209,
      env: ['M209_KEYLIST_DIR=/Users/jdunn/Documents/XCode/m209group-key-lists/',
            'MESON_SOURCE_ROOT='+meson.source_root()])
