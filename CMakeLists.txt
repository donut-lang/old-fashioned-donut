cmake_minimum_required(VERSION 3.3)
project(Donut)
find_package(PkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CINAMO_SOURCE_FILES
        src/cinamo/include/cinamo/Exception.h
        src/cinamo/include/cinamo/FormPayload.h
        src/cinamo/include/cinamo/XArchiver.h
        src/cinamo/include/cinamo/TypeTraits.h
        src/cinamo/include/cinamo/MixType.h
        src/cinamo/include/cinamo/Matrix.h
        src/cinamo/include/cinamo/FileSystem.h
        src/cinamo/include/cinamo/Handler.h
        src/cinamo/include/cinamo/XMLAttrParser.h
        src/cinamo/include/cinamo/Base64.h
        src/cinamo/include/cinamo/ClassUtil.h
        src/cinamo/include/cinamo/internal/XArchiver.h
        src/cinamo/include/cinamo/internal/FileSystem.h
        src/cinamo/include/cinamo/internal/EnumProxy.h
        src/cinamo/include/cinamo/internal/Win32.h
        src/cinamo/include/cinamo/internal/VectorProxy.h
        src/cinamo/include/cinamo/internal/String.h
        src/cinamo/include/cinamo/internal/XVal.h
        src/cinamo/include/cinamo/internal/IntProxy.h
        src/cinamo/include/cinamo/String.h
        src/cinamo/include/cinamo/Dynamic.h
        src/cinamo/include/cinamo/Platform.h
        src/cinamo/include/cinamo/XVal.h
        src/cinamo/include/cinamo/Logger.h
        src/cinamo/include/cinamo/VectorMap.h
        src/cinamo/include/cinamo/Url.h
        src/cinamo/include/cinamo/PairFunctor.h
        src/cinamo/include/cinamo/Cinamo.h
        src/cinamo/include/cinamo/Time.h
        src/cinamo/include/cinamo/Common.h
        src/cinamo/include/cinamo/functional/Base.h
        src/cinamo/include/cinamo/functional/Maybe.h
        src/cinamo/include/cinamo/functional/Parser.h
        src/cinamo/include/cinamo/functional/Box.h
        src/cinamo/include/cinamo/functional/List.h
        src/cinamo/include/cinamo/functional/Ident.h
        src/cinamo/include/cinamo/functional/Either.h
        src/cinamo/include/cinamo/Thread.h
        src/cinamo/include/cinamo/Standard.h
        src/cinamo/src/StringICU.cpp
        src/cinamo/src/String.cpp
        src/cinamo/src/XArchiver.cpp
        src/cinamo/src/Url.cpp
        src/cinamo/src/StringSplit.cpp
        src/cinamo/src/StringParse.cpp
        src/cinamo/src/XVal.cpp
        src/cinamo/src/FileSystem.cpp
        src/cinamo/src/Base64.cpp
        src/cinamo/src/Time.cpp
        src/cinamo/src/Logger.cpp
        src/cinamo/src/Win32.cpp
        src/cinamo/src/Exception.cpp
        src/cinamo/src/Dynamic.cpp
        src/cinamo/src/FormPayload.cpp
        src/cinamo/src/XMLAttrParser.cpp
)

