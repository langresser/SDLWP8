/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of _this software.

  Permission is granted to anyone to use _this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of _this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use _this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_AUDIO_DRIVER_XAUDIO2

extern "C" {
#include "SDL_audio.h"
#include "../SDL_audio_c.h"
#include "../SDL_sysaudio.h"
#include "SDL_assert.h"
};

#define INITGUID 1
#define XAUDIO2_HELPER_FUNCTIONS 1
#include <wrl\client.h>
#include <d3d11_1.h>
#include <xaudio2.h>
#include <xaudio2fx.h>

/* Hidden "_this" pointer for the audio functions */
#define _THIS   SDL_AudioDevice * _this

struct SDL_PrivateAudioData
{
    IXAudio2 *ixa2;
    IXAudio2SourceVoice *source;
    IXAudio2MasteringVoice *mastering;
    HANDLE semaphore;
    Uint8 *mixbuf;
    int mixlen;
    Uint8 *nextbuf;
};


static void
XAUDIO2_DetectDevices(int iscapture, SDL_AddAudioDevice addfn)
{
}

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	VoiceCallback(){}
	~VoiceCallback(){}

	//Called when the voice has just finished playing a contiguous audio stream.
	void __stdcall OnStreamEnd() {}

	//Unused methods are stubs
	void __stdcall OnVoiceProcessingPassEnd() { }
	void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void __stdcall OnBufferEnd(void * pBufferContext)    {
		SDL_AudioDevice *_this = (SDL_AudioDevice *) pBufferContext;
		ReleaseSemaphore(_this->hidden->semaphore, 1, NULL);
	}
	void __stdcall OnBufferStart(void * pBufferContext) {    }
	void __stdcall OnLoopEnd(void * pBufferContext) {    }
	void __stdcall OnVoiceError(void * pBufferContext, HRESULT Error) { }
};

static Uint8 *
XAUDIO2_GetDeviceBuf(_THIS)
{
    return _this->hidden->nextbuf;
}

static void
XAUDIO2_PlayDevice(_THIS)
{
    XAUDIO2_BUFFER buffer;
    Uint8 *mixbuf = _this->hidden->mixbuf;
    Uint8 *nextbuf = _this->hidden->nextbuf;
    const int mixlen = _this->hidden->mixlen;
    IXAudio2SourceVoice *source = _this->hidden->source;
    HRESULT result = S_OK;

    if (!_this->enabled) { /* shutting down? */
        return;
    }

    /* Submit the next filled buffer */
    SDL_zero(buffer);
    buffer.AudioBytes = mixlen;
    buffer.pAudioData = nextbuf;
    buffer.pContext = _this;
	buffer.LoopCount = 1;

    if (nextbuf == mixbuf) {
        nextbuf += mixlen;
    } else {
        nextbuf = mixbuf;
    }
    _this->hidden->nextbuf = nextbuf;

    result = source->SubmitSourceBuffer(&buffer);
    if (result == XAUDIO2_E_DEVICE_INVALIDATED) {
        /* !!! FIXME: possibly disconnected or temporary lost. Recover? */
    }

    if (result != S_OK) {  /* uhoh, panic! */
         source->FlushSourceBuffers();
         _this->enabled = 0;
    }
}

static void
XAUDIO2_WaitDevice(_THIS)
{
    if (_this->enabled) {
        WaitForSingleObjectEx(_this->hidden->semaphore, INFINITE, 0);
    }
}

static void
XAUDIO2_WaitDone(_THIS)
{
    IXAudio2SourceVoice *source = _this->hidden->source;
    XAUDIO2_VOICE_STATE state;
    SDL_assert(!_this->enabled);  /* flag that stops playing. */
    source->Discontinuity();
    source->GetState(&state);
    while (state.BuffersQueued > 0) {
        WaitForSingleObjectEx(_this->hidden->semaphore, INFINITE, 0);
        source->GetState(&state);
    }
}


static void
XAUDIO2_CloseDevice(_THIS)
{
    if (_this->hidden != NULL) {
        IXAudio2 *ixa2 = _this->hidden->ixa2;
        IXAudio2SourceVoice *source = _this->hidden->source;
        IXAudio2MasteringVoice *mastering = _this->hidden->mastering;

        if (source != NULL) {
			source->Stop();
            source->FlushSourceBuffers();
            source->DestroyVoice();
        }
        if (ixa2 != NULL) {
            ixa2->StopEngine();
        }
        if (mastering != NULL) {
            mastering->DestroyVoice();
        }
        if (ixa2 != NULL) {
            ixa2->Release();
        }
        SDL_free(_this->hidden->mixbuf);
        if (_this->hidden->semaphore != NULL) {
            CloseHandle(_this->hidden->semaphore);
        }

        SDL_free(_this->hidden);
        _this->hidden = NULL;
    }
}

