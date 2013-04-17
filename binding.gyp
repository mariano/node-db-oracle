{
	'targets': [
		{
			'target_name': 'oracle_bindings',
			'sources': [
				'src/node-db/binding.cc',
				'src/node-db/connection.cc',
				'src/node-db/cursor.cc',
				'src/node-db/events.cc',
				'src/node-db/exception.cc',
				'src/node-db/query.cc',
				'src/node-db/result.cc',
				'src/connection.cc',
				'src/oracle.cc',
				'src/oracle_bindings.cc',
				'src/query.cc',
				'src/result.cc',
				# Rebuild on header changes
				'src/connection.h',
				'src/oracle.h',
				'src/query.h',
				'src/result.h',
			],
			'cflags!': ['-g', '-Wall'],
			'conditions': [
				['OS == "mac"', {
		        	'include_dirs': ['$(OCI_INCLUDE_DIR)'],
					'xcode_settings': { 'GCC_ENABLE_CPP_EXCEPTIONS': 'YES' },
		         	'libraries' : ['-L$(OCI_LIB_DIR)', '-locci', '-lclntsh', '-lnnz11'],
				}],
			],
		},
	],
}
