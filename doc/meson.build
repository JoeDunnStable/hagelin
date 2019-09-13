
man_config = configuration_data()
man_config.set('VERSION', meson.project_version())

configure_file(input :  'hagelin.mansrc',
	       output : 'hagelin.man',
	       configuration : man_config,
	       install : false)

configure_file(input :  'm209.mansrc',
	       output : 'm209.man',
	       configuration : man_config,
	       install : false)


