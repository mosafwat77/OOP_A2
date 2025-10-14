#include "MainComponent.h"

MainComponent::MainComponent()
{
    // ????? volumeSlider
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);

    // ????? muteButton
    addAndMakeVisible(muteButton);
    muteButton.setButtonText("Mute");
    muteButton.onClick = [this] { toggleMuted(); };

    setSize(400, 300);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    muteButton.setBounds(area.removeFromTop(50).reduced(10));
    volumeSlider.setBounds(area.reduced(10));
}

void MainComponent::toggleMuted()  // ??? ?????? ??????
{
    if (!isMuted)
    {
        previousVolume = volumeSlider.getValue();
        volumeSlider.setValue(0.0);
        muteButton.setButtonText("Unmute");  // muteButton ???? autoButton
        isMuted = true;
    }
    else
    {
        volumeSlider.setValue(previousVolume);
        muteButton.setButtonText("Mute");  // muteButton ???? autoButton
        isMuted = false;  // ????? ??? ?????!
    }
}