static int
XAUDIO2_OpenDevice(_THIS, const char *devname, int iscapture)
{
    HRESULT result = S_OK;
    WAVEFORMATEX waveformat;
    int valid_format = 0;
    SDL_AudioFormat test_format = SDL_FirstAudioFormat(_this->spec.format);
    IXAudio2 *ixa2 = NULL;
    IXAudio2SourceVoice *source = NULL;
    UINT32 devId = 0;  /* 0 == system default device. */

    if (iscapture) {
        return SDL_SetError("XAudio2: capture devices unsupported.");
    } else if (XAudio2Create(&ixa2, 0, XAUDIO2_DEFAULT_PROCESSOR) != S_OK) {
        return SDL_SetError("XAudio2: XAudio2Create() failed at open.");
    }

    if (devname != NULL) {
        UINT32 devcount = 0;
        UINT32 i = 0;

        if (i == devcount) {
            ixa2->Release();
            return SDL_SetError("XAudio2: Requested device not found.");
        }
    }

    /* Initialize all variables that we clean on shutdown */
    _this->hidden = (struct SDL_PrivateAudioData *)
        SDL_malloc((sizeof *_this->hidden));
    if (_this->hidden == NULL) {
        return SDL_OutOfMemory();
    }
    SDL_memset(_this->hidden, 0, (sizeof *_this->hidden));

    _this->hidden->ixa2 = ixa2;
    _this->hidden->semaphore = CreateSemaphoreEx(NULL, 1, 2, NULL, 0, 0);
    if (_this->hidden->semaphore == NULL) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: CreateSemaphore() failed!");
    }

    while ((!valid_format) && (test_format)) {
        switch (test_format) {
        case AUDIO_U8:
        case AUDIO_S16:
        case AUDIO_S32:
        case AUDIO_F32:
            _this->spec.format = test_format;
            valid_format = 1;
            break;
        }
        test_format = SDL_NextAudioFormat();
    }

    if (!valid_format) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: Unsupported audio format");
    }

    /* Update the fragment size as size in bytes */
    SDL_CalculateAudioSpec(&_this->spec);

    /* We feed a Source, it feeds the Mastering, which feeds the device. */
    _this->hidden->mixlen = _this->spec.size;
    _this->hidden->mixbuf = (Uint8 *) SDL_malloc(2 * _this->hidden->mixlen);
    if (_this->hidden->mixbuf == NULL) {
        XAUDIO2_CloseDevice(_this);
        return SDL_OutOfMemory();
    }
    _this->hidden->nextbuf = _this->hidden->mixbuf;
    SDL_memset(_this->hidden->mixbuf, 0, 2 * _this->hidden->mixlen);

    /* We use XAUDIO2_DEFAULT_CHANNELS instead of _this->spec.channels. On
       Xbox360, _this means 5.1 output, but on Windows, it means "figure out
       what the system has." It might be preferable to let XAudio2 blast
       stereo output to appropriate surround sound configurations
       instead of clamping to 2 channels, even though we'll configure the
       Source Voice for whatever number of channels you supply. */
    result = ixa2->CreateMasteringVoice(&_this->hidden->mastering,
											XAUDIO2_DEFAULT_CHANNELS, _this->spec.freq, 0,
											nullptr, nullptr, AudioCategory_GameMedia);
//                                            XAUDIO2_DEFAULT_CHANNELS,
//                                            _this->spec.freq, 0, devId, NULL);
    if (result != S_OK) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: Couldn't create mastering voice");
    }

    SDL_zero(waveformat);
    if (SDL_AUDIO_ISFLOAT(_this->spec.format)) {
        waveformat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    } else {
        waveformat.wFormatTag = WAVE_FORMAT_PCM;
    }
    waveformat.wBitsPerSample = SDL_AUDIO_BITSIZE(_this->spec.format);
    waveformat.nChannels = _this->spec.channels;
    waveformat.nSamplesPerSec = _this->spec.freq;
    waveformat.nBlockAlign =
        waveformat.nChannels * (waveformat.wBitsPerSample / 8);
    waveformat.nAvgBytesPerSec =
        waveformat.nSamplesPerSec * waveformat.nBlockAlign;

	static VoiceCallback callbacks;
	XAUDIO2_SEND_DESCRIPTOR descriptors[1];
	descriptors[0].pOutputVoice = _this->hidden->mastering;
	descriptors[0].Flags = 0;
	XAUDIO2_VOICE_SENDS sends = {0};
	sends.SendCount = 1;
	sends.pSends = descriptors;
    result = ixa2->CreateSourceVoice(&source, &waveformat, 0,
                                        1.0f, &callbacks, &sends, nullptr);
    if (result != S_OK) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: Couldn't create source voice");
    }
    _this->hidden->source = source;

    /* Start everything playing! */
    result = ixa2->StartEngine();
    if (result != S_OK) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: Couldn't start engine");
    }

    result = source->Start(0, XAUDIO2_COMMIT_NOW);
    if (result != S_OK) {
        XAUDIO2_CloseDevice(_this);
        return SDL_SetError("XAudio2: Couldn't start source voice");
    }

    return 0; /* good to go. */
}

static void
XAUDIO2_Deinitialize(void)
{
}


static int
XAUDIO2_Init(SDL_AudioDriverImpl * impl)
{
    /* XAudio2Create() is a macro that uses COM; we don't load the .dll */
    IXAudio2 *ixa2 = NULL;
    if (XAudio2Create(&ixa2) != S_OK) {
        SDL_SetError("XAudio2: XAudio2Create() failed at initialization");
        return 0;  /* not available. */
    }

    /* Set the function pointers */
    impl->DetectDevices = XAUDIO2_DetectDevices;
    impl->OpenDevice = XAUDIO2_OpenDevice;
    impl->PlayDevice = XAUDIO2_PlayDevice;
    impl->WaitDevice = XAUDIO2_WaitDevice;
    impl->WaitDone = XAUDIO2_WaitDone;
    impl->GetDeviceBuf = XAUDIO2_GetDeviceBuf;
    impl->CloseDevice = XAUDIO2_CloseDevice;
    impl->Deinitialize = XAUDIO2_Deinitialize;

    return 1;   /* _this audio target is available. */
}

extern "C" AudioBootStrap XAUDIO2_bootstrap = {
    "xaudio2", "XAudio2", XAUDIO2_Init, 0
};

#endif  /* SDL_AUDIO_DRIVER_XAUDIO2 */

/* vi: set ts=4 sw=4 expandtab: */
