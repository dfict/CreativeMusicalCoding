/*
Four Hit Combo
by Joo Won Park
©2024
Performer part

Instruction
1. Open this file in SuperCollider
2. In the SuperCollider menu, select Language-> Evaluate File. A GUI window will appear.
3. Drag-and-drop an audio file (wav, aif, or mp3) to the top rentangle. A sound will immediately play.
4. Use the bottom-left slider to control the loop duration.
5. Type a number in the bottom-right text box to deermine the duration of the sound. The changed number will be used for the next audio drag-and-drop (step #3)
6. Close the GUI window or press command/control + period to stop
*/

s.waitForBoot{
	//SynthDefs
	~thesynth={
		SynthDef("Stut",{
			arg amp,lagtime,rate,pan,rangemul,where,range,dur;
			var bufdur,trig, start, end,sound,env,rtrig,mix;

			end=(range*rangemul).reciprocal;
			trig=LFPulse.ar(end);
			rtrig=Lag.ar(TRand.ar(pan.neg,pan,trig));

			//env
			env=(LFGauss.ar(end.reciprocal,0.5)-0.1).clip(0.0,1.0);

			start=~buffer.numFrames*Line.kr(0,1.0,dur);
			sound = PlayBuf.ar(~buffer.numChannels,~buffer.bufnum,BufRateScale.kr(~buffer.bufnum),trig,start,0);

			if (~buffer.numChannels==1,
				{mix = Pan2.ar(sound,rtrig)*amp*env;},
				{mix = Pan2.ar(sound.sum,rtrig)*amp*env});

			Out.ar(0,mix);

		}).add;
	};//end of SynthDef
	//~thesynth.();

	~playsynth={
		~looper=Synth.head(0,"Stut",[\amp,0.5,\rate,1,\pan,rrand(0.1,0.6),\rangemul,1,\where,0.4,\range,0.1,\dur,~dur]);
		~looper2=Synth.head(0,"Stut",[\amp,0.5,\rate,rrand(0.99,1.01),\pan,rrand(0.1,0.6),\rangemul,1,\where,0.4,\range,0.4,\dur,~dur]);
	};
	~resetsynth={~looper.free;~looper2.free};

	//long sample duration init
	~dur=100;

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

	~title2=StaticText(~guiwindow,Rect(~width*0.55,~height*0.5,~width*0.8,~height*0.05));
	~title2.string="Set Length ->";
	~title2.font=Font("Courier",15);

	~title3=StaticText(~guiwindow,Rect(~width*0.55,~height*0.7,~width*0.8,~height*0.05));
	~title3.string="<- Change Grain Dur";
	~title3.font=Font("Courier",15);

	~dragdrop0 = DragSink(~guiwindow, Rect(~width*0.15,~height*0.10,~width*0.7,~height*0.25));
	~dragdrop0.font=Font("Courier",15);
	~dragdrop0.background=Color.white;
	~dragdrop0.string = "   Drag and drop a sample (10-20sec long)";

	//~thesynth.(0);
	//SystemClock.sched(1,{~resetsynth.(0);~playsynth.(0);});
	~dragdrop0.receiveDragHandler={arg incoming;
		~dragdrop0.object = View.currentDrag.value;
		~buffer=Buffer.read(s,~dragdrop0.object,
			action:{
				//~dur=200;
				~thesynth.(0); //reload synths
				~buffer.numFrames.postln;
				SystemClock.sched(0.05,{
					~resetsynth.(0);~playsynth.(0);
					{~fader.doAction;}.defer;
					{~sectionlength.doAction;}.defer;
					{~guiwindow.background=Color.rand}.defer;
				});
			}
		);
	};

	~fader = Slider(~guiwindow, Rect(~width*0.15,~height*0.4,~width*0.3,~width*0.3));
	~fader.value=0.1;
	~fader.background=Color.black;
	~fader.action=({
		arg sl;
		var where,range;
		range=sl.value.linlin(0.0,1.0,0.1,0.06);
		~looper.set(\range,range);
		~looper2.set(\range,range);
	});

	~sectionlength = NumberBox(~guiwindow, Rect(~width*0.75,~height*0.4,~width*0.1,~width*0.3));
	~sectionlength.font=Font("Courier",40);
	~sectionlength.value=100;
	~sectionlength.background=Color.white;
	~sectionlength.action=({
		arg thenumber;
		~dur=thenumber.value;
	});

	{~guiwindow.onClose={s.freeAll;"closed".postln}}.defer;
	CmdPeriod.doOnce({~guiwindow.close});
};