# Project components

- Electronics:
    - Piezo input circuit.
    - Analog input circuit (for hi-hat controller).
    - MIDI output circuit.
    - (Nice to have) MIDI-over-USB output circuit
- Embedded software:
    - Reading the piezo inputs with ADCs.
    - Convert the read piezo values and convert them to MIDI notes.
    - Send the MIDI notes MIDI output.
    - (Nice to have) Set up the microcontroller as a recognizable MIDI device over USB.
    - Allows for configuration changes from the configuration application.
- Application software:
    - Configuration application to map and finetune piezo inputs to MIDI notes.
- CAD:
    - Enclosure for the electronics.

# Assumptions and things to keep in mind

- The inputs are 1/4" TRS (stereo) female connectors.
- Inputs may either be mono (TS) or stereo (TRS). Each input can be configured seperately.
- The piezo circuit will clip the voltage between GND and the maximum toleratated input voltage by the input pin. Some scaling (potentiometer) will be possible on the input. If more scaling is required, it's up to the circuit of the source of the signals.
- The device will require a separate power supply.
- The harware and software should be designed such that more inputs could easily be added.
