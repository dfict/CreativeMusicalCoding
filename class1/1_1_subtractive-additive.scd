// A comparison between a LPF and HPF with the same cutoff frequency values, using mouse control to quickly vary parameters. The x-value is the median cutoff frequency (or "add" parameter), and the y-value is the frequency of the sine oscillator that controls how quickly the cutoff frequency is modulating between the scaled output of +/-1700Hz.

( // LPF applied to white noise; cutoff frequency is a sine wave with input parameters controlled by the mouse cursor location
{
	var cutoff = SinOsc.ar(MouseY.kr(0,200))*1700+MouseX.kr(0,2000);
	LPF.ar(WhiteNoise.ar,freq:cutoff, mul:0.01) ! 2;
}.play
)

( // HPF applied to white noise; cutoff frequency is a sine wave with input parameters controlled by the mouse cursor location
{
	var cutoff = SinOsc.ar(MouseY.kr(0,200))*1700+MouseX.kr(0,2000);
	HPF.ar(WhiteNoise.ar,freq:cutoff);
}.scope
)


// This is just some playing around with wave modulation and addition, using mouse controls to quickly vary the arguments of the waves to find cool interference patterns (beats). There is a fairly noticeable difference in sound between triggering waves one after the other vs. summing them in-line, as demonstrated below.

// Signal Modulation
{SinOsc.ar(MouseX.kr(440,880))*SinOsc.ar(MouseX.kr(440))}.scope
{SinOsc.ar(MouseX.kr(880))*SinOsc.ar(MouseX.kr(440))}.scope

// Signal Modulation 2
{SinOsc.ar(MouseX.kr(440,880))*SinOsc.ar(MouseX.kr(440))+ SinOsc.ar(MouseX.kr(880))*SinOsc.ar(MouseX.kr(440))}.scope

// Signal Modulation`3
{SinOsc.ar(MouseX.kr(440,880))*SinOsc.ar(MouseX.kr(333))}.scope
{SinOsc.ar(MouseX.kr(700))*SinOsc.ar(MouseX.kr(282))}.scope


// Trying to figure out how to record directly to MP#... seems like maybe not a thing in SC...
SoundFile.openWrite(headerFormat:"WAV");

// Neat list of supported recording formats https://doc.sccode.org/Classes/SoundFile.html#-headerFormat

s.record(duration:15,numChannels:2,recHeaderFormat:"WAV");
s.stopRecording;


s.record;
s.stopRecording;