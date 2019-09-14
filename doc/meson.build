doxygen = find_program('doxygen')

if(doxygen.found())
  src_doxygen = [meson.source_root()+'/README.md', \
                 meson.source_root()+'/m209/m209_main.cc', \
		 meson.source_root()+'/m209/m209.h', \
		 meson.source_root()+'/m209/Keywheel.h']

	doc_config = configuration_data()
	doc_config.set('PACKAGE_NAME', meson.project_name())
	doc_config.set('PACKAGE_VERSION', meson.project_version())
	doc_config.set('PROJECT_SOURCE_DIR', meson.source_root())
	doc_config.set('INPUT', ' '.join(src_doxygen))

	doxyfile = configure_file(input : 'Doxyfile.in',
				  output : 'Doxyfile',
				  configuration : doc_config,
				  install : false)
	custom_target('doxygen',
		      input : doxyfile,
		      output : ['html', 'latex']  ,
		      command : [ doxygen, doxyfile ],
		      depend_files: src_doxygen,
		      install : false,
		      build_by_default : true)

endif

man_config = configuration_data()
man_config.set('VERSION', meson.project_version())

hagelin_man =configure_file(input :  'hagelin.mansrc',
	                          output : 'hagelin.1',
	                          configuration : man_config)

m209_man = configure_file(input :  'm209.mansrc',
	                        output : 'm209.1',
	                        configuration : man_config)

install_man([hagelin_man, m209_man])
