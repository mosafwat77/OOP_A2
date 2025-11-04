#pragma once
#include <JuceHeader.h>
#include <vector>
#include <algorithm>

struct Marker
{
    double timestamp;
    juce::String name;
    juce::Colour colour;
    bool isLoopStart = false;
    bool isLoopEnd = false;

    Marker(double time, const juce::String& markerName = "")
        : timestamp(time), name(markerName), colour(juce::Colours::orange)
    {
        if (name.isEmpty())
        {
            auto minutes = static_cast<int>(time) / 60;
            auto seconds = static_cast<int>(time) % 60;
            name = juce::String::formatted("Marker %02d:%02d", minutes, seconds);
        }
    }

    bool operator<(const Marker& other) const { return timestamp < other.timestamp; }

    juce::String getDisplayString() const
    {
        auto minutes = static_cast<int>(timestamp) / 60;
        auto seconds = static_cast<int>(timestamp) % 60;
        auto millis = static_cast<int>((timestamp - static_cast<int>(timestamp)) * 1000);

        juce::String prefix;
        if (isLoopStart) prefix = "↻ ";
        else if (isLoopEnd) prefix = "↺ ";

        return prefix + juce::String::formatted("%02d:%02d.%03d - ", minutes, seconds, millis) + name;
    }
};

class Playeraudio : public juce::AudioSource,
    public juce::ChangeBroadcaster
{
public:
    Playeraudio();
    ~Playeraudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadFile(const juce::File& audioFile);
    void start();
    void stop();
    void pause();
    void restart();

    void setGain(float gain);
    void mute();
    void unmute();
    bool isMuted() const;

    void increment(double seconds);
    void decrement(double seconds);

    void setPosition(double positionInSeconds);
    double getPosition() const;
    double getLength() const;

    void setLooping(bool shouldLoop);
    bool isLoopingEnabled() const { return isLooping; }
    bool isPlaying() const;
    void enableLooping(bool shouldLoop);
    bool isTrackPlaying() const;
    double getPlaybackTime() const;
    double getTrackDuration() const;
    juce::AudioFormatManager& getFormatManager() { return formatManager; }

    void setSpeed(double ratio);

    void addMarker(double timestamp, const juce::String& name = "");
    void addMarkerAtCurrentPosition(const juce::String& name = "");
    void removeMarker(int index);
    void removeAllMarkers();
    void jumpToMarker(int index);
    void jumpToMarkerTime(double timestamp);
    void renameMarker(int index, const juce::String& newName);

    void setLoopStartMarker(int index);
    void setLoopEndMarker(int index);
    void clearLoopMarkers();

    const std::vector<Marker>& getMarkers() const { return markers; }
    int findMarkerAtTime(double time, double tolerance = 0.1) const;

    void sendMarkerUpdate() { sendChangeMessage(); }

private:
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    float lastVolumeBeforeMute = 0.5f;
    bool muted = false;
    bool isLooping = false;
    float lastGain = 1.0f;

    std::vector<Marker> markers;
    int loopStartMarkerIndex = -1;
    int loopEndMarkerIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Playeraudio)
};
