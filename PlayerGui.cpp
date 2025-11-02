#include "PlayerGui.h"

PlayerGui::PlayerGui()
    : thumbnailCache(5),
    thumbnail(512, playerAudio.getFormatManager(), thumbnailCache)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loopStartButton);
    addAndMakeVisible(loopEndButton);
    addAndMakeVisible(clearLoopButton);
   
    loadButton.setButtonText("Load");
    playPauseButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    restartButton.setButtonText("Restart");
    muteButton.setButtonText("Mute");
    loopButton.setButtonText("Loop: OFF");
    loopStartButton.setButtonText("Set A");
    loopEndButton.setButtonText("Set B");
    clearLoopButton.setButtonText("Clear Loop"); 

    for (auto* btn : { &loadButton, &playPauseButton, &stopButton, &restartButton,
                      &muteButton, &loopButton, &loopStartButton, &loopEndButton, &clearLoopButton
     
        })
    {
        btn->addListener(this);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setSliderStyle(juce::Slider::LinearBar);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    currentTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::right);
    totalTimeLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);

    speedLabel.setText("Speed:", juce::dontSendNotification);
    addAndMakeVisible(speedLabel);

    startTimerHz(30);
}

PlayerGui::~PlayerGui() {}

void PlayerGui::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    auto waveformArea = getLocalBounds().reduced(20, 250);

    if (fileLoaded && thumbnail.getNumChannels() > 0)
    {
        const juce::SpinLock::ScopedLockType lock(thumbnailLock);

        g.setColour(juce::Colours::lightgreen);
        thumbnail.drawChannel(g, waveformArea, 0.0, thumbnail.getTotalLength(), 0, 0.3f);
        if (loopStartSet && loopEndSet && abLoopEnabled)
        {
            double duration = playerAudio.getTrackDuration();
            if (duration > 0.0)
            {
                auto loopStartX = juce::jmap(loopStartTime, 0.0, duration,
                    (double)waveformArea.getX(), (double)waveformArea.getRight());
                auto loopEndX = juce::jmap(loopEndTime, 0.0, duration,
                    (double)waveformArea.getX(), (double)waveformArea.getRight());

                g.setColour(juce::Colours::yellow.withAlpha(0.3f));
                g.fillRect((float)loopStartX, (float)waveformArea.getY(),
                    (float)(loopEndX - loopStartX), (float)waveformArea.getHeight());
            }
        }

        double duration = playerAudio.getTrackDuration();
        if (duration > 0.0)
        {
            double currentPos = playerAudio.getPlaybackTime();
            double xPos = juce::jmap(currentPos, 0.0, duration,
                (double)waveformArea.getX(),
                (double)waveformArea.getRight());

            g.setColour(juce::Colours::red);
            g.drawLine((float)xPos, (float)waveformArea.getY(),
                (float)xPos, (float)waveformArea.getBottom(), 2.0f);
        }
        if (loopStartSet)
        {
            double duration = playerAudio.getTrackDuration();
            if (duration > 0.0)
            {
                auto markerX = juce::jmap(loopStartTime, 0.0, duration,
                    (double)waveformArea.getX(), (double)waveformArea.getRight());

                g.setColour(juce::Colours::green);
                g.drawLine((float)markerX, (float)waveformArea.getY(),
                    (float)markerX, (float)waveformArea.getBottom(), 3.0f);
            }
        }

        if (loopEndSet)
        {
            double duration = playerAudio.getTrackDuration();
            if (duration > 0.0)
            {
                auto markerX = juce::jmap(loopEndTime, 0.0, duration,
                    (double)waveformArea.getX(), (double)waveformArea.getRight());

                g.setColour(juce::Colours::orange);
                g.drawLine((float)markerX, (float)waveformArea.getY(),
                    (float)markerX, (float)waveformArea.getBottom(), 3.0f);
            }
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

    loadButton.setBounds(x, y, w, 40);
    playPauseButton.setBounds(x += w + gap, y, w, 40);
    stopButton.setBounds(x += w + gap, y, w, 40);
    restartButton.setBounds(x += w + gap, y, w, 40);
    muteButton.setBounds(x += w + gap, y, w, 40);
    loopButton.setBounds(x += w + gap, y, w, 40);
    y += 50;
    x = 20;
    loopStartButton.setBounds(x, y, w, 40);
    loopEndButton.setBounds(x += w + gap, y, w, 40);
    clearLoopButton.setBounds(x += w + gap, y, w + 20, 40);

    volumeSlider.setBounds(20, 120, getWidth() - 40, 30);

    speedLabel.setBounds(20, 160, 60, 30);
    speedSlider.setBounds(90, 160, getWidth() - 110, 30);

    auto waveformArea = getLocalBounds().reduced(20, 250); 

    int sliderHeight = 25;
    int timeLabelHeight = 20;

    positionSlider.setBounds(
        waveformArea.getX(),
        waveformArea.getBottom() + 10,
        waveformArea.getWidth(),
        sliderHeight
    );

    currentTimeLabel.setBounds(
        waveformArea.getX(),
        waveformArea.getBottom() + 10 + sliderHeight + 5,
        60,
        timeLabelHeight
    );

    totalTimeLabel.setBounds(
        waveformArea.getRight() - 60,
        waveformArea.getBottom() + 10 + sliderHeight + 5,
        60,
        timeLabelHeight
    );
}
void PlayerGui::setLoopStart()
{
    if (!fileLoaded) return;

    loopStartTime = playerAudio.getPlaybackTime();
    loopStartSet = true;

    if (loopEndSet && loopEndTime <= loopStartTime)
    {
        std::swap(loopStartTime, loopEndTime);
    }

    if (loopStartSet && loopEndSet)
    {
        abLoopEnabled = true;
        playerAudio.setLooping(true);
   
    }

    repaint();
}

void PlayerGui::setLoopEnd()
{
    if (!fileLoaded) return;

    loopEndTime = playerAudio.getPlaybackTime();
    loopEndSet = true;
    if (loopStartSet && loopEndTime <= loopStartTime)
    {
        std::swap(loopStartTime, loopEndTime);
    }
    if (loopStartSet && loopEndSet)
    {
        abLoopEnabled = true;
        playerAudio.setLooping(true);
    }

    repaint();
}

void PlayerGui::clearLoopMarkers()
{
    loopStartSet = false;
    loopEndSet = false;
    abLoopEnabled = false;
    playerAudio.setLooping(false);
    loopButton.setButtonText("Loop: OFF");

    repaint();
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

juce::String PlayerGui::formatTime(double seconds)
{
    if (seconds < 0) seconds = 0;

    int minutes = (int)seconds / 60;
    int secs = (int)seconds % 60;
    return juce::String::formatted("%d:%02d", minutes, secs);
}

void PlayerGui::timerCallback()
{
    if (fileLoaded)
    {
        double currentTime = playerAudio.getPlaybackTime();
        double duration = playerAudio.getTrackDuration();
        if (abLoopEnabled && loopStartSet && loopEndSet && currentTime >= loopEndTime)
        {
            playerAudio.setPosition(loopStartTime);
            currentTime = loopStartTime; 
        }
     
        if (!positionSlider.isMouseButtonDown() && duration > 0.0)
        {
            positionSlider.setRange(0.0, duration, 0.01);
            positionSlider.setValue(currentTime, juce::dontSendNotification);
        }

        currentTimeLabel.setText(formatTime(currentTime), juce::dontSendNotification);
        totalTimeLabel.setText(formatTime(duration), juce::dontSendNotification);

        if (playerAudio.isPlaying())
            repaint();
    }
}

void PlayerGui::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio file...", juce::File{}, "*.wav;*.mp3;*.aiff;*.flac");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
                {
                    thumbnail.clear();
                    playerAudio.loadFile(file);
                    thumbnail.setSource(new juce::FileInputSource(file));
                    fileLoaded = true;
                    clearLoopMarkers();          
                    playPauseButton.setButtonText("Play");

                    double duration = playerAudio.getTrackDuration();
                    if (duration > 0.0)
                    {
                        positionSlider.setRange(0.0, duration, 0.01);
                        positionSlider.setValue(0.0);
                    }

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
            volumeSlider.setValue(playerAudio.isMuted() ? 0.0f : 0.5f, juce::dontSendNotification);
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

        if (shouldLoop && loopStartSet && loopEndSet)
        {
            abLoopEnabled = true;
        }
        else
        {
            abLoopEnabled = false;
        }

        repaint();
    }
    else if (button == &loopStartButton)
    {
        setLoopStart();
    }
    else if (button == &loopEndButton)
    {
        setLoopEnd();
    }
    else if (button == &clearLoopButton)
    {
        clearLoopMarkers();
    }
}

void PlayerGui::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setSpeed(slider->getValue());
    }
    else if (slider == &positionSlider)
    {
        playerAudio.setPosition(slider->getValue());
    }
}
