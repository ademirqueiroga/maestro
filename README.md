# Maestro

Maestro is an Android application that demonstrates synchronized audio playback using the [Oboe Audio Library](https://developer.android.com/games/sdk/oboe). The app is designed to play multiple audio tracks simultaneously while keeping them perfectly synchronized, making it ideal for applications like multi-track music players, stem-based mixing, and audio production tools.

## Features

- **Multi-track Playback**: Play multiple audio tracks simultaneously with perfect synchronization
- **Individual Track Control**: Mute/unmute individual tracks during playback
- **Seeking**: Seek through the entire composition while maintaining synchronization
- **Low Latency**: Leverages Android's Oboe library for high-performance, low-latency audio
- **Native Performance**: Uses C++ for audio processing with JNI bridging to the Android UI

## Technical Overview

The project is structured as follows:

- **Native Layer (C++)**
  - Uses the Oboe library for low-latency audio playback
  - Implements a custom audio engine in `Maestro.cpp`/`Maestro.h`
  - Handles audio data extraction and processing
  - Provides JNI interface through `maestro-bridge.cpp`

- **Android Layer (Kotlin)**
  - Provides a simple UI to control playback
  - Handles native library initialization
  - Manages audio lifecycle events

## Requirements

- Android Studio 4.1+
- Android SDK 21+
- NDK 25.2.9519653
- CMake 3.10.2+
- FFmpeg libraries for audio decoding

## Building the Project

1. Clone the repository
2. Open the project in Android Studio
3. Make sure NDK is installed through the SDK Manager
4. Build and run the project on an Android device

## Usage

The sample app demonstrates multiple audio tracks (stems) that can be individually controlled:

- Tap on a track's play/pause button to toggle that track
- Use the main playback controls to play/pause all tracks
- Use the seek bar to navigate through the tracks

## Implementation Details

Maestro synchronizes audio playback by:

1. Loading all audio tracks into memory
2. Creating an Oboe audio stream
3. Mixing audio data from multiple sources in real-time
4. Maintaining a single playback position for all tracks

## License

This project is provided as an example implementation and proof of concept.