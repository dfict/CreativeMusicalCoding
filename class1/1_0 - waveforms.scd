s.boot; // Key command ctrl+b also boots the server
FreqScope.new; // Create oscilloscope for spectral analysis

{ WhiteNoise.ar(0.1) }.play; // Some white noise
{ PinkNoise.ar(0.1) }.scope; // Some pink noise
{ LPF.ar( PinkNoise.ar(0.5),1000) }.scope; // Pink noise sent through a low-pass filter at 1kHz
{ LPF.ar( WhiteNoise.ar(0.5), 1000) }.scope; // White noise sent thru a LPF at 1kHz

// SUBTRACTIVE SYNTHESIS (waveshaping, filtering out certain frequencies)

{ LPF.ar( WhiteNoise.ar(0.15), Line.kr(8000,800,10)) }.scope; // White noise sent thru a descending LPF envelope : 8kHz to 800Hz over 10 seconds

// Sawtooth waves triggered at different frequencies
{ Saw.ar(51,0.1) }.scope
{ Saw.ar(55,0.1) }.scope
{ Saw.ar(50,0.1) ! 2}.scope // 2 channels

// Impulse response triggered at different frequencies
{ Blip.ar(273,1,0.1) }.scope;
{ Blip.ar(275,1,0.1) }.scope;
{ Blip.ar(271,1,0.1) }.scope;

{ Resonz.ar( WhiteNoise.ar(0.15), Line.kr(10000,200,10)) }.scope; // White noise sent thru a linear resonance envelope : 10kHz to 200Hz in 10 seconds
{ LPF.ar( PinkNoise.ar(0.15), Line.kr(8000,0,10)) }.scope; // Pink noise sent thru a linear LPF envelope : 8kHz to 0Hz in 10 seconds

{ Resonz.ar(LFNoise0.ar(666),5000,0.1) }.scope // Low-frequency noise sent thru a resonant filter at different fundamental & resonant frequencies
{ Resonz.ar(LFNoise0.ar(444),Line.kr(2000,500,2),0.1) }.scope // Low-freq noise sent thru a linar resonance envelope descending from 2kHz to 500Hz
{ Resonz.ar(LFNoise0.ar(222),Line.kr(5000,500,2),0.1) }.scope // Low-freq noise sent thru a linar resonance envelope descending from 5kHz to 500Hz


( // Method for passing a LF noise signal thru a linear resonance envelope
{
var signal, envelope, filter;

	signal=LFNoise0.ar(400);
	envelope=Line.kr(10000,1000,10);
	filter=Resonz.ar(signal,envelope,0.1);
	filter; // Return the filter to play the sound

}.scope;
)

// ADDITIVE SYNTHESIS (triggering multiple signals at the same time to cause frequency interference)

{ SinOsc.ar(360,0,0.1) }.scope;
{ SinOsc.ar(160,0,0.1) + SinOsc.ar(222,0,0.1)}.scope;

{ Pan2.ar(WhiteNoise.ar(0.01)) }.scope; // Pan2 creates a 2-channel stereo image of white noise signal

{SinOsc.ar([460,559,595],0,0.1)}.scope // Creates a 3-channel SinOsc using an array of frequencies as the input

{Mix(SinOsc.ar([400,660],0,0.1))}.scope // Mix turns a two channel SinOsc signal into mono
{Pan2.ar(Mix(SinOsc.ar([400,660],0,0.1)),MouseX.kr(-1,1))}.scope // Adds the mouse's x-coordinates as an input for one channel's frequency


// SAWTOOTH WAVE, sum of n harmonics, decays as 1/n with Fourier series
(
{
	var n = 24; // Using the first 6 harmonics of the series
	var saw = Mix.fill(n,{|i|
		var mult= ((-1)**i)/(2*(i+1));
		mult*SinOsc.ar(440*(i+1))}); // Sums the harmonics
	Pan2.ar(saw,0); // Outputs saw as a center-panned stereo image

}.scope;
)

// SQUARE WAVE, sum of odd harmonics, decays as 1/n
(
{
	var n = 4; // First 4 harmonics
	var square = Mix.fill(n,{|i|
		var j = 2*i+1;
		SinOsc.ar(440*j)/j})*0.25; // Sums the harmonics
	Pan2.ar(square,0); // Outputs square as center-panned stereo image

}.scope;
)

// TRIANGLE WAVE, sum of odd harmonics, decays as (+/-)1/(n**2)
// note: this looks really cool on the scope after triggering it 3 or 4 times!!!
(
{
	var n = 17; // First 17 harmonics
	var tri = Mix.fill(n,{|i|
		var k = 2*i+1;
		var mult=(-1)**i/(k**2);
		mult*SinOsc.ar(440*k)}); // Sums the harmonics
	Pan2.ar(tri,0); // Outputs tri as center-panned stereo image

}.scope;
)

// BELL TONE, a complex additive wave where the fundamental is actually one of the weakest components!
// Frequencies for a church bell a.k.a. "Carillon" bell; frequencies may be analyzed using spectral analysis
// Quick way to generate an array:
// (1..10;)
// Partials of equal temperament:
(
{
	var equal = [0.50,1.00,1.19,1.50,2.00,2.52,2.67,3.00,4.00,5.34,6.73,8.00];
	var equal_bell = SinOsc.ar(200*equal,0,0.1);
	Mix(SinOsc.ar(equal_bell));
}.scope;
)

// Partials of just tuning:
(
{
	var just = [0.50,1.00,1.20,1.50,2.00,2.05,2.67,3.00,4.00,5.33,6.67,8.00];
	var just_bell = SinOsc.ar(200*just,0,0.1);
	Mix(SinOsc.ar(just_bell));
}.scope;
)

// Partials of "actual" bell:
(
{
	var actual = [0.50,1.00,1.18,1.51,2.00,2.51,2.66,3.01,4.17,5.43,6.80,8.22];
	var actual_bell = SinOsc.ar(200*actual,0,0.1);
	Mix(SinOsc.ar(actual_bell));
}.play;
)

// Offsetting and scaling
(
{
    var cutoff = SinOsc.ar(1, mul: MouseY.kr(0, 100), add: MouseX.kr(100));
    LPF.ar(WhiteNoise.ar, cutoff);
}.scope;
)



