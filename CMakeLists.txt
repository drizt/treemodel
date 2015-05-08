project(treemodel)
cmake_minimum_required(VERSION 2.8.11)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set(HEADERS 
  mainwindow.h
  treemodel.h
  abstracttreemodel.h)

set(SOURCES
  main.cpp
  mainwindow.cpp
  treemodel.cpp
  abstracttreemodel.cpp)

set(FORMS
  mainwindow.ui)

if(QT5_BUILD)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)

  include_directories(${Qt5Core_INCLUDE_DIRS})
  include_directories(${Qt5Gui_INCLUDE_DIRS})
  include_directories(${Qt5Widgets_INCLUDE_DIRS})

  add_definitions(${Qt5Core_DEFINITIONS})
  add_definitions(${Qt5Gui_DEFINITIONS})
  add_definitions(${Qt5Widgets_DEFINITIONS})
  add_definitions(-DHAVE_QT5)

  qt5_wrap_cpp(MOC_SOURCES ${HEADERS})
  qt5_wrap_ui(UI_SOURCES ${FORMS})

else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})

  qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
  qt4_wrap_ui(UI_SOURCES ${FORMS})
endif()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UI_SOURCES})

if(QT5_BUILD)
  target_link_libraries(${PROJECT_NAME} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
else()
  target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
endif()