set(DONUT_SOURCE_FILES
        src/donut/include/donut/native/Bind.hpp
        src/donut/include/donut/native/Decoder.hpp
        src/donut/include/donut/native/Encoder.hpp
        src/donut/include/donut/native/ReactiveNativeClosure.hpp
        src/donut/include/donut/native/ReactiveNativeClosureDecl.hpp
        src/donut/include/donut/object/Decl.hpp
        src/donut/include/donut/object/DonutClosureObject.hpp
        src/donut/include/donut/object/DonutClosureObjectDecl.hpp
        src/donut/include/donut/object/DonutObject.hpp
        src/donut/include/donut/object/DonutObjectDecl.hpp
        src/donut/include/donut/object/FloatObject.hpp
        src/donut/include/donut/object/FloatObjectDecl.hpp
        src/donut/include/donut/object/Heap.hpp
        src/donut/include/donut/object/HeapDecl.hpp
        src/donut/include/donut/object/HomuraObject.hpp
        src/donut/include/donut/object/HomuraObjectDecl.hpp
        src/donut/include/donut/object/NativeClosureObject.hpp
        src/donut/include/donut/object/NativeClosureObjectDecl.hpp
        src/donut/include/donut/object/NativeObject.hpp
        src/donut/include/donut/object/NativeObjectDecl.hpp
        src/donut/include/donut/object/Object.hpp
        src/donut/include/donut/object/ObjectDecl.hpp
        src/donut/include/donut/object/PureNativeClosureObject.hpp
        src/donut/include/donut/object/PureNativeClosureObjectDecl.hpp
        src/donut/include/donut/object/ReactiveNativeClosureObject.hpp
        src/donut/include/donut/object/ReactiveNativeClosureObjectDecl.hpp
        src/donut/include/donut/object/ReactiveNativeObject.hpp
        src/donut/include/donut/object/ReactiveNativeObjectDecl.hpp
        src/donut/include/donut/object/Slot.hpp
        src/donut/include/donut/object/StringObject.hpp
        src/donut/include/donut/object/StringObjectDecl.hpp
        src/donut/include/donut/provider/BoolProvider.hpp
        src/donut/include/donut/provider/BoolProviderDecl.hpp
        src/donut/include/donut/provider/Decl.hpp
        src/donut/include/donut/provider/DonutProvider.hpp
        src/donut/include/donut/provider/DonutProviderDecl.hpp
        src/donut/include/donut/provider/FloatProvider.hpp
        src/donut/include/donut/provider/FloatProviderDecl.hpp
        src/donut/include/donut/provider/HomuraProvider.hpp
        src/donut/include/donut/provider/HomuraProviderDecl.hpp
        src/donut/include/donut/provider/IntProvider.hpp
        src/donut/include/donut/provider/IntProviderDecl.hpp
        src/donut/include/donut/provider/NativeClosureProvider.hpp
        src/donut/include/donut/provider/NativeClosureProviderDecl.hpp
        src/donut/include/donut/provider/NullProvider.hpp
        src/donut/include/donut/provider/NullProviderDecl.hpp
        src/donut/include/donut/provider/Provider.hpp
        src/donut/include/donut/provider/ProviderDecl.hpp
        src/donut/include/donut/provider/StringProvider.hpp
        src/donut/include/donut/provider/StringProviderDecl.hpp
        src/donut/include/donut/source/Closure.hpp
        src/donut/include/donut/source/Decl.hpp
        src/donut/include/donut/source/Inst.hpp
        src/donut/include/donut/source/Source.hpp
        src/donut/include/donut/vm/Machine.hpp
        src/donut/include/donut/vm/MachineImpl.hpp
        src/donut/include/donut/Clock.hpp
        src/donut/include/donut/Decl.hpp
        src/donut/include/donut/Donut.hpp
        src/donut/include/donut/DozenBox.hpp
        src/donut/include/donut/Exception.hpp
        src/donut/include/donut/Patron.hpp
        src/donut/src/native/Decoder.cpp
        src/donut/src/native/Encoder.cpp
        src/donut/src/object/DonutClosureObject.cpp
        src/donut/src/object/DonutObject.cpp
        src/donut/src/object/FloatObject.cpp
        src/donut/src/object/Heap.cpp
        src/donut/src/object/HeapGC.cpp
        src/donut/src/object/HeapObject.cpp
        src/donut/src/object/HomuraObject.cpp
        src/donut/src/object/NativeClosureObject.cpp
        src/donut/src/object/NativeObject.cpp
        src/donut/src/object/Object.cpp
        src/donut/src/object/ObjectMarking.cpp
        src/donut/src/object/ReactiveNativeObject.cpp
        src/donut/src/object/Slot.cpp
        src/donut/src/object/StringObject.cpp
        src/donut/src/parser/output/Compiler.cpp
        src/donut/src/parser/output/Compiler.h
        src/donut/src/parser/output/DonutLexer.cpp
        src/donut/src/parser/output/DonutLexer.h
        src/donut/src/parser/output/DonutParser.cpp
        src/donut/src/parser/output/DonutParser.h
        src/donut/src/parser/Parser.cpp
        src/donut/src/parser/Parser.hpp
        src/donut/src/parser/ParseUtil.cpp
        src/donut/src/parser/ParseUtil.hpp
        src/donut/src/provider/BoolProvider.cpp
        src/donut/src/provider/DonutProvider.cpp
        src/donut/src/provider/FloatProvider.cpp
        src/donut/src/provider/HeapProvider.cpp
        src/donut/src/provider/HomuraProvider.cpp
        src/donut/src/provider/IntProvider.cpp
        src/donut/src/provider/NativeClosureProvider.cpp
        src/donut/src/provider/NullProvider.cpp
        src/donut/src/provider/Provider.cpp
        src/donut/src/provider/StringProvider.cpp
        src/donut/src/source/Closure.cpp
        src/donut/src/source/Inst.cpp
        src/donut/src/source/Source.cpp
        src/donut/src/vm/Machine.cpp
        src/donut/src/vm/SaveLoad.cpp
        src/donut/src/vm/TimeOperation.cpp
        src/donut/src/Clock.cpp
        src/donut/src/Donut.cpp
        src/donut/src/DozenBox.cpp
        src/donut/src/Exception.cpp
)

set(DONUT_APP_SOURCE_FILES
        src/entrypoint/donut/main.cpp
        src/entrypoint/donut/SystemObject.cpp
        src/entrypoint/donut/SystemObject.hpp
        src/entrypoint/donut/SystemPatron.cpp
        src/entrypoint/donut/SystemPatron.hpp
        src/entrypoint/donut/SystemProvider.cpp
        src/entrypoint/donut/SystemProvider.hpp
)

