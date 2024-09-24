s.boot;
s.makeWindow;

().play

( freq:[440*(1/1),(5/4)] ).play;

(midinote: {60+1.0.xrand2}.dup(100)).play;

e = (sustain: 4, freq: [69, 73].midicps).play
// major triad in 12 tempered tuning

f = (sustain: 4, freq: {440 * [1/1, 5/4]}).play
//major triad in just

{SinOsc.ar(100)}.play;


{ SinOsc.ar([60, 63].midicps, 0, 0.5) }.play;
//
{ SinOsc.ar(261.625565*[1/1, 7/6], 0, 0.5) }.play;
{ SinOsc.ar(261.625565*[1/1, 6/5], 0, 0.5) }.play;

(a = NodeProxy.new.play; // play to hardware output.
a.fadeTime = 1; // fadeTime specifies crossfade
// set the source
a.source = { };) // start your engines, A is empty!

a.source = { }; // start your engines, A is empty!


a.source = { SinOsc.ar([350, 351.3], 0, 0.2) };

a.source = { SinOsc.ar(350*[1/1, 6/5], 0, 0.2) };

a.source = { SinOsc.ar(350*1/1, 0, 0.2) };

a.scope;
a.source = { LFPulse.ar(350*[1/1, 3/2], 0, 0.5, 0.2) }; //change to square

a.source = { LFTri.ar(350*[1/1, 3/2], 0, 0.5, 0.2) }; //change to square

a.source = { LFSaw.ar(350, 0, 0.2) }; // change to sawtooth

(a.source =
{ var volume;
	volume = MouseY.kr(0, 0.1, 0);
	LFSaw.ar(220*[1/1, 3/2], 0, volume);};

) // add a volume knob...using a veriable

a.source = { LFSaw.ar(350*[1/1, 3/2], 0, 0.2) }; // change interval

a.source = { LFSaw.ar(MouseX.kr(10,50), 0, 0.9) };  // show rhythm LFO

a.source = { LFSaw.ar(Line.ar(0.1, 50, 10)*[1/1, 3/2], 0, 0.2) };  // slide rhythm to pitch

a.source = { LFSaw.ar(MouseX.kr(1, 100, 10)*[1/1, 5/4, 7/4], 0, 0.2) };  // slide rhythm to pitch w cursor

a.source = { Blip.ar(350*[1/1, 3/2], (MouseX.kr(1,16,0)), 0.5)}; // slide XY to show different overtones over fundamental
/
a.source = { Blip.ar(100, (MouseX.kr(1,32,0)), 0.5)};  //try more overtones


a.free;


a.scope;

//just tuned 3rd
