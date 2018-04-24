-- 启用所有警告，并且作为编译错误处理
set_warnings("all", "error")
set_languages("cxx14")
add_cxxflags("/EHsc")

target("main")
	set_kind("binary")
	add_files("src/main.cpp")
	add_files("src/image/*.cpp")
	add_files("src/shape/*.cpp")
	add_files("src/model/*.cpp")