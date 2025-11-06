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
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(backwardButton);
    addAndMakeVisible(goToEndButton);
    addAndMakeVisible(smartDuckButton);

    addAndMakeVisible(addToPlaylistButton);
    addAndMakeVisible(removeFromPlaylistButton);
    addAndMakeVisible(clearPlaylistButton);

    addAndMakeVisible(addMarkerButton);
    addAndMakeVisible(deleteMarkerButton);
    addAndMakeVisible(gotoMarkerButton);

    loadButton.setButtonText("Load");
    playPauseButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    restartButton.setButtonText("Restart");
    muteButton.setButtonText("Mute");
    loopButton.setButtonText("Loop: OFF");
    loopStartButton.setButtonText("Set A");
    loopEndButton.setButtonText("Set B");
    clearLoopButton.setButtonText("Clear Loop");
    forwardButton.setButtonText("+10s");
    backwardButton.setButtonText("-10s");
    goToEndButton.setButtonText("Go To End");
    smartDuckButton.setButtonText("Smart Duck: OFF");

    addToPlaylistButton.setButtonText("Add Files");
    removeFromPlaylistButton.setButtonText("Remove");
    clearPlaylistButton.setButtonText("Clear All");
    addMarkerButton.setButtonText("Add Marker");
    deleteMarkerButton.setButtonText("Delete Marker");
    gotoMarkerButton.setButtonText("Go To");

    // Style all buttons with light gray theme
    for (auto* btn : { &loadButton, &playPauseButton, &stopButton, &restartButton,
                      &muteButton, &loopButton, &loopStartButton, &loopEndButton,
                      &clearLoopButton, &forwardButton, &backwardButton,
                      &goToEndButton, &smartDuckButton,
                      &addToPlaylistButton, &removeFromPlaylistButton, &clearPlaylistButton,
                      &addMarkerButton, &deleteMarkerButton, &gotoMarkerButton })
    {
        btn->addListener(this);

        // Light gray button styling
        btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffcccccc));
        btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffaaaaaa));
        btn->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff333333));
        btn->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff000000));
        btn->setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff999999));
    }

    // Special styling for primary buttons
    playPauseButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd4d4d4));
    playPauseButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffb8b8b8));

    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd4d4d4));
    loadButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffb8b8b8));

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffaaaaaa));
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffcccccc));
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffaaaaaa));
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffcccccc));
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setSliderStyle(juce::Slider::LinearBar);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    positionSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffcccccc));
    positionSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xff555555));
    addAndMakeVisible(positionSlider);

    currentTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::right);
    totalTimeLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);

    speedLabel.setText("Speed:", juce::dontSendNotification);
    addAndMakeVisible(speedLabel);

    playlistLabel.setText("Playlist:", juce::dontSendNotification);
    playlistLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(playlistLabel);

    markersLabel.setText("Markers:", juce::dontSendNotification);
    markersLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(markersLabel);

    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    metadataLabel.setJustificationType(juce::Justification::centredLeft);
    metadataLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(metadataLabel);

    playlistBox.setModel(new PlaylistBoxModel(playlistItems, currentPlaylistIndex,
        [this](int row) { playlistItemClicked(row); }));
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::white);
    playlistBox.setRowHeight(20);
    addAndMakeVisible(playlistBox);

    markerList.setModel(this);
    markerList.setRowHeight(25);
    addAndMakeVisible(markerList);

    spectrumData.fill(0.0f);

    startTimerHz(30);
}

PlayerGui::~PlayerGui()
{
    delete playlistBox.getModel();
}

