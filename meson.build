project('Hagelin', 'cpp', default_options : ['cpp_std=c++11'],
         version : '2018-06-06a')

compiler=meson.get_compiler('cpp').get_id()

if compiler == 'gcc' or compiler == 'clang'
  add_global_arguments(
      [ '-fvisibility=hidden' ,
	       '-Wno-non-virtual-dtor' ,
	       '-Wno-sign-compare' ,
	       '-Wno-uninitialized',
               '-Wno-return-type' ,
               '-Wno-parentheses' ,
               '-Wno-attributes'
      ] ,
      language : 'cpp' )
elif compiler == 'msvc'
  add_global_arguments(
      ['/bigobj',
       '/wd4146',
       '/wd4244' ,
       '/wd4251' ,
       '/wd4715'
      ] ,
      language : 'cpp' )
  add_global_link_arguments(
       '/STACK:16000000',
      language : 'cpp' )
endif

#Boost header and some libraries.
boostdep = dependency('boost', modules : ['regex'],  required : true)

hagelin_config = configuration_data()
hagelin_config.set('PACKAGE_NAME', meson.project_name())
hagelin_config.set('PACKAGE_VERSION', meson.project_version())
hagelin_config.set('PACKAGE_COMPILER', meson.get_compiler('cpp').get_id())
hagelin_config.set('PACKAGE_COMPILER_VERSION',meson.get_compiler('cpp').version())
hagelin_config.set('PACKAGE_OS',host_machine.system())
hagelin_config.set('BOOST_VERSION',boostdep.version())

hagelin_config_h = configure_file(input : 'config.h.in',
				  output : 'config.h',
				  configuration : hagelin_config,
				  install : false)

incdir=include_directories(['.', 'm209'])

subdir('doc')
subdir('m209')
subdir('python')
subdir('tests')
