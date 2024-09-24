(
SynthDef(\randomSine, {
    arg freq = 440, amp = 0.1, pan = 0;
    var sig;
    sig = SinOsc.ar(freq) * amp;
    Out.ar(0, Pan2.ar(sig, pan));
}).add;

~numOscs = 24;
~freqRange = [200, 2000]; // Minimum and maximum frequency in Hz

~sines = Array.fill(~numOscs, {
    var freq = exprand(~freqRange[0], ~freqRange[1]);
    var pan = rrand(-1.0, 1.0);
    Synth(\randomSine, [
        \freq, freq,
        \amp, 1 / ~numOscs, // Adjust amplitude to avoid clipping
        \pan, pan
    ]);
});
)


// To stop all synths:
 ~sines.do(_.free);


