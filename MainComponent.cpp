#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(playerGUI);
    setSize(800, 700);
    setAudioChannels(0, 2);

    loadSession();
}

MainComponent::~MainComponent()
{
    saveSession();
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

void MainComponent::saveSession()
{
    playerGUI.saveSession();
}

void MainComponent::loadSession()
{
    playerGUI.loadSession();
}