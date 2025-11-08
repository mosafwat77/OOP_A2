# 🎵 Simple Audio Player

A feature-rich audio player built with JUCE framework, offering professional playback controls, waveform visualization, playlist management, and advanced features like A-B looping and smart ducking.

![Audio Player Interface](screenshot.png)

## 📋 Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Building from Source](#building-from-source)
- [User Guide](#user-guide)
- [Architecture](#architecture)
- [Key Features Explained](#key-features-explained)
- [Session Persistence](#session-persistence)
- [Contributing](#contributing)
- [License](#license)

---

## ✨ Features

### Core Playback
- ✅ Play, pause, stop, and restart controls
- ✅ Load audio files (WAV, MP3, AIFF, FLAC)
- ✅ Real-time waveform visualization
- ✅ Position scrubbing with visual feedback
- ✅ Volume control with mute function
- ✅ Playback speed adjustment (0.5x - 2.0x)

### Advanced Features
- 🔁 **A-B Loop**: Set loop points for repetitive practice
- 🦆 **Smart Duck**: Automatic volume ducking when adding markers
- 📌 **Markers**: Add, delete, and navigate to specific positions
- 📂 **Playlist Management**: Multi-file playlist with quick switching
- 💾 **Session Persistence**: Automatically saves and restores your session
- 📊 **Live Spectrum Visualization**: Real-time audio frequency display
- 📝 **Metadata Display**: View file information and audio properties

---

## 💻 System Requirements

### Minimum Requirements
- **OS**: Windows 10/11, macOS 10.13+, or Linux (Ubuntu 18.04+)
- **RAM**: 4GB
- **Storage**: 50MB for application + space for audio files
- **Audio**: Any audio output device

### Dependencies
- **JUCE Framework**: Version 7.0+ (included in build)
- **C++ Compiler**: 
  - Windows: Visual Studio 2019+
  - macOS: Xcode 12+
  - Linux: GCC 7+ or Clang 6+

---

## 🚀 Installation

### Pre-built Binaries
Download the latest release for your platform from the [Releases](https://github.com/yourusername/simple-audio-player/releases) page.

### Windows
1. Download `SimpleAudioPlayer-Windows.zip`
2. Extract to desired location
3. Run `SimpleAudioPlayer.exe`

### macOS
1. Download `SimpleAudioPlayer-macOS.dmg`
2. Open DMG and drag app to Applications folder
3. Run from Applications

### Linux
1. Download `SimpleAudioPlayer-Linux.tar.gz`
2. Extract: `tar -xzf SimpleAudioPlayer-Linux.tar.gz`
3. Make executable: `chmod +x SimpleAudioPlayer`
4. Run: `./SimpleAudioPlayer`

---

## 🛠️ Building from Source

### Prerequisites
1. Install [JUCE](https://juce.com/get-juce/download) framework
2. Install [CMake](https://cmake.org/download/) (3.15+)
3. Install a C++17 compatible compiler

### Clone Repository
```bash
https://github.com/mosafwat77/OOP_A21
cd simple-audio-player
```

### Build Steps

#### Using CMake (All Platforms)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Using Projucer (JUCE's IDE)
1. Open `SimpleAudioPlayer.jucer` in Projucer
2. Set your target platform (Visual Studio/Xcode/Linux Makefile)
3. Click "Save and Open in IDE"
4. Build from your IDE

---

## 📖 User Guide

### Basic Playback

#### Loading Audio Files
1. Click **"Load"** button
2. Select an audio file (WAV, MP3, AIFF, or FLAC)
3. Waveform will appear in the visualization area

#### Playback Controls
- **Play/Pause**: Toggle playback
- **Stop**: Stop and reset to beginning
- **Restart**: Jump to start and play
- **Go To End**: Jump to end of track
- **±10s Buttons**: Skip forward/backward 10 seconds

#### Volume & Speed
- **Volume Slider**: Adjust playback volume (0-100%)
- **Mute Button**: Toggle mute on/off
- **Speed Slider**: Adjust playback speed (0.5x - 2.0x)
- **Position Slider**: Click or drag to scrub through audio

---

### Advanced Features

#### 🔁 A-B Loop (Practice Mode)
Perfect for musicians practicing difficult sections!

1. Play the audio to the section you want to loop
2. Click **"Set A"** at the start of the section → Green line appears
3. Let audio play to the end of the section
4. Click **"Set B"** at the end → Orange line appears
5. Loop will automatically engage, repeating between A and B
6. Click **"Clear Loop"** to disable looping

**Visual Indicators:**
- 🟢 Green line = Loop start (Point A)
- 🟠 Orange line = Loop end (Point B)
- 🟨 Yellow highlight = Looped region

**Smart Features:**
- Automatically swaps A and B if set in wrong order
- Works with standard looping (Loop button)
- Visual feedback in waveform display

---

#### 📌 Markers
Bookmark important positions in your audio!

1. Play audio to desired position
2. Click **"Add Marker"** → Cyan marker appears
3. Double-click marker in list to jump to that position
4. Select marker and click **"Delete Marker"** to remove
5. Select marker and click **"Go To"** to navigate

**Marker Features:**
- Automatically named with timestamp
- Sorted chronologically
- Visual display on waveform (cyan circles)
- Saved in session data

---

#### 🦆 Smart Duck
Automatically lowers volume briefly when adding markers, drawing attention to the action.

1. Click **"Smart Duck: OFF"** to enable
2. Button text changes to **"Smart Duck: ON"**
3. When you add a marker, volume ducks to 30% for 0.3 seconds
4. Volume automatically restores after ducking period

**Use Case:** Helps you focus on marking positions without audio distraction.

---

#### 📂 Playlist Management
Manage multiple audio files in a queue!

1. Click **"Add Files"** to add one or more audio files
2. Files appear in the playlist box
3. Click any file in the playlist to load and play it
4. Currently playing file is highlighted in yellow
5. Select a file and click **"Remove"** to delete from playlist
6. Click **"Clear All"** to empty the playlist

**Playlist Features:**
- Multi-file selection supported
- Displays filename only (no full path)
- Visual indication of currently loaded track
- Playlist saved in session

---

### 📊 Waveform Display

The main visualization area shows:
- **Blue waveform**: Audio amplitude over time
- **Cyan spectrum bars**: Real-time frequency visualization (bottom of waveform)
- **Red playhead**: Current playback position
- **Green/Orange lines**: A-B loop points
- **Yellow region**: Active loop area
- **Cyan markers**: Bookmarked positions

**Interaction:**
- Click anywhere on the waveform to jump to that position

---

### 📝 Metadata Display

Below the waveform, you'll see file information:
- **Title/Filename**: Track name or file name
- **Artist**: Artist metadata (if available)
- **Album**: Album metadata (if available)
- **Format**: File type (MP3, WAV, etc.)
- **Sample Rate**: Audio quality (e.g., 44.1 kHz)
- **Bit Depth**: Audio resolution (e.g., 16 bit)
- **Channels**: Mono or Stereo
- **Duration**: Total track length

---

## 🏗️ Architecture

### Project Structure
```
SimpleAudioPlayer/
├── Main.cpp                 # Application entry point
├── MainComponent.h/cpp      # Audio system bridge
├── PlayerGui.h/cpp          # User interface & controls
├── Playeraudio.h/cpp        # Audio engine & playback
└── README.md                # This file
```

### Component Hierarchy
```
JUCEApplication (Main.cpp)
    └── MainWindow (DocumentWindow)
        └── MainComponent (AudioAppComponent)
            └── PlayerGui (Component + UI Controls)
                └── Playeraudio (Audio Engine)
```

---

## 🔧 Key Components Explained

### 1. Main.cpp
**Purpose:** Application lifecycle management

**Responsibilities:**
- Initialize JUCE application
- Create main window
- Handle application shutdown
- Save session on quit

**Key Classes:**
- `SimpleAudioPlayer`: Main application class
- `MainWindow`: Document window with title bar

---

### 2. MainComponent
**Purpose:** Bridge between audio system and GUI

**Responsibilities:**
- Initialize audio device (stereo output)
- Pass audio callbacks to GUI
- Manage audio processing lifecycle
- Coordinate session save/load

**Key Methods:**
- `prepareToPlay()`: Initialize audio system
- `getNextAudioBlock()`: Real-time audio callback (called continuously)
- `releaseResources()`: Cleanup audio resources

---

### 3. PlayerGui
**Purpose:** User interface and interaction logic

**Responsibilities:**
- UI layout and rendering
- Handle user input (buttons, sliders)
- Waveform visualization
- Playlist management
- Marker system
- A-B loop logic
- Smart duck effect
- Session persistence

**Key Methods:**
- `paint()`: Draw waveform, markers, loop indicators
- `resized()`: Layout UI components
- `buttonClicked()`: Handle all button interactions
- `sliderValueChanged()`: Handle volume/speed/position changes
- `timerCallback()`: Update UI 30 times/second, enforce loops

**Timer Responsibilities (30 Hz):**
- Update position slider
- Update time labels
- Enforce A-B loop
- Restore ducked volume
- Update spectrum visualization
- Repaint waveform

---

### 4. Playeraudio
**Purpose:** Core audio playback engine

**Responsibilities:**
- Load audio files
- Manage playback state
- Control volume and speed
- Position control
- Looping functionality

**Audio Processing Chain:**
```
Audio File
    ↓
AudioFormatReaderSource (reads file)
    ↓
AudioTransportSource (playback control)
    ↓
ResamplingAudioSource (speed control)
    ↓
Audio Output (speakers/headphones)
```

**Key Components:**
- `AudioFormatManager`: Handles MP3, WAV, AIFF, FLAC formats
- `AudioTransportSource`: Controls play/pause/stop/position
- `ResamplingAudioSource`: Enables speed changes without pitch shift
- `AudioFormatReaderSource`: Reads audio data from file

---

## 🎯 Key Features Deep Dive

### A-B Loop Implementation

**How it works:**
1. User sets point A at time `loopStartTime`
2. User sets point B at time `loopEndTime`
3. Timer checks playback position every frame (30 times/second)
4. If position ≥ B or position < A, jump back to A
5. Creates seamless loop

**Code Flow:**
```cpp
// In timerCallback():
if (abLoopEnabled && loopStartSet && loopEndSet)
{
    if (currentTime >= loopEndTime || currentTime < loopStartTime)
    {
        playerAudio.setPosition(loopStartTime);
    }
}
```

**Smart Features:**
- Auto-swaps A and B if set backwards
- Visual feedback (green, orange lines, yellow region)
- Works with standard loop button

---

### Smart Duck Effect

**How it works:**
1. User enables Smart Duck feature
2. User adds a marker
3. System saves current volume
4. Volume reduced to 30% immediately
5. Timer tracks elapsed time
6. After 0.3 seconds, volume restored

**Timeline:**
```
Time:    0.0s    →    0.3s
Volume:  100%  →  30%  →  100%
         ↑            ↑
      Marker     Restore
       Added
```

**Implementation:**
- `triggerVolumeDuck()`: Initiates duck
- `timerCallback()`: Monitors elapsed time
- `volumeDuckActive`: Tracks duck state

---

### Waveform Visualization

**Components:**
- `AudioThumbnail`: JUCE class for waveform rendering
- `AudioThumbnailCache`: Caches waveform data
- `thumbnail.drawChannel()`: Renders waveform

**Visual Elements:**
1. **Waveform**: Blue amplitude visualization
2. **Spectrum**: Cyan bars (simulated frequency display)
3. **Playhead**: Red vertical line at current position
4. **Loop Region**: Yellow highlight between A-B
5. **Loop Points**: Green (A) and orange (B) lines
6. **Markers**: Cyan circles with vertical lines

**Coordinate Mapping:**
```cpp
// Convert time to pixel position
double xPos = juce::jmap(currentTime, 0.0, duration,
    waveformArea.getX(), waveformArea.getRight());
```

---

### Session Persistence

**What's Saved:**
- Current track and playback position
- Volume and speed settings
- Mute state
- Smart Duck enabled/disabled
- Playlist contents
- Markers (time and labels)

**Storage Location:**
- **Windows**: `%APPDATA%\SimpleAudioPlayer\session.xml`
- **macOS**: `~/Library/Application Support/SimpleAudioPlayer/session.xml`
- **Linux**: `~/.config/SimpleAudioPlayer/session.xml`

**Format:** XML structure
```xml
<AudioPlayerSession>
    <CurrentTrack index="0" path="/path/to/file.mp3" position="45.3"/>
    <Settings volume="0.7" speed="1.0" muted="false" smartDuckEnabled="true"/>
    <Playlist>
        <File path="/path/to/file1.mp3"/>
        <File path="/path/to/file2.wav"/>
    </Playlist>
    <Markers>
        <Marker time="23.5" label="Marker 1 (0:23)"/>
        <Marker time="67.2" label="Marker 2 (1:07)"/>
    </Markers>
</AudioPlayerSession>
```

**When Saved:**
- When window is closed
- When application quits
- In destructor (backup)

**When Loaded:**
- On application startup
- Automatically restores last session

---

## 🎨 UI Design Patterns

### Observer Pattern (Event Handling)
```cpp
// Setup
button.addListener(this);  // Register observer

// Callback
void buttonClicked(Button* button) override {
    // Respond to event
}
```

### Timer-Based Updates
```cpp
startTimerHz(30);  // 30 frames per second

void timerCallback() override {
    // Update UI
    // Enforce logic
    // Manage effects
}
```

### State Management
```cpp
bool fileLoaded = false;
bool loopStartSet = false;
bool volumeDuckActive = false;
// State flags control behavior
```

---

## 🔊 Audio Processing Concepts

### Real-Time Audio Thread
- `getNextAudioBlock()` called by audio system
- Runs in high-priority thread
- Must be fast (no file I/O, no UI updates)
- Fills buffer with audio samples

### Buffer-Based Processing
- Audio delivered in blocks (e.g., 512 samples)
- Block size set by audio device
- Sample rate (e.g., 44100 Hz) determines timing

### Audio Source Chain
Multiple sources can be chained together:
```
File Reader → Transport → Resampling → Output
```
Each processes audio before passing to next.

---

## 🧪 Testing

### Manual Testing Checklist
- [ ] Load different audio formats (WAV, MP3, AIFF, FLAC)
- [ ] Test all playback controls
- [ ] Verify volume and speed adjustment
- [ ] Test A-B loop with various sections
- [ ] Add and navigate markers
- [ ] Test playlist management
- [ ] Verify session persistence (quit and restart)
- [ ] Test Smart Duck feature
- [ ] Verify waveform display updates

### Known Limitations
- Maximum file size depends on available RAM
- Very long files (>1 hour) may have slow thumbnail generation
- Spectrum visualization is simulated (not true FFT)

---

## 🤝 Contributing

Contributions are welcome! Here's how to get started:

### Reporting Bugs
1. Check existing issues first
2. Create new issue with:
   - Clear description
   - Steps to reproduce
   - Expected vs actual behavior
   - System information

### Feature Requests
1. Check existing feature requests
2. Describe the feature and use case
3. Explain why it would be useful

### Pull Requests
1. Fork the repository
2. Create feature branch: `git checkout -b feature/YourFeature`
3. Make your changes
4. Test thoroughly
5. Commit: `git commit -m 'Add YourFeature'`
6. Push: `git push origin feature/YourFeature`
7. Open Pull Request

### Code Style
- Follow existing code formatting
- Use descriptive variable names
- Comment complex logic
- Keep functions focused and small

---

## 📚 Additional Resources

### JUCE Framework
- [Official Documentation](https://docs.juce.com/)
- [JUCE Tutorials](https://juce.com/learn/tutorials)
- [JUCE Forum](https://forum.juce.com/)

### Audio Programming
- [The Audio Programming Book](http://www.musicdsp.org/)
- [Digital Signal Processing Guide](https://www.dspguide.com/)

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Acknowledgments

- **JUCE Framework** - For providing excellent audio framework
- **Contributors** - Thank you to all contributors
- **Community** - JUCE forum members for guidance

---

## 🗺️ Roadmap

### Future Features
- [ ] True FFT spectrum analyzer
- [ ] Keyboard shortcuts
- [ ] Themes/skins support
- [ ] Export loop sections
- [ ] Multiple A-B loops
- [ ] Waveform zoom
- [ ] Pitch shifting
- [ ] Audio effects (reverb, EQ)
- [ ] Recording functionality
- [ ] MIDI controller support


*Last Updated: November 2025*
