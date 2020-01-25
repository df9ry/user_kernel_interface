project('Hello', 'c', version: '0.0.1', default_options: [
	'cpp_std=c++11',
	'c_flags=-fpic -pthread',
	'c_link_args=-lpthread'
])

uki_lib = shared_library('uki',
	sources: [
		'div64.c',
		'jiffies.c',
		'timer.c'
	],
	install: true
)

install_subdir('uki', install_dir: 'include')

uki_dep = declare_dependency(
	include_directories: include_directories(['.']),
	link_with: uki_lib
)

executable('uki_test',
	sources:      [
		'container_test.c',
		'list_test.c',
		'timer_test.c',
		'uki_test.c'
	],
    dependencies: [uki_dep]
)
