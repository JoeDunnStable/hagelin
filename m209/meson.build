src = ['Keywheel.cc',
       'M209.cc',
       'M209GenKey.cc',
       'Keywheel.h',
       'M209.h',
       'm209_main.cc',
       'AppendixII.cpp',
       '../KeyListDataBase/KeyListDataBase.cpp',
       '../KeyListDataBase/KeyListDataBase.hpp']

m209 = executable('m209', src,
                  dependencies : boostdep,
                  include_directories : incdir,
                  install: true)
                  
test('test_h', m209, args : '-h')
test('test_V', m209, args : '-V')
test('test_A_c', m209,
     args : ['-A', '-c',
            '--fileIn', meson.source_root()+'/tests/plain.txt',
            '--fileOut', meson.build_root()+'/tests/cipher.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/')
test('test_A_c_d', m209,
     args : ['-A', '-c', '-d',
             '--fileIn', meson.source_root()+'/tests/plain.txt',
             '--fileOut', meson.build_root()+'/tests/cipher.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/',
     should_fail: true)
test('test_A_c_p', m209,
     args : ['-A', '-c', '-p',
             '--fileIn', meson.source_root()+'/tests/plain.txt',
             '--fileOut', meson.build_root()+'/tests/cipher.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/',
     should_fail: true)
test('test_A_d', m209,
     args : ['-A', '-d',
       '--fileIn', meson.build_root()+'/tests/cipher.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/')
test('test_A_c_n', m209,
     args : ['-A', '-c',
             '-n', 'M209GROUP',
             '--fileIn', meson.source_root()+'/tests/plain.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/')
test('test_A_c_n_fail', m209,
     args : ['-A', '-c',
             '-n', 'DUMMY',
             '--fileIn', meson.source_root()+'/tests/plain.txt'],
     env: 'M209_KEYLIST_DIR='+meson.source_root()+'/../m209group-key-lists/',
     should_fail: true)
test('test_g_p', m209,
     args: ['-g', '-p', '-n', 'MYTEST',
            '--fileOut', meson.build_root()+'/tests/newkey.txt'])
test('test_a_c_l', m209,
     args: ['-a', '-c', '-t', meson.source_root()+'/tests',
            '-l', 'MB',
            '--fileIn', meson.source_root()+'/tests/plain.txt'])
test('test_a_c_nol', m209,
     args: ['-a', '-c', '-t', meson.source_root()+'/tests',
            '--fileIn', meson.source_root()+'/tests/plain.txt'],
     should_fail: true)
test('test_a_d', m209,
     args: ['-a', '-d', '-t', meson.source_root()+'/tests',
            '--fileIn', meson.source_root()+'/tests/cipher.txt'])
test('test_c_k_i', m209,
     args: ['-c', '-k', meson.source_root()+'/tests/MB.m209key',
            '-i', 'A', 'A', 'A', 'A', 'A', 'A',
            '--fileIn', meson.source_root()+'/tests/plain.txt',
            '--fileOut', meson.build_root()+'/tests/cipher2.txt'])
test('test_d_k_i', m209,
args: ['-d', '-k', meson.source_root()+'/tests/MB.m209key',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.build_root()+'/tests/cipher2.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered2.txt'])
test('test_c_nok_i', m209,
args: ['-c',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.source_root()+'/tests/plain.txt',
       '--fileOut', meson.build_root()+'/tests/cipher2.txt'],
       should_fail: true)
test('test_c_k_noi', m209,
args: ['-c', '-k', meson.source_root()+'/tests/MB.m209key',
       '--fileIn', meson.source_root()+'/tests/plain.txt',
       '--fileOut', meson.build_root()+'/tests/cipher2.txt'],
       should_fail: true)
test('test_d_k_i5', m209,
args: ['-d', '-k', meson.source_root()+'/tests/MB.m209key',
       '-i', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.build_root()+'/tests/cipher2.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered2.txt'],
       should_fail: true)
test('test_d_k_i7', m209,
args: ['-d', '-k', meson.source_root()+'/tests/MB.m209key',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.build_root()+'/tests/cipher2.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered2.txt'],
       should_fail: true)
