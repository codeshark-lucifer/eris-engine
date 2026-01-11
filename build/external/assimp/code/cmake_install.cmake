# Install script for directory: D:/Programing Education/eris - engine/external/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ErisEngine")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp6.0.2-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Programing Education/eris - engine/build/external/assimp/lib/libassimp.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/anim.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/aabb.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ai_assert.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/camera.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/color4.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/color4.inl"
    "D:/Programing Education/eris - engine/build/external/assimp/code/../include/assimp/config.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ColladaMetaData.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/commonMetaData.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/defs.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/cfileio.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/light.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/material.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/material.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/matrix3x3.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/matrix4x4.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/mesh.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ObjMaterial.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/pbrmaterial.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/GltfMaterial.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/postprocess.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/quaternion.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/quaternion.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/scene.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/metadata.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/texture.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/types.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/vector2.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/vector2.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/vector3.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/vector3.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/version.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/cimport.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/AssertHandler.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/importerdesc.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Importer.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/IOStream.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Logger.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/LogStream.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/cexport.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Exporter.hpp"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SceneCombiner.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/fast_atof.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/qnan.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/BaseImporter.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Hash.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ParsingUtils.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/StreamReader.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/StreamWriter.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/StringComparison.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/StringUtils.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/GenericProperty.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SpatialSort.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SmallVector.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/StandardShapes.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/RemoveComments.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Subdivision.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Vertex.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/LineSplitter.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/TinyFormatter.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Profiler.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/LogAux.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Bitmap.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/XMLTools.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/XmlParser.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/MathFunctions.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Exceptional.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/ByteSwapper.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Programing Education/eris - engine/external/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Programing Education/eris - engine/build/external/assimp/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
