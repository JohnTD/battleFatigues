cmake_minimum_required( VERSION 2.8 )
project( battle-fatigues )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory( . SRC_LIST )
add_executable( battle-fatigues ${SRC_LIST} )
target_link_libraries( battle-fatigues ${OpenCV_LIBS} )
