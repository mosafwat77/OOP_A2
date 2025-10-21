#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(playerGUI);
    setSize(700, 200);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerGUI.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerGUI.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    playerGUI.releaseResources();
}

void MainComponent::resized()
{

    playerGUI.setBounds(getLocalBounds().reduced(10));
}
