// Here is the very first example from the first release of SuperCollider 2

(// analog bubbles
{	var f;  // without a declaration, this would use the interpreter variable f
	f = LFSaw.kr(0.4, 0, 24, LFSaw.kr([8,7.23], 0, 3, 81)).midicps; // glissando function
	CombN.ar(SinOsc.ar(f, 0, 0.04), 0.2, 0.2, 4) 					// echoing sine wave
}.play)

// While the example sounds lovely, it is quite difficult to unpack what, exactly, is going on.
// Below we buid it up stage by stage

( // the basic sine tone

{ var k, f, a; 			// 'k' for key number, 'f' for frequency, 'a' for audio
	k = 81;				// MIDI key 81 it an octave above concert A
	f = k.midicps;		// convert the MIDI key number to a frequency (880)
	a = SinOsc.ar(f);	// create the audio signal with a SinOsc UGen
	a = a * 0.04		// scale its volume to 28 dB below maximum
}.play;


)

( // now in stereo

{ var k, f, a;
	k = 81;
	f = k.midicps;
	a = SinOsc.ar(f);
	a = a * 0.04;
	[a,a];				// returning an array with two elements generates a stereo output
}.play;

/* NOTE: Arrays are used to represent multi-channel audio in a synthesis function
*/

)

// the basic glissando ranging from A2 to A6
(
{  var k, f, a;
	k = LFSaw.kr(0.4);		// a ramp ranging from -1 to 1
	k = k * 24;				// now it ranges from -24 to 24
	k = k + 81;				// now from 56 to 104
	f = k.midicps; 			// convert from MIDI key numbers (where 60 = Middle C )
							// to frequencies (where 261.6255653006 Cycles per Second = middle C)
	a = SinOsc.ar(f);		// apply that to the frequency of a sine tone
	a = a * 0.04;			// make it 28dB softer
							// the last expression inside the function
							// becomes the sound played
}.play
)

(// now with some echo
{var k, f, a;
	k = LFSaw.kr(0.4) * 24 + 81;
	f = k.midicps;

	a = SinOsc.ar(f);
	a = a * 0.04;
	CombN.ar(a, 0.2, 0.2, 4) 	// the arguments are: audio, max_delay, delay, echo_duration
}.play
)

(// now stereo// the
{
var k, f, a;
	k = LFSaw.kr(0.4) * 24 + [81, 81];	// this now creates a two element array
	f = k.midicps; 						// the result is also an array
	a = SinOsc.ar(f);					// the resultant array of SinOsc's is a STEREO SIGNAL
	a = a * 0.04;						// attenuate the stereo pair
	CombN.ar(a, 0.2, 0.2,4) 			// while this creates two separate delays
										// we hear it as a mono signal out of two speakers
}.play
)

(// add one modulator
{
	var k, f, a, mod1;
	mod1 = LFSaw.kr(8);
	mod1 = mod1.range( 78, 84);

	k = LFSaw.kr(0.4) * 24 + [mod1, mod1];	// center point for the ramp moves from 77 to 83
	f = k.midicps;
	a = SinOsc.ar(f) * 0.04;
	CombN.ar(a, 0.2, 0.2, 4)
}.play)

(// now two modulators
{
	var k, f, a, mod1, mod2;
	mod1 = LFSaw.kr(8);
	mod1 = mod1.range( 78, 84);

	mod2 = LFSaw.kr(7.23);					// define the second modulator
	mod2 = mod2.range( 78, 84);

	k = LFSaw.kr(0.4) * 24 + [mod1, mod2];	// use the second modulator
	f = k.midicps;
	a = SinOsc.ar(f) * 0.04;
	CombN.ar(a, 0.2, 0.2, 4)
}.play
)

