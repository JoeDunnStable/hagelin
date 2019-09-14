
py_config = configuration_data()
py_config.set('VERSION', meson.project_version())

configure_file(input :  'm209-indicator.pysrc',
	       output : 'm209-indicator.py',
	       configuration : py_config,
	       install : true,
	       install_dir : 'bin',
	       install_mode : 'rwxr-xr-x')

configure_file(input :  'm209-keylist.pysrc',
	       output : 'm209-keylist.py',
	       configuration : py_config,
	       install : true,
	       install_dir : 'bin',
	       instal)_mode : 'rwxr-xr-x') 

