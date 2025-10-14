#pragma once
#include <JuceHeader.h>

class MainComponent : public juce::Component
{
public:
    MainComponent();
    void resized() override;

private:
    juce::Slider volumeSlider;
    juce::TextButton muteButton;

    bool isMuted = false;
    double previousVolume = 0.5;

    void toggleMuted();  // ??? ?????? ??????

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};