#pragma once
#include <JuceHeader.h>

class Playeraudio
{
public:
    Playeraudio();
    ~Playeraudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

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

private:
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    bool muted = false;
    bool isLooping = false;
    float lastGain = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Playeraudio)
};
