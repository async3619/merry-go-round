{
	"targets": [
		{
			"target_name": "merry-go-round",
			"include_dirs": "%INCLUDE_PATH%",
			"libraries": "%LIBRARY_FILES%",
			"dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
			"sources": [
				"<!@(node ./node_modules/ts-node/dist/bin.js tools/list-sources)"
			],
			"conditions": [
				["OS=='win'", {
					"configurations": {
						"Debug": {
							"msbuild_settings": {
								"ClCompile": {
									"RuntimeLibrary": "MultiThreadedDebug",
									"DisableSpecificWarnings": ["4541", "4291"],
									"Optimization": "Disabled",
									"ExceptionHandling": "Sync"
								},
								"Link": {
									"AdditionalLibraryDirectories": "%MSVC_LIBRARY_PATH%",
									"GenerateDebugInformation": "true"
								}
							}
						},
						"Release": {
							"msbuild_settings": {
								"ClCompile": {
									"RuntimeLibrary": "MultiThreaded",
									"DisableSpecificWarnings": ["4541"],
									"ExceptionHandling": "Sync"
								},
								"Link": {
									"AdditionalLibraryDirectories": "%MSVC_LIBRARY_PATH%"
								}
							}
						}
					}
				}],
				["OS=='linux'", {
					"ldflags": "%GCC_LIBRARY_PATH%",
					"cflags!": ["-fno-exceptions", "-fno-rtti"],
					"cflags_cc!": ["-fno-exceptions", "-fno-rtti"]
				}],
				["OS=='mac'", {
					"cflags+": ["-fvisibility=hidden", "-fno-rtti"],
					"xcode_settings": {
						"OTHER_LDFLAGS": "%GCC_LIBRARY_PATH%",
						"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
						"CLANG_CXX_LIBRARY": "libc++",
						"MACOSX_DEPLOYMENT_TARGET": "10.7",
						"GCC_SYMBOLS_PRIVATE_EXTERN": "YES"
					}
				}]
			]
		}
	]
}
