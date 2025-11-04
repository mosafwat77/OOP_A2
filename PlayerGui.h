#pragma once
#include <JuceHeader.h>
#include "Playeraudio.h"

class PlayerGui : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel,
    public juce::ChangeListener
{
public:
    PlayerGui();
    ~PlayerGui() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;
    void deleteKeyPressed(int lastRowSelected) override;

private:
    Playeraudio playerAudio;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::SpinLock thumbnailLock;

    juce::TextButton loadButton;
    juce::TextButton playPauseButton;
    juce::TextButton stopButton;
    juce::TextButton restartButton;
    juce::TextButton muteButton;
    juce::TextButton loopButton;
    juce::TextButton loopStartButton;
    juce::TextButton loopEndButton;
    juce::TextButton clearLoopButton;
    juce::TextButton forwardButton{ ">|" };
    juce::TextButton backwardButton{" |<"};

    // Marker controls
    juce::TextButton addMarkerButton;
    juce::TextButton deleteMarkerButton;
    juce::ListBox markerListBox;
    juce::Label markerLabel;

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;
    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    juce::Label speedLabel;
    juce::Label volumelabel;

    std::unique_ptr<juce::FileChooser> fileChooser;
    bool fileLoaded = false;

    bool loopStartSet = false;
    bool loopEndSet = false;
    double loopStartTime = 0.0;
    double loopEndTime = 0.0;
    bool abLoopEnabled = false;

    int selectedMarkerRow = -1;

    void setLoopStart();
    void setLoopEnd();
    void clearLoopMarkers();
    juce::String formatTime(double seconds);

    void addMarker();
    void deleteSelectedMarker();
    void jumpToMarker(int markerIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
