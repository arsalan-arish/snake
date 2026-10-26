#include <snake/program.h>
#include <filesystem>

Program::Program(char* argv[]) {

    // Set cwd where assets exist. In a release, good design is to set it to the directory containing the executable, assets folder should be parallel to it
    namespace fs = std::filesystem;
    fs::path executablePath(argv[0]);
    fs::current_path(executablePath.parent_path().parent_path().parent_path());

}