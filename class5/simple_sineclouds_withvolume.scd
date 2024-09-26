(
SynthDef(\randomSineShiver, {
    arg freq = 440, rhy = 0.4, amp = 0.1, pan = 0;
    var sig, puls;
    puls = LFTri.kr(rhy).range(0, 1);  // Changed to use .range for smoother modulation
    sig = SinOsc.ar(freq) * puls * amp;
    Out.ar(0, Pan2.ar(sig, pan));
}).add;

~numOscs = 11;
~freqRange = [60, 8000]; // Minimum and maximum frequency in Hz
~pulsRange = [0.1, 2.0]; // Adjusted minimum to allow for slower modulations
~masterAmp = 0.5;  // Added master amplitude control

~pulsingSines = Array.fill(~numOscs, {
    var freq = exprand(~freqRange[0], ~freqRange[1]);
    var rhy = exprand(~pulsRange[0], ~pulsRange[1]);  // Changed to exprand for more natural distribution
    var pan = rrand(-1.0, 1.0);
    Synth(\randomSineShiver, [
        \freq, freq,
        \rhy, rhy,
        \amp, ~masterAmp / ~numOscs,  // Adjusted amplitude calculation
        \pan, pan
    ]);
});
)

// To stop all synths:
~pulsingSines.do(_.free);