# FM Synth
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/:user/:repo/:workflow)

A simple yet expressive FM synthesizer implemented using LLDSP & JUCE.
Currently the architecture is a carrier with a single modulator where the modulators waveform matches that of the carrier. The plugin implements a simple biquad low-pass filter with its own envelope. Finally there is a distortion and my schroeder reverb implementation.

![Recording 2025-05-05 at 13 00 22](https://github.com/user-attachments/assets/ba7b4ee4-d4c5-4e2d-9a43-f4b4e043feb0)

## Running + Building
Currently the project supports Standalone and VST3 formats. You can either grab one of the releases or build from source.
While AU and MacOS support is not currently provided I plan on working on this in the future.

## Credits
I have recently migrated from using the Projucer to CMake. For this I have used the amazing [pamplejuce](https://github.com/sudara/pamplejuce) project along with it's GitHub actions.


