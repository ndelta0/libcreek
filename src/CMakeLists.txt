file(GLOB_RECURSE CREEK_INCLUDES CONFIGURE_DEPENDS "../include/creek/*.h*")
file(GLOB_RECURSE CREEK_SOURCES CONFIGURE_DEPENDS "*.c*" "*.h*")

add_library(creek ${CREEK_SOURCES} ${CREEK_INCLUDES})

target_compile_features(creek PUBLIC cxx_std_11)

target_include_directories(creek
  PRIVATE ../include/creek
  INTERFACE ../include)
