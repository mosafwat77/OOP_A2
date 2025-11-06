#pragma once

#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{
public:
    PlayerGui();
    ~PlayerGui() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void saveSession();
    void loadSession();

private:
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    juce::String formatTime(double seconds);

    void setLoopStart();
    void setLoopEnd();
    void clearLoopMarkers();

    void addFilesToPlaylist(const juce::Array<juce::File>& files);
    void loadSelectedTrack();
    void playlistItemClicked(int row);
    void removeSelectedFromPlaylist();
    void clearPlaylist();

    juce::File getSessionFile();
    void savePlaylistToXml(juce::XmlElement& xml);
    void loadPlaylistFromXml(const juce::XmlElement& xml);

    void updateMetadata(const juce::File& file);
    juce::String getMetadataString(const juce::File& file);

    struct Marker
    {
        double time;
        juce::String label;
    };

    void refreshMarkerList();
    void sortMarkers();
    void addMarkerAtCurrentPosition();
    void deleteSelectedMarker();
    void gotoSelectedMarker();

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;
    void deleteKeyPressed(int lastRowSelected) override;

    void triggerVolumeDuck();
    void updateSpectrum();

    Playeraudio playerAudio;

    juce::TextButton loadButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton restartButton;
    juce::TextButton muteButton;
    juce::TextButton loopButton;
    juce::TextButton loopStartButton;
    juce::TextButton loopEndButton;
    juce::TextButton clearLoopButton;
    juce::TextButton forwardButton;
    juce::TextButton backwardButton;
    juce::TextButton goToEndButton;
    juce::TextButton smartDuckButton;

    juce::TextButton addToPlaylistButton;
    juce::TextButton removeFromPlaylistButton;
    juce::TextButton clearPlaylistButton;

    juce::TextButton addMarkerButton;
    juce::TextButton deleteMarkerButton;
    juce::TextButton gotoMarkerButton;

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    juce::Label speedLabel;
    juce::Label playlistLabel;
    juce::Label metadataLabel;
    juce::Label markersLabel;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::SpinLock thumbnailLock;

    juce::ListBox playlistBox;
    juce::StringArray playlistItems;
    juce::Array<juce::File> playlistFiles;
    int currentPlaylistIndex = -1;

    juce::ListBox markerList;
    std::vector<Marker> markers;

    std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileLoaded = false;

    double loopStartTime = 0.0;
    double loopEndTime = 0.0;
    bool loopStartSet = false;
    bool loopEndSet = false;
    bool abLoopEnabled = false;

    bool volumeDuckActive = false;
    double volumeDuckStartTime = 0.0;
    float volumeBeforeDuck = 1.0f;
    static constexpr double duckDuration = 0.3;

    bool smartDuckEnabled = false;

    std::array<float, 32> spectrumData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};

class PlaylistBoxModel : public juce::ListBoxModel
{
public:
    PlaylistBoxModel(juce::StringArray& items, int& currentIndex,
        std::function<void(int)> callback)
        : playlistItems(items), currentPlaylistIndex(currentIndex),
        itemClickCallback(callback)
    {
    }

    int getNumRows() override
    {
        return playlistItems.size();
    }

    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
            g.fillAll(juce::Colours::lightblue);
        else if (rowNumber == currentPlaylistIndex)
            g.fillAll(juce::Colours::lightyellow);
        else
            g.fillAll(juce::Colours::white);

        g.setColour(juce::Colours::black);
        g.drawText(playlistItems[rowNumber], 5, 0, width - 10, height,
            juce::Justification::centredLeft, true);
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        if (itemClickCallback)
            itemClickCallback(row);
    }

private:
    juce::StringArray& playlistItems;
    int& currentPlaylistIndex;
    std::function<void(int)> itemClickCallback;
};