s.boot;

s.makeWindow;



(a = NodeProxy.new.play; // play to hardware output.
a.fadeTime = 1; // fadeTime specifies crossfade
// set the source
a.source = { };) // start your engines, A is empty!

a.source = { }; // start your engines, A is empty!

(a.source =
{ var ampMod, mod, kY, kX;
	kY = MouseY.kr(0, 0.1, 0);
	kX = MouseX.kr(0.1, 200, 0);
	mod = kX;
	ampMod = SinOsc.ar(kX*[1, 1.5], 0, 0.5);
	LFSaw.ar(220*[mod*1.1, mod*1], 0, ampMod*[1, 1.5]);};
) // AM modulation 2 ch

(a.source =
{ var ampMod, kY, kX;
	kX = MouseX.kr(0.1, 200, 0);
	ampMod = SinOsc.ar(kX, 0, 1.0);
	LFTri.ar(220, 0, ampMod);};
) //simple AM


(
a.source =
{ var ampMod, mod, kY, kX;
	kY = MouseY.kr(0, 20, 0);
	kX = MouseX.kr(0.1, 200, 0);
	ampMod = SinOsc.ar(kX, 0, kY);
	LFTri.ar(220, 0, ampMod);};
) //simple AM w offset


(
a.source =
{ var ampMod, freqMod, kY, kX;
	kY = MouseY.kr(0, 100, 0);
	kX = MouseX.kr(0.1, 100, 0);
	freqMod = SinOsc.ar(kX, 0, kY);
	LFTri.ar(200+freqMod, 0, 1.0);};
) //simple FM w offset

//try LFSsaw, LFPulse, LFTri, etc

(
a.source =
{ var sig, mod, kY, kX;
	kY = MouseY.kr(0, 100, 0);
	kX = MouseX.kr(20, 2000, 0);
	mod =  SinOsc.ar(kX, 0, 1.0);
	sig = PinkNoise.ar(1.0);
	RLPF.ar(sig, 100+mod, 1.0, 1.0);
};
)

(
a.source =
{ var sig, mod, kY, kX;
	kY = MouseY.kr(40, 8000, 0);
	kX = MouseX.kr(0.1, 20, 0);
	mod = SinOsc.ar(kX, 0, 1000, kY);
	sig = Saw.ar(60, 0.1);
	sig = PinkNoise.ar();
	RLPF.ar(sig, mod, 0.1);};
)
// input to filter

a.source = ();




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
