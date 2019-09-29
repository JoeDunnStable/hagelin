src = ['../m209/Keywheel.cc',
       '../m209/M209.cc',
       '../m209/M209GenKey.cc',
       '../m209/Keywheel.h',
       '../m209/M209.h',
       '../m209/AppendixII.cpp',
       '../KeyListDataBase/KeyListDataBase.cpp',
       '../KeyListDataBase/KeyListDataBase.hpp',
       'ValidateDrumOldBroken.cpp',
       'ValidateDrumOldBroken.hpp',
       'ValidateDrumOldFixed.cpp',
       'ValidateDrumOldFixed.hpp',
       'Check_KeyLists_main.cpp']

Check_KeyLists = executable('Check_KeyLists', src,
                       dependencies : boostdep,
                       include_directories : incdir,
                       install: false)

run_target('Check_KeyLists', command : Check_KeyLists)
