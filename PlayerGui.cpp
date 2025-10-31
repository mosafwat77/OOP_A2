#include "PlayerGui.h"

PlayerGui::PlayerGui()
    : thumbnailCache(5),
    thumbnail(512, playerAudio.getFormatManager(), thumbnailCache)
{
    // Initialize buttons
    loadButton.setButtonText("Load");
    playPauseButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    restartButton.setButtonText("Restart");
    muteButton.setButtonText("Mute");
    loopButton.setButtonText("Loop: OFF");

    // Add buttons
    for (auto* btn : { &loadButton, &playPauseButton, &stopButton, &restartButton, &muteButton, &loopButton })
    {
        addAndMakeVisible(*btn);
        btn->addListener(this);

    }
    for (auto* label : { &speedLabel })
    {
        label->setJustificationType(juce::Justification::centredLeft);
        label->setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(label);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // Timer for updating playhead
    startTimerHz(30);

    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(1.0);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedSlider.addListener(this);


}

PlayerGui::~PlayerGui() {
    speedSlider.removeListener(this);
}

void PlayerGui::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    auto waveformArea = getLocalBounds().reduced(20, 120);

    if (fileLoaded && thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::lightgreen);
        thumbnail.drawChannel(g, waveformArea, 0.0, thumbnail.getTotalLength(), 0, 0.3f);

        // Playhead line
        double duration = playerAudio.getTrackDuration();
        double position = playerAudio.getPlaybackTime();

        if (duration > 0.0)
        {
            double progress = position / duration;
            int playheadX = waveformArea.getX() + (int)(progress * waveformArea.getWidth());

            g.setColour(juce::Colours::red);
            g.drawLine((float)playheadX, (float)waveformArea.getY(),
                (float)playheadX, (float)waveformArea.getBottom(), 2.0f);
        }
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("Load an audio file to see waveform",
            waveformArea, juce::Justification::centred, 1);
    }
}

void PlayerGui::resized()
{
    int y = 20;
    int x = 20;
    int w = 80;
    int gap = 10;
    auto area = getLocalBounds().reduced(20);
    int sliderHeight = 36;

    loadButton.setBounds(x, y, w, 40);
    playPauseButton.setBounds(x += w + gap, y, w, 40);
    stopButton.setBounds(x += w + gap, y, w, 40);
    restartButton.setBounds(x += w + gap, y, w, 40);
    muteButton.setBounds(x += w + gap, y, w, 40);
    loopButton.setBounds(x += w + gap, y, w, 40);

    volumeSlider.setBounds(20, 80, getWidth() - 40, 30);
    speedSlider.setBounds(100, area.getY() - 10, getWidth() - 120, sliderHeight);
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
                {
                    thumbnail.clear(); // امسح القديم قبل تحميل الجديد
                    playerAudio.loadFile(file);
                    thumbnail.setSource(new juce::FileInputSource(file));
                    fileLoaded = true;
                    repaint();
                }
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
        bool shouldLoop = !playerAudio.isLoopingEnabled();
        playerAudio.setLooping(shouldLoop);
        loopButton.setButtonText(shouldLoop ? "Loop: ON" : "Loop: OFF");
    }
}

void PlayerGui::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue()); 

}

void PlayerGui::timerCallback()
{
    if (fileLoaded && playerAudio.isPlaying())
        repaint();
}
