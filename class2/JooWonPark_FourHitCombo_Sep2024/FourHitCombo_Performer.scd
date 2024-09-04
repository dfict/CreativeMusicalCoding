/*
Four Hit Combo
by Joo Won Park
©2024
Performer part

Instruction
1. Open this file in SuperCollider
2. In the SuperCollider menu, select Language-> Evaluate File. A GUI window will appear.
3. Drag-and-drop an audio file (wav, aif, or mp3) to the top rentangle. A sound will immediately play.
4. Use the bottom-left 2D slider to control the loop. The horizontal move changes the loop point. The vertical move changes the loop duration.
5. Press the bottom-right button to randomly change the loop starting point and the loop duration
6. Close the GUI window or press command/control + period to stop
*/
s.waitForBoot{
	//SynthDefs
	~thesynth={
		SynthDef("Stut",{
			arg amp,lagtime,rate,pan,rangemul,where,range;
			var bufdur,trig, start, end,sound,env,rtrig,mix;

			//end = ((range)*bufdur*~looprate).reciprocal;
			end=(range*rangemul).reciprocal;
			trig=LFPulse.ar(end);
			rtrig=Lag.ar(TRand.ar(pan.neg,pan,trig));

			// env
			env=(LFGauss.ar(end.reciprocal,0.5)-0.1).clip(0.0,1.0);

			start=~buffer.numFrames*where;
			sound = PlayBuf.ar(~buffer.numChannels,~buffer.bufnum,BufRateScale.kr(~buffer.bufnum),trig,start,0);

			if (~buffer.numChannels==1,
				{mix = Pan2.ar(sound,rtrig)*amp*env;},
				{mix = Pan2.ar(sound.sum,rtrig)*amp*env});

			Out.ar(0,mix);

		}).add;
	};//end of SynthDef
	//~thesynth.();

	~playsynth={
		~looper=Synth.head(0,"Stut",[\amp,0.5,\rate,1,\pan,rrand(0.1,0.6),\rangemul,1,\where,0.4,\range,0.1]);
		~looper2=Synth.head(0,"Stut",[\amp,0.5,\rate,rrand(0.99,1.01),\pan,rrand(0.1,0.6),\rangemul,1,\where,0.4,\range,0.4]);
	};
	~resetsynth={~looper.free;~looper2.free};

	//GUI
	~height=Window.screenBounds.height*0.6;
	~width=Window.screenBounds.width*0.5;

	~guiwindow = Window.new("Twenty Six Changes (2024)");
	~guiwindow.bounds=Rect(Window.screenBounds.width*0.1,Window.screenBounds.height*0.1,~width,~height);
	~guiwindow.background=Color.rand;
	~guiwindow.front;

	~title=StaticText(~guiwindow,Rect(~width*0.15,~height*0.9,~width*0.8,~height*0.05));
	~title.string="By Joo Won Park www.joowonpark.net/twentysix";
	~title.font=Font("Courier",15);

	~dragdrop0 = DragSink(~guiwindow, Rect(~width*0.15,~height*0.10,~width*0.7,~height*0.25));
	~dragdrop0.font=Font("Courier",15);
	~dragdrop0.background=Color.white;
	~dragdrop0.string = "   Drag and drop a sample";

	//~thesynth.(0);
	//SystemClock.sched(1,{~resetsynth.(0);~playsynth.(0);});
	~dragdrop0.receiveDragHandler={arg incoming;
		~dragdrop0.object = View.currentDrag.value;
		~buffer=Buffer.read(s,~dragdrop0.object,
			action:{
				~thesynth.(0); //reload synths
				~buffer.numFrames.postln;
				SystemClock.sched(0.05,{
					~resetsynth.(0);~playsynth.(0);
					{~randbutton.doAction;}.defer;
					{~guiwindow.background=Color.rand}.defer;
				});
			}
		);
	};

	~fader = Slider2D(~guiwindow, Rect(~width*0.15,~height*0.4,~width*0.3,~width*0.3));
	~fader.setXY(0.5,0.5);
	~fader.background=Color.black;
	~fader.action=({
		arg sl;
		var where,range;
		where=sl.x.linlin(0.0,1.0,0.0,1.00);
		range=sl.y.linlin(0.0,1.0,1.0,0.06);
		~looper.set(\where,where,\range,range);
		~looper2.set(\where,where,\range,range);
	});

	~randbutton=Button(~guiwindow, Rect(~width*0.55,~height*0.4,~width*0.3,~width*0.3));
	~randbutton.font=Font("Courier",24);
	~randbutton.states=([["Random",Color.black,Color.white]]);
	~randbutton.action={
		arg randon;
		var xrand,yrand;
		xrand=rrand(0.1,0.85);
		yrand=rrand(0.04,0.92);
		~fader.x=xrand;
		~fader.y=yrand;
		~looper.set(\where,xrand,\range,yrand);
		~looper2.set(\where,xrand,\range,yrand);
	};

	{~guiwindow.onClose={s.freeAll;"closed".postln}}.defer;
	CmdPeriod.doOnce({~guiwindow.close});
};