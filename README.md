# FM Synth
A simple yet expressive FM synthesizer implemented using LLDSP & JUCE.

![Recording 2025-05-05 at 13 00 22](https://github.com/user-attachments/assets/ba7b4ee4-d4c5-4e2d-9a43-f4b4e043feb0)

Currently the architecture is a carrier with a single modulator where the modulators waveform matches that of the carrier. The plugin implements a simple biquad low-pass filter with its own envelope. Finally there is a distortion and my schroeder reverb implementation.