set(TEST_SOURCE_FILES
    external/gtest/googletest/src/gtest-all.cc
    test/cinamo/Base64Test.cpp
    test/cinamo/BoxTest.cpp
    test/cinamo/ClassUtilTest.cpp
    test/cinamo/ConstexprList.cpp
    test/cinamo/DemangleTest.cpp
    test/cinamo/EitherTest.cpp
    test/cinamo/ExceptionTest.cpp
    test/cinamo/FileUtilTest.cpp
    test/cinamo/FormPayloadTest.cpp
    test/cinamo/FunctorBaseTest.cpp
    test/cinamo/LoggerTest.cpp
    test/cinamo/MatrixTest.cpp
    test/cinamo/MaybeTest.cpp
    test/cinamo/MixTypeTest.cpp
    test/cinamo/ParserTest.cpp
    test/cinamo/StringUtilTest.cpp
    test/cinamo/TypeTraitsTest.cpp
    test/cinamo/UnitTest.cpp
    test/cinamo/UrlTest.cpp
    test/cinamo/VectorMapTest.cpp
    test/cinamo/XArchiverTest.cpp
    test/cinamo/XMLAttrParserTest.cpp
    test/cinamo/XValueTest.cpp
    test/cinamo/XValueTestBinaryTypes.inc.h
    test/cinamo/XValueTestTypes.inc.h

    test/donut/lang/ArrayTest.cpp
    test/donut/lang/AssignOprationTest.cpp
    test/donut/lang/BlockTest.cpp
    test/donut/lang/BooleanTest.cpp
    test/donut/lang/DonutHelper.hpp
    test/donut/lang/FloatTet.cpp
    test/donut/lang/ForTest.cpp
    test/donut/lang/FunctionTest.cpp
    test/donut/lang/GlobalTest.cpp
    test/donut/lang/HomuraTest.cpp
    test/donut/lang/IfTest.cpp
    test/donut/lang/IntegerTest.cpp
    test/donut/lang/InterruptTest.cpp
    test/donut/lang/LocalTest.cpp
    test/donut/lang/NullTest.cpp
    test/donut/lang/ObjectTest.cpp
    test/donut/lang/ProtoTet.cpp
    test/donut/lang/ReactiveObjectTest.cpp
    test/donut/lang/ReturnTest.cpp
    test/donut/lang/SaveTest.cpp
    test/donut/lang/SeekTest.cpp
    test/donut/lang/SelfTest.cpp
    test/donut/lang/StringTest.cpp
    test/donut/ClockTest.cpp
    test/donut/EncoderDecoderStaticTest.cpp
    test/donut/ExceptionTest.cpp
    test/donut/Helper.hpp
    test/donut/ObjectTest.cpp
    test/donut/ParserTest.cpp
    test/donut/SlotTest.cpp
    test/donut/SourceSerializeTest.cpp
    test/main.cpp
    test/TestCommon.hpp
    test/TestUtil.cpp
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(APPLE)
  ## TODO: for OSX, homebrew
  set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/opt/icu4c/lib/pkgconfig")
endif()

include_directories(src/donut/include)
include_directories(src/cinamo/include)
include_directories(external/gtest/googletest/include)
include_directories(external/gtest/googletest)

pkg_check_modules(ICU4C REQUIRED icu-io icu-uc)
link_directories(${ICU4C_LIBRARY_DIRS})
include_directories(${ICU4C_INCLUDE_DIRS})

include(ExternalProject)
ExternalProject_Add(
        libantlr
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c-3.4
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c
        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c-3.4/configure --enable-64bit --prefix=${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_IN_SOURCE 1
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/libantlr3c/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2)

###############################################################################
#### cinamo lib
###############################################################################
add_subdirectory("external/tinyxml2")
add_library(cinamo STATIC ${CINAMO_SOURCE_FILES})
target_link_libraries(cinamo tinyxml2)

###############################################################################
#### donut lib
###############################################################################
add_library(donut STATIC ${DONUT_SOURCE_FILES})
add_dependencies(donut libantlr)
link_libraries(donut antlr3c)

###############################################################################
#### donut app
###############################################################################
add_executable(donutApp ${DONUT_APP_SOURCE_FILES})
target_link_libraries(donutApp cinamo)
target_link_libraries(donutApp donut)
target_link_libraries(donutApp ${ICU4C_LIBRARIES})
add_dependencies(donutApp libantlr)

###############################################################################
#### test app
###############################################################################
add_executable(testApp ${TEST_SOURCE_FILES})
target_link_libraries(testApp cinamo)
target_link_libraries(testApp donut)
target_link_libraries(testApp pthread)
target_link_libraries(testApp ${ICU4C_LIBRARIES})
add_dependencies(testApp libantlr)
