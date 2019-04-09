{
	"targets": [
		{
			"target_name": "nbind",
			"include_dirs": "%INCLUDE_PATH%",
			"libraries": "%LIBRARY_FILES%",
			"sources": [
				"native/**/*.cc"
			],
			"includes": [
				"auto.gypi"
			],
			"conditions": [
				["OS=='win'", {
					"msbuild_settings": {
						"ClCompile": {
							"RuntimeLibrary": "%RUNTIME_LIBRARY%",
							"DisableSpecificWarnings": ["4541"]
						},
						"Link": {
							"AdditionalLibraryDirectories": "%MSVC_LIBRARY_PATH%",
						}
					}
				}],
				["OS=='linux'", {
					"ldflags": "%GCC_LIBRARY_PATH%"
				}],
				["OS=='mac'", {
					"xcode_settings": {
						"OTHER_LDFLAGS": "%GCC_LIBRARY_PATH%"
					}
				}]
			]
		}
	],
	"includes": [
		"auto-top.gypi"
	]
}
