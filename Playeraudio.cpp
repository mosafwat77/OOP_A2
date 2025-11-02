#include "Playeraudio.h"

Playeraudio::Playeraudio()
{
    formatManager.registerBasicFormats();
    transportSource.setGain(1.0f);
}

Playeraudio::~Playeraudio()
{
    transportSource.releaseResources();
}

void Playeraudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    transportSource.setLooping(isLooping);
}

void Playeraudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
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
        transportSource.setSource(nullptr);
        readerSource.reset(new juce::AudioFormatReaderSource(reader, true));

        if (auto* r = readerSource->getAudioFormatReader())
        {
            transportSource.setSource(readerSource.get(),
                0,
                nullptr, 
                r->sampleRate); 
        }

        transportSource.setLooping(isLooping);
    }
}

void Playeraudio::start() { transportSource.start(); }
void Playeraudio::stop() { transportSource.stop(); }
void Playeraudio::pause() { transportSource.stop(); }

void Playeraudio::restart()
{
    transportSource.setPosition(0.0);
    transportSource.start();
}

void Playeraudio::setGain(float gain)
{
    if (!muted)
        transportSource.setGain(gain);
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

bool Playeraudio::isMuted() const
{
    return muted;
}

void Playeraudio::increment(double seconds)
{
    double pos = transportSource.getCurrentPosition();
    double len = transportSource.getLengthInSeconds();
    transportSource.setPosition(std::min(pos + seconds, len));
}

void Playeraudio::decrement(double seconds)
{
    double pos = transportSource.getCurrentPosition();
    transportSource.setPosition(std::max(0.0, pos - seconds));
}

void Playeraudio::setPosition(double positionInSeconds)
{
    transportSource.setPosition(positionInSeconds);
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
    transportSource.setLooping(shouldLoop);
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

bool Playeraudio::isPlaying() const
{
    return transportSource.isPlaying();
}

bool Playeraudio::isTrackPlaying() const
{
    return transportSource.isPlaying();
}

void Playeraudio::enableLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
    transportSource.setLooping(shouldLoop);
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
    
}
