cmake_minimum_required( VERSION 2.8 )
project( battleFatigues )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory( . SRC_LIST )
add_executable( battleFatigues ${SRC_LIST} )
target_link_libraries( battleFatigues ${OpenCV_LIBS} )
