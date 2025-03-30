# FM Synth
## Written using the LLDSP library in JUCE.

Currently the architecture is a carrier with a single modulator. The modulators waveform matches that of the carrier. For effects there is a low-pass filter with an optional envelope to control the filter. Finally there is a simple tanh waveshaper distortion and my schroeder reverb implementation.