void PlayerGui::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));

    auto area = getLocalBounds().reduced(10);
    area.removeFromTop(50 + 5);
    area.removeFromTop(50 + 5);
    area.removeFromTop(30 + 5);
    area.removeFromTop(30 + 10);
    auto waveformArea = area.removeFromTop(130);

    if (fileLoaded && thumbnail.getNumChannels() > 0)
    {
        const juce::SpinLock::ScopedLockType lock(thumbnailLock);

        g.setColour(juce::Colour(0xff2a2a2a));
        g.fillRect(waveformArea);

        g.setColour(juce::Colours::lightblue);
        thumbnail.drawChannel(g, waveformArea, 0.0, thumbnail.getTotalLength(), 0, 0.6f);

        for (size_t i = 0; i < spectrumData.size(); ++i)
        {
            float barWidth = waveformArea.getWidth() / (float)spectrumData.size();
            float barHeight = spectrumData[i] * waveformArea.getHeight() * 0.3f;
            float x = waveformArea.getX() + i * barWidth;
            float y = waveformArea.getBottom() - barHeight;

            g.setColour(juce::Colours::cyan.withAlpha(0.4f));
            g.fillRect(x, y, barWidth - 1.0f, barHeight);
        }

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

        for (const auto& marker : markers)
        {
            double duration = playerAudio.getTrackDuration();
            if (duration > 0.0)
            {
                auto markerX = juce::jmap(marker.time, 0.0, duration,
                    (double)waveformArea.getX(), (double)waveformArea.getRight());

                g.setColour(juce::Colours::cyan);
                g.fillEllipse((float)markerX - 3.0f, (float)waveformArea.getY() + 5.0f, 6.0f, 6.0f);

                g.drawLine((float)markerX, (float)waveformArea.getY() + 11.0f,
                    (float)markerX, (float)waveformArea.getBottom(), 1.0f);
            }
        }
    }
    else
    {
        g.setColour(juce::Colour(0xff2a2a2a));
        g.fillRect(waveformArea);

        g.setColour(juce::Colours::grey);
        g.drawFittedText("Load an audio file to see waveform",
            waveformArea, juce::Justification::centred, 1);
    }
}

