## Installing the RV-Monitor

### Integrating the RV-Monitor into another C++ project
Recent CMake provide the `FetchContent` module which vastly simplifies the integration
for common use scenarios.
Simply insert the following snippet somewhere in your `CMakeLists.txt`:
```cmake
include(FetchContent)

FetchContent_Declare(RVMonitors
        GIT_REPOSITORY https://github.com/CommonRoad/rv-monitors.git
        # You can specify any reference here, but prefer specifying a concrete commit if possible
        # as that will speed up the build since Git won't need to check whether branch moved in the meantime
        GIT_TAG <reference to commit, branch, tag...>
)
FetchContent_MakeAvailable(RVMonitors)
```

Then add the RV-Monitor as a dependency to the targets which require it:
```cmake
target_link_libraries(<MyLibraryOrExecutable> PUBLIC RVMonitors::rvmonitors)
```
