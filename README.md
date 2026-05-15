# DIYDECK: STM32 Audio Effects Processor with Custom PCB

DIYDECK is a custom audio-processing board/system that takes an external audio signal, conditions it for an STM32 ADC, applies real-time embedded DSP effects, and outputs the processed signal through the STM32 DAC into the audio board/amplifier path.

[Demo](https://youtu.be/CMMS7bL4tLE)

## Features

- STM32 Nucleo-F303RE firmware using PlatformIO
- Real-time ADC-to-DAC audio passthrough
- Timer-driven sampling using TIM3
- Freeverb-style reverb
- Ring modulation
- Rotary encoder control for PWM-selected filter settings
- Mute switch and button-controlled effects
- Custom KiCad PCB files included

## System overview

Input audio is AC-coupled and biased around mid-supply so the STM32 ADC can sample the waveform safely. The firmware removes the measured DC bias, applies optional effects, recenters the signal around the DAC midpoint, and outputs it through DAC channel 1.

Signal chain:

Laptop / audio source  
→ AC coupling and biasing  
→ STM32 ADC input  
→ real-time DSP  
→ STM32 DAC output  
→ PCB audio path / amplifier / speaker

## Hardware

- STM32 Nucleo-F303RE
- ADC input: A0 / PA0 / ADC1_IN1
- DAC output: A2 / PA4 / DAC_OUT1
- Rotary encoder for filter selection
- Effect buttons for reverb and ring modulation
- Mute switch
- Custom PCB designed in KiCad

## Firmware architecture

- `src/main.cpp`: ADC/DAC setup, audio ISR, effect toggles, timer setup
- `src/encoder_control.cpp`: rotary encoder decoding and PWM setting selection
- `lib/my_reverb`: Freeverb-style reverb implementation
- `lib/my_ringmod`: ring modulation effect

## Audio processing

Each sample is read from the ADC, denoised slightly by masking noisy LSBs, centered by subtracting a moving DC estimate, optionally processed through reverb and/or ring modulation, clipped to the DAC range, and written to the DAC.

## Filter / PWM control

The rotary encoder selects one of 16 predefined cutoff settings. Each setting maps a target cutoff frequency to two PWM duty cycles used by the external board.

## Debugging and validation

During bring-up, I debugged DC offset, clipping, low output volume, scratchiness, sample-rate instability, and ADC/DAC scaling. I used serial diagnostics and lab equipment including an oscilloscope, multimeter, function generator, and bench power supply to validate the signal path.

## Known limitations

- Audio quality depends heavily on input biasing and gain staging
- Some sample rates produced more noise or unstable effect behavior
- Reverb quality is constrained by STM32 memory and real-time processing budget

## Build and upload

```bash
pio run
pio run --target upload
pio device monitor
