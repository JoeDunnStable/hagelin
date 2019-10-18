src = ['../m209/Keywheel.cc',
       'C52.cpp',
       'C52GenKey.cpp',
       '../m209/Keywheel.h',
       'C52.hpp',
       'C52_main.cpp']

c52 = executable('c52', src,
                  dependencies : boostdep,
                  include_directories : incdir,
                  install: true)
                  
test('test_c52_h', c52, args : '-h')
test('test_c52_V', c52, args : '-V')
test('test_c52_A_c', c52,
     args : ['-A', '-c',
            '--fileIn', meson.source_root()+'/tests/plain.txt',
            '--fileOut', meson.build_root()+'/tests/cipher_c52_AutoKey.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/')
test('test_c52_A_c_d', c52,
     args : ['-A', '-c', '-d',
             '--fileIn', meson.source_root()+'/tests/plain.txt',
             '--fileOut', meson.build_root()+'/tests/cipher2_c52_AutoKey.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/',
     should_fail: true)
test('test_c52_A_c_p', c52,
     args : ['-A', '-c', '-p',
             '--fileIn', meson.source_root()+'/tests/plain.txt',
             '--fileOut', meson.build_root()+'/tests/cipher2_c52_AutoKey.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/',
     should_fail: true)
test('test_c52_A_d', c52,
     args : ['-A', '-d',
       '--fileIn', meson.source_root()+'/tests/cipher_c52_AutoKey.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered_c52_AutoKey.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/')
test('test_c52_A_c_n', c52,
     args : ['-A', '-c',
             '-n', 'c52NET',
             '--fileIn', meson.source_root()+'/tests/plain.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/')
test('test_c52_A_c_n_fail', c52,
     args : ['-A', '-c',
             '-n', 'DUMMY',
             '--fileIn', meson.source_root()+'/tests/plain.txt'],
     env: 'C52_KEYLIST_DIR='+meson.source_root()+'/data/',
     should_fail: true)
test('test_c52_g_p', c52,
     args: ['-g', '-p', '-n', 'MYTEST',
            '--fileOut', meson.build_root()+'/tests/newkey_c52.txt'],
     timeout: 1000)
test('test_c52_a_c_l', c52,
     args: ['-a', '-c', '-t', meson.source_root()+'/tests',
            '-l', '20191015',
            '--fileIn', meson.source_root()+'/tests/plain.txt'])
test('test_c52_a_c_nol', c52,
     args: ['-a', '-c', '-t', meson.source_root()+'/tests',
            '--fileIn', meson.source_root()+'/tests/plain.txt'],
     should_fail: true)
test('test_c52_a_d', c52,
     args: ['-a', '-d', '-t', meson.source_root()+'/tests',
            '-l', '20191015',
            '--fileIn', meson.source_root()+'/tests/cipher_c52_AutoMsg.txt',
            '--fileOut', meson.build_root()+'/tests/deciphered_c52_AutoMsg.txt'])
test('test_c52_c_k_i', c52,
     args: ['-c', '-k', meson.source_root()+'/tests/20191015.c52key',
            '-i', 'A', 'A', 'A', 'A', 'A', 'A',
            '--fileIn', meson.source_root()+'/tests/plain.txt',
            '--fileOut', meson.build_root()+'/tests/cipher2_c52.txt'])
test('test_c52_d_k_i', c52,
args: ['-d', '-k', meson.source_root()+'/tests/20191015.c52key',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.source_root()+'/tests/cipher2_c52.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered2_c52.txt'])
test('test_c52_c_nok_i', c52,
args: ['-c',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.source_root()+'/tests/plain.txt',
       '--fileOut', meson.build_root()+'/tests/cipher3_c52.txt'],
       should_fail: true)
test('test_c52_c_k_noi', c52,
args: ['-c', '-k', meson.source_root()+'/tests/20191015.c52key',
       '--fileIn', meson.source_root()+'/tests/plain.txt',
       '--fileOut', meson.build_root()+'/tests/cipher3_c52.txt'],
       should_fail: true)
test('test_c52_d_k_i5', c52,
args: ['-d', '-k', meson.source_root()+'/tests/20191015.c52key',
       '-i', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.build_root()+'/tests/cipher_c52.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered3_c52.txt'],
       should_fail: true)
test('test_c52_d_k_i7', c52,
args: ['-d', '-k', meson.source_root()+'/tests/20191015.c52key',
       '-i', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
       '--fileIn', meson.build_root()+'/tests/cipher2_c52.txt',
       '--fileOut', meson.build_root()+'/tests/deciphered3_c52.txt'],
       should_fail: true)
