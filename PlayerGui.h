#pragma once

#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::AudioAppComponent,
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

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
  
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void setLoopStart();
    void setLoopEnd();
    void clearLoopMarkers();

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

    juce::String formatTime(double seconds)
    {
        int mins = (int)(seconds / 60.0);
        int secs = (int)std::fmod(seconds, 60.0);
        return juce::String::formatted("%d:%02d", mins, secs);
    }

    Playeraudio playerAudio;
    bool fileLoaded = false;
    float previousVolume = 0.5f;

    double loopStartTime = 0.0;
    double loopEndTime = 0.0;
    bool loopStartSet = false;
    bool loopEndSet = false;
    bool abLoopEnabled = false;

    juce::TextButton loadButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton restartButton;
    juce::TextButton muteButton;
    juce::TextButton loopButton;
    juce::TextButton loopStartButton{ "A" };
    juce::TextButton loopEndButton{ "B" };
    juce::TextButton clearLoopButton{ "Clear Loop" };
    juce::TextButton forwardButton{ ">>" };
    juce::TextButton backwardButton{ "<<" };
    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::TextButton deleteMarkerButton{ "Delete Marker" };
    juce::TextButton gotoMarkerButton{ "Go To Marker" };

    juce::ListBox markerList;
    std::vector<Marker> markers;

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    juce::Label speedLabel;

    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::SpinLock thumbnailLock;

    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
