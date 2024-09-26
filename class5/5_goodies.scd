n = NodeProxy.audio(s,2).play;
n.gui;

n[0] = { | delay = 1 decay = 5, amp = 1|
	var audio, freq, db;
	freq = MouseX.kr(300, 2500);
	db = MouseY.kr(-60, -20 );
	audio = SinOsc.ar(freq, 0, amp * db.dbamp);
	audio = CombC.ar(audio, 1, delay, decay) + audio;
	// gate = Slope.kr(MouseY.kr).abs;
	// audio = Linen.kr(gate, 0.0, 1, 0.1) * audio;
	Pan2.ar(audio)
};

n[0] = { | delay = 1 decay = 5, amp = 1, edecay  = 1, eattack = 0 |
	var audio, freq, db, gate;
	freq = MouseX.kr(300, 2500);
	db = MouseY.kr(-60, -20 );
	audio = SinOsc.ar(freq, 0, amp * db.dbamp);
	audio = CombC.ar(audio, 1, delay, decay) + audio;
	gate = Slope.kr(MouseY.kr).abs;
	audio = Linen.kr(gate, eattack, 1, edecay) * audio;
	Pan2.ar(audio)
};


n[0] = { | rq = 0.01 |
	var gate, audio, wave;
	wave = SinOsc;
	wave = Saw;
//	wave = LFTri;
	gate = Slope.kr(MouseY.kr).abs;
	audio = wave.ar(freq: MouseX.kr(1, 500, 3), mul: 0.1);
	audio = Linen.kr(gate, 0.0, 1, 3).cubed * audio;
   audio = RLPF.ar(audio, MouseY.kr(400,1000), rq);
	Pan2.ar(Limiter.ar(audio, 0.1))
};



n[0] = { | decay = 1, amp = 0.1|
	var gate, audio, freq, formant;
	gate = Slope.kr(MouseY.kr(lag: 0)).abs;
	freq = MouseX.kr(1, 500, 'exponential');
	formant = freq.linlin(1, 500, 400, 1000);
	formant = formant * MouseY.kr(0.1,5, 'exponential');
	audio = Formant.ar(freq, formant, formant/5 );
	audio = Linen.kr(gate, 0.0, 1, decay) * audio;
	audio = audio * amp;
	Pan2.ar(audio, 0.1)
};
