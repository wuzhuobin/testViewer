cmake_minimum_required(VERSION 2.8)

project(testViewer)

#ITK
find_package(ITK REQUIRED)
if(NOT ITK_FOUND)
	message(FATAL_ERROR "ERROR: ITK IS NOT FOUND")
endif()
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
	message(FATAL_ERROR "ERROR: VTK IS NOT FOUND")
endif()
include(${VTK_USE_FILE})

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
	find_package(Qt5 COMPONENTS Widgets REQUIRED)
	include_directories(${Qt5Widgets_INCLUDE_DIRS})
	
else()
	find_package(Qt4 REQUIRED)
	include(${QT4_USE_FILE})
endif()

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}
)

set(QT_HEAD
	mainviewer.h
)

set(QT_SRC
	mainviewer.cpp
)

set(QT_UI
	mainviewer.ui
)

set(QT_QRC
	mainviewer.qrc
)

set(HEAD 
	MaximumWallThickness.h
	myVtkInteractorStyleImage.h
)

set(SRC
	MaximumWallThickness.cpp
	myVtkInteractorStyleImage.cpp
	main.cpp
)

set(RESOURCE
	
)


qt5_wrap_ui(QT_UI_HEAD ${QT_UI})
qt5_wrap_cpp(QT_MOC ${QT_HEAD})
qt5_add_resources(QT_RESOURCES ${QT_QRC})

add_executable(testViewer MACOSX_BUNDLE
	${QT_HEAD}
	${QT_SRC}
	${QT_UI}
	${QT_QRC}
	${HEAD}
	${SRC}
	${QT_UI_HEAD}
	${QT_MOC}
	${QT_RESOURCES}
	${RESOURCE}
)

target_link_libraries(testViewer
	${VTK_LIBRARIES}
	${ITK_LIBRARIES}
	${Glue}
	${Qt5Widgets_LIBRARIES}
)

source_group("GENERATED"
	FILES	${QT_UI_HEAD}
			${QT_MOC}
			${QT_RESOURCES}
)

source_group("FORM"
	FILES	${QT_UI}
)

source_group("HEAD"
	FILES	${HEAD}
			${QT_HEAD}
)

source_group("SOURCE"
	FILES	${QT_SRC}
			${QT_QRC}
			${SRC}
			${RESOURCE}
)