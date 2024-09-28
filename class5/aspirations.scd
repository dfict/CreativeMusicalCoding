//gotta make the crickets

(
{
Pan2.ar(
	SinOsc.ar(rrand(80,108).midicps, 2pi.rand,
		Mix.ar(SinOsc.ar(exprand(0.3,8) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).max(0)
		* Mix.ar(SinOsc.ar(exprand(6,24) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).abs
	), 1.0.rand2);

}.play;
)



////
//frogs commented out with variables added

(
{
	var frog1, frog2, out, sig, mix;
	frog1= Mix.ar(SinOsc.ar(exprand(0.3,8) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).max(0);
	frog2= Mix.ar(SinOsc.ar(exprand(6,24) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).abs;
	sig = SinOsc.ar(rrand(80,108).midicps, 2pi.rand, frog1*frog2);
	out = Pan2.ar(sig, 1.0.rand2);
}.play;
)



///?///////////////////


(
~modSine = NodeProxy.audio(s, 2).play;

~modSine[0] = { |freqMin=80, freqMax=108, numInstances=1|
    var maxInstances = 20;
    var sig = Array.fill(maxInstances, { |i|
        var trigger = Changed.kr(numInstances) + Impulse.kr(0);
        var baseFreq = TExpRand.kr(freqMin.midicps, freqMax.midicps, trigger);
        var modLow = TExpRand.kr(0.3, 8, trigger);
        var modHigh = TExpRand.kr(6, 24, trigger);
        var pan = TRand.kr(-1, 1, trigger);

        Pan2.ar(
            SinOsc.ar(
                baseFreq,
                0,
                SinOsc.kr(modLow * LFNoise1.kr(0.1).range(0.7, 1.3), 0).max(0)
                * SinOsc.kr(modHigh * LFNoise1.kr(0.1).range(0.7, 1.3), 0).abs),
            pan
        ) * (i < numInstances).lag(0.1);  // Crossfade instances in/out
    });

    Mix(sig) * (1 / (numInstances.sqrt));  // Normalize amplitude
};

~modSine.gui;
)


// Change parameters
~modSine.set(\freqMin, 60, \freqMax, 109, \numInstances, 9);

// Regenerate all instances with new random values
~modSine.set(\numInstances, ~modSine.get(\numInstances));

// Stop
~modSine.stop(5);

///////////////



(
s.boot;  // Ensure the server is booted

s.waitForBoot({
    ~modSine = NodeProxy.audio(s, 2).play;

    ~modSine[0] = { |freqMin=80, freqMax=108, modDepthLow=0.1, modDepthHigh=0.1, numInstances=1|
        var maxInstances = 30;
        var sig = Array.fill(maxInstances, { |i|
            var trigger = Changed.kr(numInstances) + Impulse.kr(0);
            var baseFreq = TExpRand.kr(freqMin.midicps, freqMax.midicps, trigger);
            var modLow = TExpRand.kr(0.3, 8, trigger);
            var modHigh = TExpRand.kr(6, 24, trigger);
            var pan = TRand.kr(-1, 1, trigger);

            Pan2.ar(
                SinOsc.ar(
                    baseFreq,
                    0,
                    SinOsc.kr(modLow * LFNoise1.kr(0.1).range(0.7, 1.3), 0, modDepthLow).max(0)
                    * SinOsc.kr(modHigh * LFNoise1.kr(0.1).range(0.7, 1.3), 0, modDepthHigh).abs
                ),
                pan
            ) * (i < numInstances).lag(0.1);  // Crossfade instances in/out
        });

        Mix(sig) * (1 / (numInstances.sqrt));  // Normalize amplitude
    };

    // Set the spec for numInstances to be an integer from 1 to 30
    Spec.add(\numInstances, ControlSpec(1, 30, \lin, 1, 1));

    // Create a custom GUI with integer number box for numInstances
    ~modSine.gui((\numInstances: \i32numberbox));
});
)

~modSine.gui

// Change parameters
~modSine.set(\freqMin, 60, \freqMax, 90, \modDepthLow, 0.2, \modDepthHigh, 0.15, \numInstances, 5);

// Regenerate all instances with new random values
~modSine.set(\numInstances, ~modSine.get(\numInstances));

// Stop
~modSine.stop(5);

////////////////
(
Ndef(\workingMod, {
    arg freqMin = 80, freqMax = 108, modDepth = 0.1;

    // Use Rand UGens to generate new random values each time the synth is triggered
    var baseFreq = Rand(freqMin, freqMax).midicps;
    var modLow = Rand(0.3, 8);
    var modHigh = Rand(6, 24);
    var pan = Rand(-1, 1);

    Pan2.ar(
        SinOsc.ar(
            baseFreq,
            Rand(0, 2pi),
            Mix.ar(SinOsc.ar(modLow * [Rand(0.7, 1.3), 1], [Rand(0, 2pi), Rand(0, 2pi)], modDepth)).max(0)
            * Mix.ar(SinOsc.ar(modHigh * [Rand(0.7, 1.3), 1], [Rand(0, 2pi), Rand(0, 2pi)], modDepth)).abs
        ),
        pan
    );
}).play;
)

// Change frequency range (in MIDI notes)
Ndef(\workingMod).set(\freqMin, 60, \freqMax, 90);

// Adjust modulation depth
Ndef(\workingMod).set(\modDepth, 0.2);

// Regenerate with new random values
Ndef(\workingMod).send;

// Stop
Ndef(\workingMod).stop(5);