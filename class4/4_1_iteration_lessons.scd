
[6,4, 1, 2, 4, 6].do{"hi".postln};


(
[6,4, 1, 2, 4, 6].do{
	arg item;
	item.cubed.postln;
}
)

10.do
{
|i|
	i.postln;
}

4.do {
	arg i;
	i.reciprocal.postln;
}

(
z = [6, 4, 0.5, 10, 7].collect
{
	arg item;
	item.squared;
};
)

x = {VarSaw.ar(40!2, 0, 0.05)}.play;

(
SynthDef.new(\sawMESS,
	{
		arg freq = 40;
		var temp, sum, env;
		env = EnvGen.kr(
			Env.perc(0.01, 0.5, 0.4, -2),
			doneAction:2);
		sum = 0;
		20.do
		{temp = VarSaw.ar(freq *{Rand(0.99,1.01)}!2,
		{Rand(0.0,1.0)}!2,
		{ExpRand(0.005, 0.05)}!2
		);
		sum = sum + temp;
		};
		sum = sum * 0.5 * env;
		Out.ar(0, sum);
}).add;
)
x = Synth.new(\sawMESS);
x = Synth.new(\sawMESS, [\freq, 800]);
x = Synth.new(\sawMESS, [\freq, 600]);
x = Synth.new(\sawMESS, [\freq, 260]);
x = Synth.new(\sawMESS, [\freq, 20]);

x = Synth.new(\sawMESS, [\freq, 55.midicps]);
x = Synth.new(\sawMESS, [\freq, 40.midicps]);
x = Synth.new(\sawMESS, [\freq, 59.midicps]);
x = Synth.new(\sawMESS, [\freq, 66.midicps]);


x.free;

x = {Blip.ar(40, 40, 1.0)}.play

(
SynthDef.new(\blipoo, {
	arg freq = 40;
	var temp, sum;
	sum = 0;
	40.do
	{
		arg count;
		temp = SinOsc.ar(freq * (count + 1) * LFNoise1.kr({Rand(0.05,1.2)}!2).range(0.98,1.02));
		temp = temp * LFNoise1.kr({Rand(0.1,10)}!2).exprange(0.01, 1);
		sum = sum + temp;
	};
	sum = sum * 0.05;
	Out.ar(0, sum);
}).add;
)

y = Synth.new(\blipoo);
y.set(\freq, 76);
y.set(\freq, 25);

y = Synth.new(\blipoo, [\freq, 40]);

y.free;