void PlayerGui::resized()
{
    auto area = getLocalBounds().reduced(10);

    // First row - Main transport controls (centered and larger)
    auto buttonRow1 = area.removeFromTop(50);
    int w = 90;
    int gap = 8;

    int totalWidth = (w * 7) + (gap * 6);
    int leftMargin = (buttonRow1.getWidth() - totalWidth) / 2;
    buttonRow1.removeFromLeft(leftMargin);

    loadButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    playPauseButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    stopButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    restartButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    goToEndButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    muteButton.setBounds(buttonRow1.removeFromLeft(w));
    buttonRow1.removeFromLeft(gap);
    loopButton.setBounds(buttonRow1.removeFromLeft(w));

    area.removeFromTop(5);

    // Second row - Navigation and loop controls (centered)
    auto buttonRow2 = area.removeFromTop(50);
    w = 85;
    gap = 8;

    totalWidth = (w * 6) + (gap * 5) + 120;
    leftMargin = (buttonRow2.getWidth() - totalWidth) / 2;
    buttonRow2.removeFromLeft(leftMargin);

    backwardButton.setBounds(buttonRow2.removeFromLeft(w));
    buttonRow2.removeFromLeft(gap);
    forwardButton.setBounds(buttonRow2.removeFromLeft(w));
    buttonRow2.removeFromLeft(gap);
    loopStartButton.setBounds(buttonRow2.removeFromLeft(w));
    buttonRow2.removeFromLeft(gap);
    loopEndButton.setBounds(buttonRow2.removeFromLeft(w));
    buttonRow2.removeFromLeft(gap);
    clearLoopButton.setBounds(buttonRow2.removeFromLeft(w + 20));
    buttonRow2.removeFromLeft(gap);
    smartDuckButton.setBounds(buttonRow2.removeFromLeft(120));

    area.removeFromTop(5);

    auto volumeArea = area.removeFromTop(30);
    volumeSlider.setBounds(volumeArea);

    area.removeFromTop(5);

    auto speedArea = area.removeFromTop(30);
    speedLabel.setBounds(speedArea.removeFromLeft(60));
    speedSlider.setBounds(speedArea);

    area.removeFromTop(10);

    auto waveformArea = area.removeFromTop(130);

    area.removeFromTop(5);

    auto posSliderArea = area.removeFromTop(25);
    positionSlider.setBounds(posSliderArea);

    area.removeFromTop(5);

    auto timeArea = area.removeFromTop(20);
    currentTimeLabel.setBounds(timeArea.removeFromLeft(60));
    totalTimeLabel.setBounds(timeArea.removeFromRight(60));

    area.removeFromTop(5);

    auto metadataArea = area.removeFromTop(55);
    metadataLabel.setBounds(metadataArea);

    area.removeFromTop(5);

    auto playlistHeaderArea = area.removeFromTop(30);
    playlistLabel.setBounds(playlistHeaderArea.removeFromLeft(80));
    addToPlaylistButton.setBounds(playlistHeaderArea.removeFromLeft(85));
    playlistHeaderArea.removeFromLeft(gap);
    removeFromPlaylistButton.setBounds(playlistHeaderArea.removeFromLeft(85));
    playlistHeaderArea.removeFromLeft(gap);
    clearPlaylistButton.setBounds(playlistHeaderArea.removeFromLeft(85));

    area.removeFromTop(5);

    auto playlistArea = area.removeFromTop(100);
    playlistBox.setBounds(playlistArea);

    area.removeFromTop(5);

    auto markersHeaderArea = area.removeFromTop(30);
    markersLabel.setBounds(markersHeaderArea.removeFromLeft(80));
    addMarkerButton.setBounds(markersHeaderArea.removeFromLeft(100));
    markersHeaderArea.removeFromLeft(gap);
    deleteMarkerButton.setBounds(markersHeaderArea.removeFromLeft(110));
    markersHeaderArea.removeFromLeft(gap);
    gotoMarkerButton.setBounds(markersHeaderArea.removeFromLeft(85));

    area.removeFromTop(5);
    markerList.setBounds(area);
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
        loopButton.setButtonText("Loop: ON");
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
        loopButton.setButtonText("Loop: ON");
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

void PlayerGui::triggerVolumeDuck()
{
    if (smartDuckEnabled && !volumeDuckActive)
    {
        volumeDuckActive = true;
        volumeDuckStartTime = juce::Time::getMillisecondCounterHiRes() / 1000.0;
        volumeBeforeDuck = playerAudio.getCurrentGain();
        playerAudio.setGain(volumeBeforeDuck * 0.3f);
    }
}

void PlayerGui::updateSpectrum()
{
    for (size_t i = 0; i < spectrumData.size(); ++i)
    {
        float target = juce::Random::getSystemRandom().nextFloat() *
            (playerAudio.isPlaying() ? 1.0f : 0.0f);
        spectrumData[i] = spectrumData[i] * 0.8f + target * 0.2f;
    }
}

void PlayerGui::timerCallback()
{
    if (fileLoaded)
    {
        double currentTime = playerAudio.getPlaybackTime();
        double duration = playerAudio.getTrackDuration();

        if (abLoopEnabled && loopStartSet && loopEndSet)
        {
            if (currentTime >= loopEndTime || currentTime < loopStartTime)
            {
                playerAudio.setPosition(loopStartTime);
                currentTime = loopStartTime;
            }
        }

        if (!positionSlider.isMouseButtonDown() && duration > 0.0)
        {
            positionSlider.setRange(0.0, duration, 0.01);
            positionSlider.setValue(currentTime, juce::dontSendNotification);
        }

        currentTimeLabel.setText(formatTime(currentTime), juce::dontSendNotification);
        totalTimeLabel.setText(formatTime(duration), juce::dontSendNotification);

        if (volumeDuckActive)
        {
            double currentTimeMs = juce::Time::getMillisecondCounterHiRes() / 1000.0;
            double elapsed = currentTimeMs - volumeDuckStartTime;

            if (elapsed >= duckDuration)
            {
                playerAudio.setGain(volumeBeforeDuck);
                volumeDuckActive = false;
            }
        }

        updateSpectrum();

        if (playerAudio.isPlaying())
            repaint();
    }
}

void PlayerGui::addFilesToPlaylist(const juce::Array<juce::File>& files)
{
    for (const auto& file : files)
    {
        if (file.existsAsFile())
        {
            playlistFiles.add(file);
            playlistItems.add(file.getFileName());
        }
    }
    playlistBox.updateContent();
    playlistBox.repaint();
}

void PlayerGui::loadSelectedTrack()
{
    if (currentPlaylistIndex >= 0 && currentPlaylistIndex < playlistFiles.size())
    {
        auto file = playlistFiles[currentPlaylistIndex];

        thumbnail.clear();
        playerAudio.loadFile(file);
        thumbnail.setSource(new juce::FileInputSource(file));
        fileLoaded = true;
        clearLoopMarkers();
        markers.clear();
        markerList.updateContent();
        playPauseButton.setButtonText("Play");

        double duration = playerAudio.getTrackDuration();
        if (duration > 0.0)
        {
            positionSlider.setRange(0.0, duration, 0.01);
            positionSlider.setValue(0.0);
        }

        updateMetadata(file);

        repaint();
        playlistBox.repaint();
    }
}

void PlayerGui::playlistItemClicked(int row)
{
    if (row >= 0 && row < playlistFiles.size())
    {
        currentPlaylistIndex = row;
        loadSelectedTrack();
    }
}

void PlayerGui::removeSelectedFromPlaylist()
{
    int selectedRow = playlistBox.getSelectedRow();
    if (selectedRow >= 0 && selectedRow < playlistFiles.size())
    {
        playlistFiles.remove(selectedRow);
        playlistItems.remove(selectedRow);

        if (currentPlaylistIndex == selectedRow)
        {
            currentPlaylistIndex = -1;
        }
        else if (currentPlaylistIndex > selectedRow)
        {
            currentPlaylistIndex--;
        }

        playlistBox.updateContent();
        playlistBox.repaint();
    }
}

void PlayerGui::clearPlaylist()
{
    playlistFiles.clear();
    playlistItems.clear();
    currentPlaylistIndex = -1;
    playlistBox.updateContent();
    playlistBox.repaint();
}

juce::File PlayerGui::getSessionFile()
{
    auto appDataDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    auto sessionDir = appDataDir.getChildFile("SimpleAudioPlayer");

    if (!sessionDir.exists())
        sessionDir.createDirectory();

    return sessionDir.getChildFile("session.xml");
}

void PlayerGui::saveSession()
{
    auto sessionFile = getSessionFile();

    juce::XmlElement xml("AudioPlayerSession");

    if (currentPlaylistIndex >= 0 && currentPlaylistIndex < playlistFiles.size())
    {
        auto* trackElement = xml.createNewChildElement("CurrentTrack");
        trackElement->setAttribute("index", currentPlaylistIndex);
        trackElement->setAttribute("path", playlistFiles[currentPlaylistIndex].getFullPathName());
        trackElement->setAttribute("position", playerAudio.getPlaybackTime());
    }

    auto* settingsElement = xml.createNewChildElement("Settings");
    settingsElement->setAttribute("volume", volumeSlider.getValue());
    settingsElement->setAttribute("speed", speedSlider.getValue());
    settingsElement->setAttribute("muted", playerAudio.isMuted());
    settingsElement->setAttribute("smartDuckEnabled", smartDuckEnabled);

    savePlaylistToXml(xml);

    auto* markersElement = xml.createNewChildElement("Markers");
    for (const auto& marker : markers)
    {
        auto* markerElement = markersElement->createNewChildElement("Marker");
        markerElement->setAttribute("time", marker.time);
        markerElement->setAttribute("label", marker.label);
    }

    xml.writeTo(sessionFile);
}

void PlayerGui::loadSession()
{
    auto sessionFile = getSessionFile();

    if (!sessionFile.existsAsFile())
        return;

    auto xml = juce::XmlDocument::parse(sessionFile);
    if (xml == nullptr)
        return;

    if (auto* settingsElement = xml->getChildByName("Settings"))
    {
        volumeSlider.setValue(settingsElement->getDoubleAttribute("volume", 0.5));
        speedSlider.setValue(settingsElement->getDoubleAttribute("speed", 1.0));

        if (settingsElement->getBoolAttribute("muted", false))
        {
            playerAudio.mute();
            muteButton.setButtonText("Unmute");
        }

        smartDuckEnabled = settingsElement->getBoolAttribute("smartDuckEnabled", false);
        smartDuckButton.setButtonText(smartDuckEnabled ? "Smart Duck: ON" : "Smart Duck: OFF");
    }

    loadPlaylistFromXml(*xml);

    if (auto* markersElement = xml->getChildByName("Markers"))
    {
        markers.clear();
        for (auto* markerElement : markersElement->getChildIterator())
        {
            if (markerElement->hasTagName("Marker"))
            {
                double time = markerElement->getDoubleAttribute("time", 0.0);
                juce::String label = markerElement->getStringAttribute("label");
                markers.push_back({ time, label });
            }
        }
        markerList.updateContent();
    }

    if (auto* trackElement = xml->getChildByName("CurrentTrack"))
    {
        int index = trackElement->getIntAttribute("index", -1);
        juce::String path = trackElement->getStringAttribute("path");
        double position = trackElement->getDoubleAttribute("position", 0.0);

        juce::File file(path);
        if (file.existsAsFile())
        {
            int foundIndex = playlistFiles.indexOf(file);
            if (foundIndex >= 0)
            {
                currentPlaylistIndex = foundIndex;
            }
            else if (index >= 0 && index < playlistFiles.size())
            {
                currentPlaylistIndex = index;
            }

            if (currentPlaylistIndex >= 0)
            {
                loadSelectedTrack();
                playerAudio.setPosition(position);
            }
        }
    }
}

void PlayerGui::savePlaylistToXml(juce::XmlElement& xml)
{
    auto* playlistElement = xml.createNewChildElement("Playlist");

    for (int i = 0; i < playlistFiles.size(); ++i)
    {
        auto* fileElement = playlistElement->createNewChildElement("File");
        fileElement->setAttribute("path", playlistFiles[i].getFullPathName());
    }
}

void PlayerGui::loadPlaylistFromXml(const juce::XmlElement& xml)
{
    if (auto* playlistElement = xml.getChildByName("Playlist"))
    {
        playlistFiles.clear();
        playlistItems.clear();

        for (auto* fileElement : playlistElement->getChildIterator())
        {
            if (fileElement->hasTagName("File"))
            {
                juce::String path = fileElement->getStringAttribute("path");
                juce::File file(path);

                if (file.existsAsFile())
                {
                    playlistFiles.add(file);
                    playlistItems.add(file.getFileName());
                }
            }
        }

        playlistBox.updateContent();
        playlistBox.repaint();
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
                    markers.clear();
                    markerList.updateContent();
                    playPauseButton.setButtonText("Play");

                    double duration = playerAudio.getTrackDuration();
                    if (duration > 0.0)
                    {
                        positionSlider.setRange(0.0, duration, 0.01);
                        positionSlider.setValue(0.0);
                    }

                    updateMetadata(file);

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
    else if (button == &goToEndButton)
    {
        double duration = playerAudio.getTrackDuration();
        if (duration > 0.0)
        {
            playerAudio.setPosition(duration - 0.1);
        }
    }
    else if (button == &muteButton)
    {
        if (playerAudio.isMuted())
        {
            playerAudio.unmute();
            muteButton.setButtonText("Mute");
        }
        else
        {
            playerAudio.mute();
            muteButton.setButtonText("Unmute");
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
        else if (!shouldLoop)
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
    else if (button == &forwardButton)
    {
        playerAudio.increment(10.0);
    }
    else if (button == &backwardButton)
    {
        playerAudio.decrement(10.0);
    }
    else if (button == &smartDuckButton)
    {
        smartDuckEnabled = !smartDuckEnabled;
        smartDuckButton.setButtonText(smartDuckEnabled ? "Smart Duck: ON" : "Smart Duck: OFF");
    }
    else if (button == &addToPlaylistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files...", juce::File{}, "*.wav;*.mp3;*.aiff;*.flac");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& chooser)
            {
                addFilesToPlaylist(chooser.getResults());
            });
    }
    else if (button == &removeFromPlaylistButton)
    {
        removeSelectedFromPlaylist();
    }
    else if (button == &clearPlaylistButton)
    {
        clearPlaylist();
    }
    else if (button == &addMarkerButton)
    {
        addMarkerAtCurrentPosition();
        triggerVolumeDuck();
    }
    else if (button == &deleteMarkerButton)
    {
        deleteSelectedMarker();
    }
    else if (button == &gotoMarkerButton)
    {
        gotoSelectedMarker();
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

void PlayerGui::updateMetadata(const juce::File& file)
{
    juce::String metadata = getMetadataString(file);
    metadataLabel.setText(metadata, juce::dontSendNotification);
}

juce::String PlayerGui::getMetadataString(const juce::File& file)
{
    juce::String info;

    auto* reader = playerAudio.getFormatManager().createReaderFor(file);

    if (reader != nullptr)
    {
        juce::String filename = file.getFileNameWithoutExtension();
        juce::String format = file.getFileExtension().toUpperCase().substring(1);

        int sampleRate = (int)reader->sampleRate;
        int bitDepth = reader->bitsPerSample;
        int numChannels = reader->numChannels;
        double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;

        int minutes = (int)lengthInSeconds / 60;
        int seconds = (int)lengthInSeconds % 60;
        juce::String duration = juce::String::formatted("%d:%02d", minutes, seconds);

        juce::StringPairArray metadata = reader->metadataValues;
        juce::String title = metadata.getValue("title", "");
        juce::String artist = metadata.getValue("artist", "");
        juce::String album = metadata.getValue("album", "");

        if (title.isNotEmpty())
            info << "Title: " << title << "\n";
        else
            info << "File: " << filename << "\n";

        if (artist.isNotEmpty())
            info << "Artist: " << artist << "\n";

        if (album.isNotEmpty())
            info << "Album: " << album << "\n";

        info << "Format: " << format << " | ";
        info << sampleRate / 1000.0 << " kHz | ";
        info << bitDepth << " bit | ";
        info << (numChannels == 1 ? "Mono" : (numChannels == 2 ? "Stereo" : juce::String(numChannels) + " ch")) << " | ";
        info << "Duration: " << duration;

        delete reader;
    }
    else
    {
        info = "Could not read file metadata";
    }

    return info;
}

void PlayerGui::refreshMarkerList()
{
    markerList.updateContent();
}

void PlayerGui::sortMarkers()
{
    std::sort(markers.begin(), markers.end(),
        [](const Marker& a, const Marker& b) { return a.time < b.time; });
}

void PlayerGui::addMarkerAtCurrentPosition()
{
    if (!fileLoaded) return;

    double currentTime = playerAudio.getPlaybackTime();
    juce::String markerName = "Marker " + juce::String(markers.size() + 1) +
        " (" + formatTime(currentTime) + ")";

    markers.push_back({ currentTime, markerName });
    sortMarkers();
    refreshMarkerList();
    repaint();
}

void PlayerGui::deleteSelectedMarker()
{
    int selectedRow = markerList.getSelectedRow();
    if (selectedRow >= 0 && selectedRow < markers.size())
    {
        markers.erase(markers.begin() + selectedRow);
        refreshMarkerList();
        repaint();
    }
}

void PlayerGui::gotoSelectedMarker()
{
    int selectedRow = markerList.getSelectedRow();
    if (selectedRow >= 0 && selectedRow < markers.size())
    {
        playerAudio.setPosition(markers[selectedRow].time);
        markerList.repaint();
    }
}

int PlayerGui::getNumRows()
{
    return markers.size();
}

void PlayerGui::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else
        g.fillAll(juce::Colours::white);

    g.setColour(juce::Colours::black);

    if (rowNumber < markers.size())
    {
        juce::String markerText = markers[rowNumber].label;
        g.drawText(markerText, 5, 0, width - 5, height, juce::Justification::centredLeft);
    }
}

void PlayerGui::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    if (row >= 0 && row < markers.size())
    {
        playerAudio.setPosition(markers[row].time);
        markerList.repaint();
    }
}

void PlayerGui::deleteKeyPressed(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < markers.size())
    {
        markers.erase(markers.begin() + lastRowSelected);
        refreshMarkerList();
        repaint();
    }
}