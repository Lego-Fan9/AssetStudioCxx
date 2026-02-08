add_executable(AssetStudioCxxTestingCLI main.cpp)

target_compile_definitions(AssetStudioCxx PRIVATE SET_BUNDLE_TEST_FILE)

target_link_libraries(AssetStudioCxxTestingCLI
    PRIVATE 
        AssetStudioCxx
        spdlog
)