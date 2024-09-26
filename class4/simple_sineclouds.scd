(
SynthDef(\randomSine, {
    arg freq = 440, amp = 0.1, pan = 0;
    var sig;
    sig = SinOsc.ar(freq) * amp;
    Out.ar(0, Pan2.ar(sig, pan, 0.5));
}).add;

~numOscs = 50;
~freqRange = [20, 8000]; // Minimum and maximum frequency in Hz

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

~sines.do(_.scope);
// To stop all synths:
 ~sines.do(_.free);

{v = SinOsc.ar(440, 1.0, LFTri.ar(1, 0, 0.25, 0.75))}.scope;




