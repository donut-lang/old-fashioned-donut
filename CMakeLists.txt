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

set(CHISA_SOURCE_FILES
        src/chisa/audio/Instrument.cpp
        src/chisa/audio/Instrument.hpp
        src/chisa/audio/Quartet.cpp
        src/chisa/audio/Quartet.hpp
        src/chisa/audio/SoundSpec.cpp
        src/chisa/audio/SoundSpec.hpp
        src/chisa/doc/dispose/BlockSession.cpp
        src/chisa/doc/dispose/BlockSession.hpp
        src/chisa/doc/dispose/Disposer.cpp
        src/chisa/doc/dispose/Disposer.hpp
        src/chisa/doc/dispose/PostDisposer.cpp
        src/chisa/doc/dispose/PostDisposer.hpp
        src/chisa/doc/dispose/TextContext.cpp
        src/chisa/doc/dispose/TextContext.hpp
        src/chisa/doc/node/Decl.hpp
        src/chisa/doc/node/Node.cpp
        src/chisa/doc/node/Node.hpp
        src/chisa/doc/node/NodeReader.cpp
        src/chisa/doc/node/NodeReader.hpp
        src/chisa/doc/node/NodeWalker.cpp
        src/chisa/doc/node/NodeWalker.hpp
        src/chisa/doc/node/Serializer.cpp
        src/chisa/doc/render/RenderObject.cpp
        src/chisa/doc/render/RenderObject.hpp
        src/chisa/doc/render/RenderTree.cpp
        src/chisa/doc/render/RenderTree.hpp
        src/chisa/doc/Util.cpp
        src/chisa/doc/Util.hpp
        src/chisa/geom/Area.hpp
        src/chisa/geom/Decl.hpp
        src/chisa/geom/Matrix.hpp
        src/chisa/geom/Serialize.cpp
        src/chisa/geom/Vector.hpp
        src/chisa/gl/internal/BitmapGlyph.cpp
        src/chisa/gl/internal/Buffer.cpp
        src/chisa/gl/internal/Buffer.hpp
        src/chisa/gl/internal/FontManager.cpp
        src/chisa/gl/internal/FontManager.hpp
        src/chisa/gl/internal/FreeType.cpp
        src/chisa/gl/internal/ImageManager.cpp
        src/chisa/gl/internal/ImageManager.hpp
        src/chisa/gl/internal/Order.hpp
        src/chisa/gl/internal/Serializer.cpp
        src/chisa/gl/internal/SpriteManager.cpp
        src/chisa/gl/internal/SpriteManager.hpp
        src/chisa/gl/Canvas.cpp
        src/chisa/gl/Canvas.hpp
        src/chisa/gl/Color.cpp
        src/chisa/gl/Color.hpp
        src/chisa/gl/DecoUtil.cpp
        src/chisa/gl/DecoUtil.hpp
        src/chisa/gl/Drawable.cpp
        src/chisa/gl/Drawable.hpp
        src/chisa/gl/DrawableManager.cpp
        src/chisa/gl/DrawableManager.hpp
        src/chisa/gl/Font.cpp
        src/chisa/gl/Font.hpp
        src/chisa/gl/ImageFormat.hpp
        src/chisa/gl/OpenGL.hpp
        src/chisa/gl/PredefinedSymRenderer.cpp
        src/chisa/gl/PredefinedSymRenderer.hpp
        src/chisa/gl/Sprite.cpp
        src/chisa/gl/Sprite.hpp
        src/chisa/gl/Util.hpp
        src/chisa/input/Joystick.cpp
        src/chisa/input/Joystick.hpp
        src/chisa/input/JoystickManager.cpp
        src/chisa/input/JoystickManager.hpp
        src/chisa/tk/donut/element/LabelObject.cpp
        src/chisa/tk/donut/element/LabelObject.hpp
        src/chisa/tk/donut/element/ScriptButtonObject.cpp
        src/chisa/tk/donut/element/ScriptButtonObject.hpp
        src/chisa/tk/donut/element/SplitComboObject.cpp
        src/chisa/tk/donut/element/SplitComboObject.hpp
        src/chisa/tk/donut/element/TextAreaObject.cpp
        src/chisa/tk/donut/element/TextAreaObject.hpp
        src/chisa/tk/donut/widget/ContentObject.cpp
        src/chisa/tk/donut/widget/ContentObject.hpp
        src/chisa/tk/donut/AngelObject.cpp
        src/chisa/tk/donut/AngelObject.hpp
        src/chisa/tk/donut/AngelTargetObject.cpp
        src/chisa/tk/donut/AngelTargetObject.hpp
        src/chisa/tk/donut/ButtonObject.cpp
        src/chisa/tk/donut/ButtonObject.hpp
        src/chisa/tk/donut/ElementGroupObject.cpp
        src/chisa/tk/donut/ElementGroupObject.hpp
        src/chisa/tk/donut/ElementObject.cpp
        src/chisa/tk/donut/ElementObject.hpp
        src/chisa/tk/donut/GeistObject.cpp
        src/chisa/tk/donut/GeistObject.hpp
        src/chisa/tk/donut/HeavenObject.cpp
        src/chisa/tk/donut/HeavenObject.hpp
        src/chisa/tk/donut/Patron.cpp
        src/chisa/tk/donut/Patron.hpp
        src/chisa/tk/donut/ServantObject.cpp
        src/chisa/tk/donut/ServantObject.hpp
        src/chisa/tk/donut/WidgetObject.cpp
        src/chisa/tk/donut/WidgetObject.hpp
        src/chisa/tk/donut/WorldObject.cpp
        src/chisa/tk/donut/WorldObject.hpp
        src/chisa/tk/element/AbstractButton.cpp
        src/chisa/tk/element/AbstractButton.hpp
        src/chisa/tk/element/AbstractToggleButton.cpp
        src/chisa/tk/element/AbstractToggleButton.hpp
        src/chisa/tk/element/ClickButton.cpp
        src/chisa/tk/element/ClickButton.hpp
        src/chisa/tk/element/ClockButton.cpp
        src/chisa/tk/element/ClockButton.hpp
        src/chisa/tk/element/ClockSlider.cpp
        src/chisa/tk/element/ClockSlider.hpp
        src/chisa/tk/element/ElementFactory.cpp
        src/chisa/tk/element/ElementFactory.hpp
        src/chisa/tk/element/ElementGroup.cpp
        src/chisa/tk/element/ElementGroup.hpp
        src/chisa/tk/element/Empty.cpp
        src/chisa/tk/element/Empty.hpp
        src/chisa/tk/element/FrameCombo.cpp
        src/chisa/tk/element/FrameCombo.hpp
        src/chisa/tk/element/Label.cpp
        src/chisa/tk/element/Label.hpp
        src/chisa/tk/element/MarginCombo.cpp
        src/chisa/tk/element/MarginCombo.hpp
        src/chisa/tk/element/ScriptButton.cpp
        src/chisa/tk/element/ScriptButton.hpp
        src/chisa/tk/element/ScrollCombo.cpp
        src/chisa/tk/element/ScrollCombo.hpp
        src/chisa/tk/element/SplitCombo.cpp
        src/chisa/tk/element/SplitCombo.hpp
        src/chisa/tk/element/TabButton.cpp
        src/chisa/tk/element/TabButton.hpp
        src/chisa/tk/element/TabCombo.cpp
        src/chisa/tk/element/TabCombo.hpp
        src/chisa/tk/element/TextArea.cpp
        src/chisa/tk/element/TextArea.hpp
        src/chisa/tk/element/WidgetElement.cpp
        src/chisa/tk/element/WidgetElement.hpp
        src/chisa/tk/heaven/AngelElementTarget.cpp
        src/chisa/tk/heaven/AngelElementTarget.hpp
        src/chisa/tk/heaven/AngelWidgetTarget.cpp
        src/chisa/tk/heaven/AngelWidgetTarget.hpp
        src/chisa/tk/heaven/ContentUnderlineServant.cpp
        src/chisa/tk/heaven/ContentUnderlineServant.hpp
        src/chisa/tk/heaven/ElementServant.cpp
        src/chisa/tk/heaven/ElementServant.hpp
        src/chisa/tk/heaven/HaloServant.cpp
        src/chisa/tk/heaven/HaloServant.hpp
        src/chisa/tk/heaven/LoneAngel.cpp
        src/chisa/tk/heaven/LoneAngel.hpp
        src/chisa/tk/heaven/TwinAngel.cpp
        src/chisa/tk/heaven/TwinAngel.hpp
        src/chisa/tk/widget/ContentWidget.cpp
        src/chisa/tk/widget/ContentWidget.hpp
        src/chisa/tk/widget/ImageWidget.cpp
        src/chisa/tk/widget/ImageWidget.hpp
        src/chisa/tk/widget/WidgetFactory.cpp
        src/chisa/tk/widget/WidgetFactory.hpp
        src/chisa/tk/Action.hpp
        src/chisa/tk/ActionMediator.cpp
        src/chisa/tk/ActionSession.cpp
        src/chisa/tk/Angel.cpp
        src/chisa/tk/Angel.hpp
        src/chisa/tk/Element.cpp
        src/chisa/tk/Element.hpp
        src/chisa/tk/Heaven.cpp
        src/chisa/tk/Heaven.hpp
        src/chisa/tk/Stack.hpp
        src/chisa/tk/Task.cpp
        src/chisa/tk/Task.hpp
        src/chisa/tk/Universe.cpp
        src/chisa/tk/Universe.hpp
        src/chisa/tk/Widget.cpp
        src/chisa/tk/Widget.hpp
        src/chisa/tk/World.cpp
        src/chisa/tk/World.hpp
        src/chisa/Chisa.cpp
        src/chisa/Chisa.hpp
        src/chisa/Hexe.cpp
        src/chisa/Hexe.hpp
        src/chisa/PlatformFairy.cpp
        src/chisa/PlatformFairy.hpp
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

set(CHISA_APP_SOURCE_FILES
        src/entrypoint/pc/main.cpp
        src/entrypoint/pc/SDLJoystick.cpp
        src/entrypoint/pc/SDLJoystick.hpp
        src/entrypoint/pc/SDLJoystickManager.cpp
        src/entrypoint/pc/SDLJoystickManager.hpp
        src/entrypoint/pc/SDLPlatformFairy.cpp
        src/entrypoint/pc/SDLPlatformFairy.hpp
        src/entrypoint/pc/SDLQuartet.cpp
        src/entrypoint/pc/SDLQuartet.hpp
        src/nes/donut/NesGeistObject.cpp
        src/nes/donut/NesGeistObject.hpp
        src/nes/machine/debugger/Assembler.cpp
        src/nes/machine/debugger/Assembler.hpp
        src/nes/machine/debugger/Debugger.cpp
        src/nes/machine/debugger/Debugger.hpp
        src/nes/machine/debugger/Disassembler.cpp
        src/nes/machine/debugger/Disassembler.hpp
        src/nes/machine/debugger/DisassemblerConst.cpp
        src/nes/machine/debugger/MemoryComparator.cpp
        src/nes/machine/debugger/MemoryComparator.hpp
        src/nes/machine/debugger/Watcher.cpp
        src/nes/machine/debugger/Watcher.hpp
        src/nes/machine/exception/EmulatorException.cpp
        src/nes/machine/exception/EmulatorException.hpp
        src/nes/machine/fairy/AudioFairy.hpp
        src/nes/machine/fairy/DebuggerFairy.hpp
        src/nes/machine/fairy/FairyConstantValues.cpp
        src/nes/machine/fairy/GamepadFairy.hpp
        src/nes/machine/fairy/VideoFairy.hpp
        src/nes/machine/file/NesFile.cpp
        src/nes/machine/file/NesFile.hpp
        src/nes/machine/mapper/board/VRC4.cpp
        src/nes/machine/mapper/board/VRC4.hpp
        src/nes/machine/mapper/Mapper0.cpp
        src/nes/machine/mapper/Mapper0.hpp
        src/nes/machine/mapper/Mapper1.cpp
        src/nes/machine/mapper/Mapper1.hpp
        src/nes/machine/mapper/Mapper2.cpp
        src/nes/machine/mapper/Mapper2.hpp
        src/nes/machine/mapper/Mapper21.cpp
        src/nes/machine/mapper/Mapper21.hpp
        src/nes/machine/mapper/Mapper23.cpp
        src/nes/machine/mapper/Mapper23.hpp
        src/nes/machine/mapper/Mapper25.cpp
        src/nes/machine/mapper/Mapper25.hpp
        src/nes/machine/mapper/Mapper3.cpp
        src/nes/machine/mapper/Mapper3.hpp
        src/nes/machine/mapper/Mapper4.cpp
        src/nes/machine/mapper/Mapper4.hpp
        src/nes/machine/Audio.cpp
        src/nes/machine/AudioChannel.hpp
        src/nes/machine/Cartridge.cpp
        src/nes/machine/IOPort.cpp
        src/nes/machine/Processor.cpp
        src/nes/machine/Ram.cpp
        src/nes/machine/Video.cpp
        src/nes/machine/VirtualMachine.cpp
        src/nes/machine/VirtualMachine.hpp
        src/nes/widget/NesMemoryCompare.cpp
        src/nes/widget/NesMemoryCompare.hpp
        src/nes/widget/NesMemoryWidget.cpp
        src/nes/widget/NesMemoryWidget.hpp
        src/nes/widget/NesScreenWidget.cpp
        src/nes/widget/NesScreenWidget.hpp
        src/nes/widget/NesTraceWidget.cpp
        src/nes/widget/NesTraceWidget.hpp
        src/nes/Hexe.cpp
        src/nes/Hexe.hpp
        src/nes/NesFairy.cpp
        src/nes/NesGeist.cpp
        src/nes/NesGeist.hpp
)

set(TEST_SOURCE_FILES
    external/gtest/googletest/src/gtest-all.cc
    test/chisa/audio/SoundSpecTest.cpp
    test/chisa/doc/dispose/TextContextTest.cpp
    test/chisa/doc/node/NodeReaderTest.cpp
    test/chisa/doc/UtilTest.cpp
    test/chisa/geom/AreaTest.cpp
    test/chisa/geom/GeomSerializeTest.cpp
    test/chisa/geom/MatrixTest.cpp
    test/chisa/geom/MinMaxTest.cpp
    test/chisa/geom/SpaceTest.cpp
    test/chisa/geom/VectorTest.cpp
    test/chisa/gl/BufferTest.cpp
    test/chisa/gl/ColorTest.cpp
    test/chisa/gl/DrawableTest.cpp
    test/chisa/gl/FontTest.cpp
    test/chisa/gl/OrderTest.cpp
    test/chisa/gl/UtilTest.cpp
    test/chisa/logging/ExceptionTest.cpp
    test/chisa/logging/LoggingTest.cpp
    test/chisa/tk/ContentMeurerTest.cpp
    test/chisa/tk/SplitComboTest.cpp
    test/chisa/tk/TaskHandlerTest.cpp
    test/chisa/tk/WidgetTest.cpp
    test/chisa/HandlerTest.cpp

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
pkg_check_modules(LIBPNG REQUIRED libpng)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)
pkg_check_modules(SDL2 REQUIRED sdl2)

link_directories(${LIBPNG_LIBRARY_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})
link_directories(${FREETYPE2_LIBRARY_DIRS})
link_directories(${ICU4C_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
find_package(ANTLR REQUIRED)

###############################################################################
#### cinamo lib
###############################################################################
add_subdirectory("external/tinyxml2")
add_library(cinamo STATIC ${CINAMO_SOURCE_FILES})
target_include_directories(cinamo PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(cinamo PUBLIC "external/tinyxml2")
target_link_libraries(cinamo tinyxml2)

###############################################################################
#### donut lib
###############################################################################
add_library(donut STATIC ${DONUT_SOURCE_FILES})
target_include_directories(donut PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(donut PUBLIC ${ANTLR_INCLUDE_DIRS})
target_include_directories(donut PUBLIC "external/tinyxml2")

###############################################################################
#### chisa lib
###############################################################################
add_library(chisa STATIC ${CHISA_SOURCE_FILES})
target_include_directories(chisa PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(chisa PUBLIC ${LIBPNG_INCLUDE_DIRS})
target_include_directories(chisa PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(chisa PUBLIC ${FREETYPE2_INCLUDE_DIRS})
target_include_directories(chisa PUBLIC "external/tinyxml2")

###############################################################################
#### chisa app
###############################################################################
add_executable(chisaApp ${CHISA_APP_SOURCE_FILES})
target_link_libraries(chisaApp cinamo)
target_link_libraries(chisaApp donut)
target_link_libraries(chisaApp chisa)
target_include_directories(chisaApp PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(chisaApp PUBLIC ${LIBPNG_INCLUDE_DIRS})
target_include_directories(chisaApp PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(chisaApp PUBLIC ${FREETYPE2_INCLUDE_DIRS})
target_include_directories(chisaApp PUBLIC "external/tinyxml2")
target_link_libraries(chisaApp ${LIBPNG_LIBRARIES})
target_link_libraries(chisaApp ${ICU4C_LIBRARIES})
target_link_libraries(chisaApp ${FREETYPE2_LIBRARIES})
target_link_libraries(chisaApp ${OPENGL_LIBRARIES})
target_link_libraries(chisaApp ${ANTLR_LIBRARIES})
target_link_libraries(chisaApp ${SDL2_LIBRARIES})
target_link_libraries(chisaApp pthread)

###############################################################################
#### donut app
###############################################################################
add_executable(donutApp ${DONUT_APP_SOURCE_FILES})
target_link_libraries(donutApp cinamo)
target_link_libraries(donutApp donut)
target_include_directories(donutApp PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(donutApp PUBLIC ${LIBPNG_INCLUDE_DIRS})
target_include_directories(donutApp PUBLIC ${FREETYPE2_INCLUDE_DIRS})
target_include_directories(donutApp PUBLIC "external/tinyxml2")
target_link_libraries(donutApp ${LIBPNG_LIBRARIES})
target_link_libraries(donutApp ${ICU4C_LIBRARIES})
target_link_libraries(donutApp ${FREETYPE2_LIBRARIES})
target_link_libraries(donutApp ${OPENGL_LIBRARIES})
target_link_libraries(donutApp ${ANTLR_LIBRARIES})

###############################################################################
#### test app
###############################################################################
add_executable(testApp ${TEST_SOURCE_FILES})
target_link_libraries(testApp cinamo)
target_link_libraries(testApp donut)
target_link_libraries(testApp chisa)
target_include_directories(testApp PUBLIC ${ICU4C_INCLUDE_DIRS})
target_include_directories(testApp PUBLIC ${LIBPNG_INCLUDE_DIRS})
target_include_directories(testApp PUBLIC ${FREETYPE2_INCLUDE_DIRS})
target_include_directories(testApp PUBLIC "external/tinyxml2")
target_link_libraries(testApp ${LIBPNG_LIBRARIES})
target_link_libraries(testApp ${ICU4C_LIBRARIES})
target_link_libraries(testApp ${FREETYPE2_LIBRARIES})
target_link_libraries(testApp ${OPENGL_LIBRARIES})
target_link_libraries(testApp ${ANTLR_LIBRARIES})
target_link_libraries(testApp ${SDL2_LIBRARIES})
target_link_libraries(testApp pthread)
