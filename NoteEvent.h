#pragma once
#include<vector>
#include"Sinewave.h"
class NoteEvent
{
	public:
		std::vector<Sinewave> sines;
		int attack=0, hold=0, decay=0, release=0;//Unit: steps(Stamp)
		double sustain = 1.0,basefrequency=440.0;
		double releaseFrom = 0.0;//Release�����ǿ�ȿ�ʼ
		int noteStart=-99999,noteReleaseTime=-1;//-1:�յģ�-2��û������
		int MidiID=0, velocity=127;//max 128
		int unision = 1, offset = 0.1;

		static double sampleRate;
		
		static double toAmp(double dB);
		static double powCurve(double x);//��������ָ�����ʽ
		static int toStamp(double millionSeconds);
		
		NoteEvent();
		NoteEvent(int mid,int startTime, int vel,int fre);

		void setAHDSR(double a, double h, double d, double s, double r);
		void releaseNote(int timeStamp);//�����ɿ�����ʼrelease
		bool playCompleted(int timeStamp);//�����Ƿ񲥷����
		double EnvAHDSR(int currentStamp);//ȡ�õ�ǰ������ֵ
};

