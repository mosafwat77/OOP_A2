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
    bool isPlaying() const;

    void increment(double seconds);
    void decrement(double seconds);
    void setPosition(double positionInSeconds);

    void setGain(float gain);
    double getPosition() const;
    double getLength() const;

    void mute();
    void unmute();
    bool isMuted() const;

    void setLooping(bool shouldLoop);
    bool isLoopingEnabled() const { return isLooping; }

private:
    juce::AudioFormatManager formatManager;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    bool muted = false;
    bool isLooping = false;
    float lastGain = 0.5f;
};
