src = ['../m209/Keywheel.cc',
       '../m209/M209.cc',
       '../m209/M209GenKey.cc',
       '../m209/Keywheel.h',
       '../m209/M209.h',
       'test_m209.cpp']

test_m209 = executable('test_m209', src,
                       dependencies : boostdep,
                       include_directories : incdir,
                       install: false)

run_target('check', command : test_m209)
