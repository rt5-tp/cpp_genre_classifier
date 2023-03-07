#include <iostream>
#include <portaudio.h>
#include <fftw3.h>
#include <math.h>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_SECONDS (3)
#define NUM_SAMPLES (SAMPLE_RATE * NUM_SECONDS)

int main() {
    PaError err;
    PaStream *stream;
    fftw_complex *in, *out;
    fftw_plan p;
    double *data;
    int num_fft_samples = NUM_SAMPLES / 2 + 1;

    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cout << "Failed to initialize PortAudio: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Allocate memory for the input and output buffers
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NUM_SAMPLES);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_fft_samples);
    data = (double*) malloc(sizeof(double) * NUM_SAMPLES);

    // Create a plan for the FFT
    p = fftw_plan_dft_r2c_1d(NUM_SAMPLES, data, out, FFTW_ESTIMATE);

    // Open the default input stream
    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, NULL, NULL);
    if (err != paNoError) {
        std::cout << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cout << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Capture audio data for 3 seconds
    for (int i = 0; i < NUM_SAMPLES; i += FRAMES_PER_BUFFER) {
        err = Pa_ReadStream(stream, data + i, FRAMES_PER_BUFFER);
        if (err != paNoError) {
            std::cout << "Failed to read PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            return 1;
        }
    }

    // Stop the stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cout << "Failed to stop PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Execute the FFT
    fftw_execute(p);

    // Print the magnitude spectrum
    for (int i = 0; i < num_fft_samples; i++) {
        double real = out[i][0];
        double imag = out[i][1];
        double magnitude = sqrt(real*real + imag*imag);
        std::cout << i * (SAMPLE_RATE / 2) / num_fft_samples << " Hz: " << magnitude << std::endl;
    }

    // Clean up
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    free(data);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
