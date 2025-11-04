#include "Playeraudio.h"

Playeraudio::Playeraudio()
{
    formatManager.registerBasicFormats();
}

Playeraudio::~Playeraudio()
{
    transportSource.setSource(nullptr);
}

void Playeraudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Playeraudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    // Handle looping if enabled
    if (isLooping && loopStartMarkerIndex >= 0 && loopEndMarkerIndex >= 0
        && loopStartMarkerIndex < markers.size() && loopEndMarkerIndex < markers.size())
    {
        auto currentPos = transportSource.getCurrentPosition();
        auto loopStart = markers[loopStartMarkerIndex].timestamp;
        auto loopEnd = markers[loopEndMarkerIndex].timestamp;

        if (currentPos >= loopEnd)
        {
            transportSource.setPosition(loopStart);
        }
    }
}

void Playeraudio::releaseResources()
{
    resampleSource.releaseResources();
    transportSource.releaseResources();
}

void Playeraudio::loadFile(const juce::File& audioFile)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    if (audioFile.existsAsFile())
    {
        auto* reader = formatManager.createReaderFor(audioFile);
        if (reader != nullptr)
        {
            readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            // Clear markers when loading new file
            markers.clear();
            loopStartMarkerIndex = -1;
            loopEndMarkerIndex = -1;
            sendChangeMessage();
        }
    }
}

void Playeraudio::start()
{
    transportSource.start();
}

void Playeraudio::stop()
{
    transportSource.stop();
}

void Playeraudio::pause()
{
    transportSource.stop();
}

void Playeraudio::restart()
{
    setPosition(0.0);
    start();
}

void Playeraudio::setGain(float gain)
{
    lastGain = gain;
    if (!muted)
    {
        transportSource.setGain(gain);
    }
}

void Playeraudio::mute()
{
    if (!muted)
    {
        lastVolumeBeforeMute = transportSource.getGain();
        transportSource.setGain(0.0f);
        muted = true;
    }
}

void Playeraudio::unmute()
{
    if (muted)
    {
        transportSource.setGain(lastVolumeBeforeMute);
        muted = false;
    }
}

bool Playeraudio::isMuted() const
{
    return muted;
}

void Playeraudio::increment(double seconds)
{
    setPosition(transportSource.getCurrentPosition() + seconds);
}

void Playeraudio::decrement(double seconds)
{
    setPosition(transportSource.getCurrentPosition() - seconds);
}

void Playeraudio::setPosition(double positionInSeconds)
{
    auto length = transportSource.getLengthInSeconds();
    if (length > 0)
    {
        transportSource.setPosition(juce::jlimit(0.0, length, positionInSeconds));
    }
}

double Playeraudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double Playeraudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void Playeraudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
}

bool Playeraudio::isPlaying() const
{
    return transportSource.isPlaying();
}

void Playeraudio::enableLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
}

bool Playeraudio::isTrackPlaying() const
{
    return transportSource.isPlaying();
}

double Playeraudio::getPlaybackTime() const
{
    return transportSource.getCurrentPosition();
}

double Playeraudio::getTrackDuration() const
{
    return transportSource.getLengthInSeconds();
}

void Playeraudio::setSpeed(double ratio)
{
    resampleSource.setResamplingRatio(ratio);
}

// ==================== MARKER MANAGEMENT ====================

void Playeraudio::addMarker(double timestamp, const juce::String& name)
{
    markers.emplace_back(timestamp, name);
    std::sort(markers.begin(), markers.end());
    sendChangeMessage();
}

void Playeraudio::addMarkerAtCurrentPosition(const juce::String& name)
{
    addMarker(transportSource.getCurrentPosition(), name);
}

void Playeraudio::removeMarker(int index)
{
    if (index >= 0 && index < static_cast<int>(markers.size()))
    {
        // Update loop marker indices if needed
        if (index == loopStartMarkerIndex)
            loopStartMarkerIndex = -1;
        else if (loopStartMarkerIndex > index)
            loopStartMarkerIndex--;

        if (index == loopEndMarkerIndex)
            loopEndMarkerIndex = -1;
        else if (loopEndMarkerIndex > index)
            loopEndMarkerIndex--;

        markers.erase(markers.begin() + index);
        sendChangeMessage();
    }
}

void Playeraudio::removeAllMarkers()
{
    markers.clear();
    loopStartMarkerIndex = -1;
    loopEndMarkerIndex = -1;
    sendChangeMessage();
}

void Playeraudio::jumpToMarker(int index)
{
    if (index >= 0 && index < static_cast<int>(markers.size()))
    {
        transportSource.setPosition(markers[index].timestamp);
    }
}

void Playeraudio::jumpToMarkerTime(double timestamp)
{
    transportSource.setPosition(timestamp);
}

void Playeraudio::renameMarker(int index, const juce::String& newName)
{
    if (index >= 0 && index < static_cast<int>(markers.size()) && newName.isNotEmpty())
    {
        markers[index].name = newName;
        sendChangeMessage();
    }
}

void Playeraudio::setLoopStartMarker(int index)
{
    if (index >= 0 && index < static_cast<int>(markers.size()))
    {
        // Clear previous loop start marker
        if (loopStartMarkerIndex >= 0 && loopStartMarkerIndex < static_cast<int>(markers.size()))
        {
            markers[loopStartMarkerIndex].isLoopStart = false;
            markers[loopStartMarkerIndex].colour = juce::Colours::orange;
        }

        loopStartMarkerIndex = index;
        markers[index].isLoopStart = true;
        markers[index].colour = juce::Colours::green;

        // Enable looping if we have both start and end markers
        if (loopEndMarkerIndex >= 0)
        {
            isLooping = true;
        }

        sendChangeMessage();
    }
}

void Playeraudio::setLoopEndMarker(int index)
{
    if (index >= 0 && index < static_cast<int>(markers.size()))
    {
        // Clear previous loop end marker
        if (loopEndMarkerIndex >= 0 && loopEndMarkerIndex < static_cast<int>(markers.size()))
        {
            markers[loopEndMarkerIndex].isLoopEnd = false;
            markers[loopEndMarkerIndex].colour = juce::Colours::orange;
        }

        loopEndMarkerIndex = index;
        markers[index].isLoopEnd = true;
        markers[index].colour = juce::Colours::red;

        // Enable looping if we have both start and end markers
        if (loopStartMarkerIndex >= 0)
        {
            isLooping = true;
        }

        sendChangeMessage();
    }
}

void Playeraudio::clearLoopMarkers()
{
    if (loopStartMarkerIndex >= 0 && loopStartMarkerIndex < static_cast<int>(markers.size()))
    {
        markers[loopStartMarkerIndex].isLoopStart = false;
        markers[loopStartMarkerIndex].colour = juce::Colours::orange;
        loopStartMarkerIndex = -1;
    }

    if (loopEndMarkerIndex >= 0 && loopEndMarkerIndex < static_cast<int>(markers.size()))
    {
        markers[loopEndMarkerIndex].isLoopEnd = false;
        markers[loopEndMarkerIndex].colour = juce::Colours::orange;
        loopEndMarkerIndex = -1;
    }

    isLooping = false;
    sendChangeMessage();
}

int Playeraudio::findMarkerAtTime(double time, double tolerance) const
{
    for (int i = 0; i < static_cast<int>(markers.size()); ++i)
    {
        if (std::abs(markers[i].timestamp - time) <= tolerance)
        {
            return i;
        }
    }
    return -1;
}
