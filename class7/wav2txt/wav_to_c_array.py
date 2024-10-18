import wave
import numpy as np
import os

def wav_to_c_array(input_file, output_file, max_samples=1024):
    try:
        with wave.open(input_file, 'rb') as wav_file:
            # Get WAV file parameters
            channels, sample_width, framerate, n_frames, _, _ = wav_file.getparams()
            
            # Read raw audio data
            frames = wav_file.readframes(n_frames)
        
        # Convert to numpy array
        samples = np.frombuffer(frames, dtype=np.int16)
        
        # If stereo, take only one channel
        if channels == 2:
            samples = samples[::2]
        
        # Downsample to 8kHz
        samples = samples[::int(framerate/8000)]
        
        # Limit number of samples
        samples = samples[:max_samples]
        
        # Normalize to 0-255 range
        samples = ((samples - samples.min()) / (samples.max() - samples.min()) * 255).astype(np.uint8)
        
        # Write C array to file
        with open(output_file, 'w') as f:
            f.write("const uint8_t PROGMEM audioSample[] = {\n")
            for i in range(0, len(samples), 8):
                f.write("    " + ", ".join(map(str, samples[i:i+8])) + ",\n")
            f.write("};\n")
            f.write(f"#define SAMPLE_LENGTH {len(samples)}\n")
        
        print(f"Conversion successful. Output written to {output_file}")
    except FileNotFoundError:
        print(f"Error: The file '{input_file}' was not found.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

# Get user input for file names
input_file = input("Enter the name of the input WAV file: ")
output_file = input("Enter the name for the output C file: ")

# Ensure input file exists
if not os.path.exists(input_file):
    print(f"Error: The file '{input_file}' does not exist in the current directory.")
    print(f"Current working directory: {os.getcwd()}")
    print("Files in current directory:")
    print("\n".join(os.listdir('.')))
else:
    wav_to_c_array(input_file, output_file)