#pragma once

#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::AudioAppComponent,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGui();
    ~PlayerGui() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    juce::String formatTime(double seconds);

    void setLoopStart();
    void setLoopEnd();
    void clearLoopMarkers();

    Playeraudio playerAudio;

    juce::TextButton loadButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton restartButton;
    juce::TextButton muteButton;
    juce::TextButton loopButton;
    juce::TextButton loopStartButton{ "A" };
    juce::TextButton loopEndButton{ "B" };
    juce::TextButton clearLoopButton{ "Clear Loop" };
    juce::TextButton forwardButton{ "|> " };
    juce::TextButton backwardButton{ "|< " };


    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    juce::Label speedLabel;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::SpinLock thumbnailLock;

    std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileLoaded = false;
    float previousVolume = 0.5f; 


    double loopStartTime = 0.0;
    double loopEndTime = 0.0;
    bool loopStartSet = false;
    bool loopEndSet = false;
    bool abLoopEnabled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
