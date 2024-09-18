//The primary and more flexible procedure for creating sound is to first create a SynthDef and then execute the SynthDef by calling a Synth explicitly.

//A SynthDef is essentially a recipe for a particular sound, and a Synth is the execution of that recipe.

//"Function.play is often more convienent than SynthDef.play, particularly for short examples and quick testing. But where reuse and maximum flexibility are of greater importance, SynthDef and its various methods are usually the better choice."


s.boot;

(
z = {
	arg noiseHz=8;
	var freq, amp, sig;
	freq = LFNoise0.kr(noiseHz).exprange(200,1000);
	amp = LFNoise1.kr(12).exprange(0.02,1);
	sig = SinOsc.ar(freq) * amp;
}.play
)

// as a function.play


(
SynthDef.new(\sineTest, {
	arg noiseHz=8;
	var freq, amp, sig;
	freq = LFNoise0.kr(noiseHz).exprange(200,1000);
	amp = LFNoise1.kr(12).exprange(0.02,1);
	sig = SinOsc.ar(freq) * amp;
	Out.ar([0,1], sig); //the critical line of code!
}).add
)

//as a synth def

//To execute the SynthDef, we create a new Synth and provide it with the name of a SynthDef.
x = Synth.new(\sineTest);


x.free; // to free the synth

//As is the case with many objects in SuperCollider, we create a new instance using the "new" message. "new," in this case, takes 6 arguments, but generally, you'll only specify values for the first two arguments, name and ugenGraphFunc.

//The first argument is the name you want to give to your SynthDef, which can be specified
//as either a string:
x = Synth.new("sineTest");
//or a symbol.
x = Synth.new(\sineTest);

//The second argument is a UGen function, which is nearly identical to our function from above, so we can pretty much copy and paste.

//However, there's one additional thing to consider when building a SynthDef. If you want this SynthDef to output a signal, you must include an output UGen. The most basic of these UGens is simply called Out. Out needs an output bus index, and the signal to write to that bus.

//We'll close out the SynthDef, and the last thing we need to do is "add" the new SynthDef, which sends it to the audio server so that it can be used. There are other methods to make a SynthDef usable, such as load, send, and store, but add is probably the most flexible.


x.free; // to free the synth


//The SynthDef above has one argument, noiseHz, whose default value is 8. If I wanted to create a Synth that starts with a different value for noiseHz, I can add a second argument to Synth.new. This is an array that contains the symbolic name of the argument, followed by a comma, and the value.

x = Synth.new(\sineTest, [\noiseHz, 32])

//the set message can be used to update control arguments while the Synth is active.
x.set(\noiseHz, 1)
x.free;


//another synth idea:
{RLPF.ar(Dust.ar([12, 15]), LFNoise1.ar([0.3, 0.2]).range(100, 3000), 0.02)}.play;

//lets make it a synthdef we can control
(
SynthDef.new(\dusty, {
	arg rate1=10, rate2=14;
	var amp1, amp2, in1, in2, sig1, sig2, freq1, freq2;
	in1 = Dust.ar(rate1);
	in2 = Dust.ar(rate2);
	freq1 = LFNoise1.ar(0.2).range(100, 3000);
	freq2 = LFNoise1.ar(0.3).range(100, 3000);
	sig1 = RLPF.ar(in1, freq1, 0.02);
	sig2 = RLPF.ar(in2, freq2, 0.02);
	sig1 = FreeVerb.ar(sig1, 0.7, 0.8, 0.25);
	sig2 = FreeVerb.ar(sig2, 0.7, 0.8, 0.25);
	Out.ar(0, sig1);
	Out.ar(1, sig2);
}).add;
)

x = Synth.new(\dusty);

(
x.set(\rate1, 1);
x.set(\rate2, 2,);
)


// modulate bandwidth
{ Resonz.ar(WhiteNoise.ar(0.5), 2000, XLine.kr(1, 0.001, 8)) }.play

(
SynthDef.new(\pulseTest, {
	arg ampHz=4, fund=40, maxPartial=4, width=0.5;
	var amp1, amp2, sig1, sig2, freq1, freq2;
	amp1 = LFPulse.kr(ampHz,0,0.12) * 0.75;
	amp2 = LFPulse.kr(ampHz,0.5,0.12) * 0.75;
	freq1 = LFNoise0.kr(4).exprange(fund, fund * maxPartial).round(fund);
	freq2 = LFNoise0.kr(4).exprange(fund, fund * maxPartial).round(fund);
	freq1 = freq1 * (LFPulse.kr(8)+1);
	freq2 = freq2 * (LFPulse.kr(6)+1);
	sig1 = Pulse.ar(freq1, width, amp1);
	sig2 = Pulse.ar(freq2, width, amp2);
	sig1 = FreeVerb.ar(sig1, 0.7, 0.8, 0.25);
	sig2 = FreeVerb.ar(sig2, 0.7, 0.8, 0.25);
	Out.ar(0, sig1);
	Out.ar(1, sig2);
}).add;
)

Let's give it a try.

x = Synth.new(\pulseTest);

We can change the width

x.set(\width, 0.05);
x.set(\width, 0.25);

the fundamental frequency

x.set(\fund, 50);
x.set(\fund, 60);
x.set(\fund, 80);
x.set(\fund, 160);
x.set(\fund, 30);

The maximum partial number

x.set(\maxPartial, 8);
x.set(\maxPartial, 20);

And last, we can change the rate of amplitude pulsing

x.set(\ampHz, 2);
x.set(\ampHz, 1);
x.set(\ampHz, 0.25);

x.free;

If I want to initialize this Synth with arguments that are different from the default values, I can do so by providing an array of symbol-value pairs as Synth's second argument, like this

x = Synth.new(\pulseTest, [\ampHz, 3.3, \fund, 48, \maxPartial, 4, \width, 0.15]);

x.free;

That's it for tutorial number 3. From this point on, I'll use function.play for short simple examples, and SynthDef for more complicated examples. Stay tuned for number 4, where I'll talk about envelopes and doneActions. Thanks for watching!