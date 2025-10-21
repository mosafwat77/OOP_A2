#include "PlayerGui.h"

PlayerGui::PlayerGui()
{
    for (auto* btn : { &loadButton, &playPauseButton, &stopButton, &restartButton, &incButton, &decButton, &muteButton, &loopButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

PlayerGui::~PlayerGui() {}

void PlayerGui::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGui::resized()
{
    int y = 20;
    int x = 20;
    int w = 80;
    int gap = 10;

    loadButton.setBounds(x, y, w, 40);
    playPauseButton.setBounds(x += w + gap, y, w, 40);
    stopButton.setBounds(x += w + gap, y, w, 40);
    restartButton.setBounds(x += w + gap, y, w, 40);
    decButton.setBounds(x += w + gap, y, w, 40);
    incButton.setBounds(x += w + gap, y, w, 40);
    muteButton.setBounds(x += w + gap, y, w, 40);
    loopButton.setBounds(x += w + gap, y, w, 40);

    volumeSlider.setBounds(20, 80, getWidth() - 40, 30);
}

void PlayerGui::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGui::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGui::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGui::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);
            });
    }
    else if (button == &playPauseButton)
    {
        if (!playerAudio.isPlaying())
        {
            playerAudio.start();
            playPauseButton.setButtonText("Pause");
        }
        else
        {
            playerAudio.pause();
            playPauseButton.setButtonText("Play");
        }
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
        playPauseButton.setButtonText("Play");
    }
    else if (button == &restartButton)
    {
        playerAudio.restart();
        playPauseButton.setButtonText("Pause");
    }
    else if (button == &incButton)
    {
        playerAudio.increment(5.0); 
    }
    else if (button == &decButton)
    {
        playerAudio.decrement(5.0); 
    }
    else if (button == &muteButton)
    {
        if (playerAudio.isMuted())
        {
            playerAudio.unmute();
            muteButton.setButtonText("Mute");
            volumeSlider.setValue(0.5f, juce::dontSendNotification);
        }
        else
        {
            playerAudio.mute();
            muteButton.setButtonText("Unmute");
            volumeSlider.setValue(0.0f, juce::dontSendNotification);
        }
    }
    else if (button == &loopButton)
    {
        loopButton.setToggleState(!loopButton.getToggleState(), juce::dontSendNotification);
        if (onLoopToggled)
            onLoopToggled(loopButton.getToggleState());
    }
}

void PlayerGui::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}
