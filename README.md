# FM Synth
A simple FM synthesizer implemented using LLDSP & JUCE.

![Recording 2025-04-03 at 11 44 26](https://github.com/user-attachments/assets/2fa75f2b-2cc3-412a-8e21-6552d3ead113)

Currently the architecture is a carrier with a single modulator where the modulators waveform matches that of the carrier. The plugin implements a simple biquad low-pass filter with its own envelope. Finally there is a simple tanh waveshaper distortion and my schroeder reverb implementation.

## TODO
- Change font of JUCE rendered text to match UI assets.
- Investigate ways of creating more interesting sounds with the simple two operator architecture.
- Add envelope for pitch.

