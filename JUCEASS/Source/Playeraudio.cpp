/*
  ==============================================================================

    Playeraudio.cpp
    Created: 21 Oct 2025 12:30:44am
    Author:  Muhammed

  ==============================================================================
*/

#include "Playeraudio.h"

Playeraudio::Playeraudio()
{
    formatManager.registerBasicFormats();
}

Playeraudio::~Playeraudio()
{
    transportSource.releaseResources();
}

void Playeraudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Playeraudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Handle looping manually
    transportSource.getNextAudioBlock(bufferToFill);

    if (isLooping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void Playeraudio::releaseResources()
{
    transportSource.releaseResources();
}

void Playeraudio::loadFile(const juce::File& audioFile)
{
    auto* reader = formatManager.createReaderFor(audioFile);
    if (reader != nullptr)
    {
        transportSource.stop();
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
        readerSource->setLooping(isLooping); // make reader source loopable
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
    }
}

void Playeraudio::start() { transportSource.start(); }
void Playeraudio::stop() { transportSource.stop(); }
void Playeraudio::pause() { transportSource.stop(); }
void Playeraudio::restart() { transportSource.setPosition(0.0); transportSource.start(); }
bool Playeraudio::isPlaying() const { return transportSource.isPlaying(); }

void Playeraudio::increment(double seconds)
{
    auto pos = transportSource.getCurrentPosition();
    transportSource.setPosition(std::min(pos + seconds, getLength()));
}

void Playeraudio::decrement(double seconds)
{
    auto pos = transportSource.getCurrentPosition();
    transportSource.setPosition(std::max(0.0, pos - seconds));
}

void Playeraudio::setPosition(double positionInSeconds)
{
    transportSource.setPosition(positionInSeconds);
}

void Playeraudio::setGain(float gain)
{
    if (!muted)
        transportSource.setGain(gain);
}

double Playeraudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double Playeraudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void Playeraudio::mute()
{
    if (!muted)
    {
        lastGain = transportSource.getGain();
        transportSource.setGain(0.0f);
        muted = true;
    }
}

void Playeraudio::unmute()
{
    if (muted)
    {
        transportSource.setGain(lastGain);
        muted = false;
    }
}

void Playeraudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

bool Playeraudio::isMuted() const
{
    return muted;
}
