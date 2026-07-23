file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    *.h
    *.cpp)

add_executable(InstrumentApp ${SOURCES})

set_target_properties(InstrumentApp PROPERTIES SUFFIX ".elf")

target_link_libraries(InstrumentApp
    PRIVATE
        ChannelMapping
        PiezoPeakDetector
        Communication
        ADC
        MIDI
        Interrupt
        Timing
)