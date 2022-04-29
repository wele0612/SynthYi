#include "NoteEvent.h"
#include<cmath>


double NoteEvent::EnvAHDSR(int currentStamp)
{
	int step = currentStamp - noteStart;
	if (step <= 0) {//before
		releaseFrom = 0.0;
		return 0.0;
	}
	else if (currentStamp > noteReleaseTime && currentStamp < noteReleaseTime + release && noteReleaseTime != -2) {
		//release
		int releaseStep = noteReleaseTime + release - currentStamp;
		return releaseFrom*(powCurve((double)releaseStep / (double)release));//Release±ØÐëÏÈÅÐ¶Ï
	}
	else if (step > 0 && step < attack) {//attack
		releaseFrom= powCurve((double)step / (double)attack);
		return releaseFrom;
	}
	else if (step <= attack + hold) {//hold
		releaseFrom = 1.0;
		return 1.0;
	}
	else if (step <= attack + hold + decay) {//decay
		double nowPercentage = powCurve(1 - ((double)(step - attack - hold) / (double)decay));
		releaseFrom= (1.0 - sustain) * nowPercentage + sustain;
		return releaseFrom;
	}
	else if (noteReleaseTime == -2 || currentStamp < noteReleaseTime) {//sustain
		releaseFrom = sustain;
		return sustain;
	}
	
	return 0.0;//after
}


double NoteEvent::toAmp(double dB)
{
	return std::pow(20, dB * 0.1);
}

double NoteEvent::powCurve(double x)
{
	return std::pow(21.0, x) / 20.0 - 0.05;
}

int NoteEvent::toStamp(double millionSeconds)
{
	return (int)(millionSeconds/1000.0*sampleRate);
}

NoteEvent::NoteEvent()
{
	return;
}

NoteEvent::NoteEvent(int mid, int startTime, int vel,int fre)
{
	MidiID = mid;
	noteStart = startTime;
	velocity = vel;
	basefrequency = fre;
	noteReleaseTime = -2;
	for (int i = 1; i < 50; i++) {
		//sines.push_back(Sinewave(fre * (double)i, 1.0 / (double)i, 0.0));//¾â³Ý²¨
		sines.push_back(Sinewave(fre * (2 * (double)i - 1.0), 1.0 / (2 * (double)i - 1.0), 0.0));//·½²¨
	}
}

void NoteEvent::setAHDSR(double a, double h, double d, double s, double r)
{
	attack = toStamp(a);
	hold = toStamp(h);
	decay = toStamp(d);
	sustain = s;
	release = toStamp(r);
}

void NoteEvent::releaseNote(int timeStamp)
{
	noteReleaseTime = timeStamp;
}

bool NoteEvent::playCompleted(int timeStamp)
{
	return (noteReleaseTime!=-2)&&timeStamp>(noteReleaseTime+release);
}

double NoteEvent::sampleRate = 44100;