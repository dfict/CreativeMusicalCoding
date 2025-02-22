// Using "scope" to look at Sound Spectrum and  Oscillator Waveforms



// we want a nice Ndef:
s.boot;					// Wait for this to finish...
s.scope(2);				// a new message!  this provides a picture of the audio output

f = 215.35;				// this frequency keeps the waveform from drifting in the
a = Ndef(\a);
a.clear;				// make sure there is nothing left from previous use
a.initBus(\audio, 2);	// Ndef tries to guess what you want
						// Sometimes it is better to just spell it out;
a.play;

// Saw, Pulse, LFSaw, LFPulse, and LFTri are waveforms that can be
// easily created as mixtures of sinewaves at specific frequencies and amplitudes
// The frequencies are in the "harmonic series" which means they are integer
// multiples of a "fundamental frequency"
// For example, here are the first 10 harmonics of 200 Hx
//	[200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]
// We can write it more compactly as:
//	200 * (1..10);

// Let's look at some waveforms
(
~blip = { | numHarms = 1 |
	{|freq = 215.35 |
		Mix.fill(numHarms, {| i |
			i = i + 1;
			SinOsc.ar(i * freq, 0, 0.1/numHarms)
		})
	}
};
)
a.play
	a.source= ~blip.(1)		// source clears everythin else out of the Ndef
	a[0]= ~blip.(2)
	a[0]= ~blip.(3)
	a[0]= ~blip.(4)
	a[0]= ~blip.(5)
	a[0]= ~blip.(6)
	a[0]= ~blip.(7)
	a[0]= ~blip.(8)
	a[0]= ~blip.(9)
	a[0]= ~blip.(10)
	a[0]= ~blip.(11)
	a[0]= ~blip.(12)
	a[0]= ~blip.(13)
	a[0]= ~blip.(14)
	a[0]= ~blip.(15)
	a[0]= ~blip.(16)

// now we will do the same thing, but with the phase position of
// each overtone gently scrambled.  While the waveform changes quite a lot
// the effect on the sound is relatively subtle
(
~blip = { | numHarms = 1 |
	{|freq = 215.35 |
		Mix.fill(numHarms, {| i |
			i = i + 1;
			SinOsc.ar(i * freq, LFNoise1.kr(1,pi), 0.1/numHarms)
		})
	}
};
)

	a[0]= ~blip.(1)
	a[0]= ~blip.(2)
	a[0]= ~blip.(3)
	a[0]= ~blip.(4)
	a[0]= ~blip.(5)
	a[0]= ~blip.(6)
	a[0]= ~blip.(7)
	a[0]= ~blip.(8)
	a[0]= ~blip.(10)
	a[0]= ~blip.(11)
	a[0]= ~blip.(12)
	a[0]= ~blip.(13)
	a[0]= ~blip.(14)
	a[0]= ~blip.(15)
	a[0]= ~blip.(16)

// But if we mix another channel in, the change in phase leads to
// constructive and destructive interference, which gives quite a lot
// of life to the sound.
// This is often called "chorusing" as it is similar to the effect when
// many people are singing together and, being human, have slightly different
// pitches
	a[1] = ~blip.(16)

	a.source = 0;
// Most instruments have overtones that are softer as they get further from
// the fundamental.  The same is true for two basic synthesizer waveforms.
// The Saw wave has overtones whose amplitude is inversely proportional to
// their overtone number. So Overtone N has a volumne of 1/N
(
~saw = { | numHarms = 1 |
	{|freq = 215.35 |
		Mix.fill(numHarms, {| n |
			n = n + 1;
			SinOsc.ar(n * freq, 0, 0.1/n)
		})
	}
};
)
// let's make a Saw wave component by component
a[0] = ~saw.(1);
	a[0] = ~saw.(2);
	a[0] = ~saw.(3);
	a[0] = ~saw.(4);
	a[0] = ~saw.(5);
	a[0] = ~saw.(6);
	a[0] = ~saw.(7);
	a[0] = ~saw.(8);
	a[0] = ~saw.(10);
	a[0] = ~saw.(12);
	a[0] = ~saw.(14);
	a[0] = ~saw.(16);

a.source = 0;


(
// Let's invert the waveform by inverting each component
~reversesaw = { | numHarms = 1 |
	{ |freq = 215.35 |
		Mix.fill(numHarms, {| i |
			i = i + 1;
			SinOsc.ar(i * freq, 0, 0.1/i) * -1;
		});
	}
};
)
	a[0] = ~reversesaw.(1);
	a[0] = ~reversesaw.(2);
	a[0] = ~reversesaw.(3);
	a[0] = ~reversesaw.(4);
	a[0] = ~reversesaw.(5);
	a[0] = ~reversesaw.(6);
	a[0] = ~reversesaw.(7);
	a[0] = ~reversesaw.(8);
	a[0] = ~reversesaw.(10);
	a[0] = ~reversesaw.(12);
	a[0] = ~reversesaw.(14);
	a[0] = ~reversesaw.(16);

(
// Now let's scramble the phase of the components a little bit
~scramblesaw = { | numHarms = 1 |
	{ |freq = 215.35, rate = 1 |
		Mix.fill(numHarms, {| i |
			i = i + 1;
			SinOsc.ar(i * freq, LFNoise1.kr(rate,2pi), -0.1/i);
		});
	}
};
	a[0] = ~scramblesaw.(16);
)
// This time, we can play with how fast the phase scrambling happens:
a.set(\rate, 2)
// now, let's chorus:

	a[1] = ~scramblesaw.(16);

// A square wave has odd harmonics
(
~square = { | numHarms = 1 |
	{|freq = 215.35, rate = 1 |
		Mix.fill(numHarms, {| i |
			i = i * 2 + 1;		// make it go 1, 3, 5, 7....
			SinOsc.ar(i * freq, 0, 0.1/i)
		})
	}
};

)
	a.source = ~square.(16)
	a[1] = ~square.(16)
	a.set(\rate, 0.1);
	a.set(\freq, 215.35/4)

// A triangle wave also has odd harmonics,
// but their volumes fall off at the square of the harmonic number
// and the phase of the components alternates between 0 and pi
(
	~tri = { | numHarms = 1 |
	{|freq = 215.35, rate = 1 |
		Mix.fill(numHarms, {| i |
			var n;
			n = i * 2 + 1;		// make it go 1, 3, 5, 7....
			SinOsc.ar(n * freq, pi * i mod: 2pi, 0.1/n.squared)
		})
	}
};

)
	a.source = ~tri.(16)
a.play
a.source=0