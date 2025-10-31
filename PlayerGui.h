#pragma once
#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGui();
    ~PlayerGui() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void timerCallback() override;

private:
    Playeraudio playerAudio;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton loopButton{ "Loop: OFF" };

    juce::Slider volumeSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::Label speedLabel;
    juce::Slider speedSlider;
    bool fileLoaded = false;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
