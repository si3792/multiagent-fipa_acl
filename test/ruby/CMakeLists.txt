INCLUDE_DIRECTORIES(${RUBY_INCLUDE_PATH})
CONFIGURE_FILE(test_config.rb.in ${CMAKE_CURRENT_BINARY_DIR}/test_config.rb @ONLY)
ADD_TEST(Ruby /usr/bin/env testrb ${CMAKE_CURRENT_SOURCE_DIR} -x marshal)

# ADD_LIBRARY(test_ruby MODULE test_rb_value.cc)





