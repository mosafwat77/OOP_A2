#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    transportSource.releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& audioFile)
{
    auto* reader = formatManager.createReaderFor(audioFile);

    if (reader != nullptr)
    {
        // Stop current playback and clear old source
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        // Create new reader source
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));

        // Set the new source with proper sample rate
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
    }
}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::setPosition(double positionInSeconds)
{
    transportSource.setPosition(positionInSeconds);
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}