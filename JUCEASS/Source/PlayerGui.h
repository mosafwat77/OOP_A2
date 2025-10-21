#pragma once
#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGui();
    ~PlayerGui() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    std::function<void(bool)> onLoopToggled;

private:
    Playeraudio playerAudio;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton incButton{ "|>" };
    juce::TextButton decButton{ "|<" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton loopButton{ juce::CharPointer_UTF8("\xE2\x86\xBB") };
    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
