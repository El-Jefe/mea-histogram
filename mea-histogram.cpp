/*
* MEA histogram module for RTXI 2.x
*/

#include <mea-histogram.h>
#include <main_window.h>
#include <qwt_plot_histogram.h>
#include <iostream>
#include <QtGui>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new MeaHistogram();
}

static DefaultGUIModel::variable_t vars[] = {
    { "Vm", "Membrane Voltage (in mV)", DefaultGUIModel::INPUT, },
	{ "ISI", "ISI (ms)", DefaultGUIModel::OUTPUT, },
	{ "Threshold (mV)", "Threshold (mV) at which to detect a spike",
		DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{
		"Min Interval (ms)",
		"Minimum interval (refractory period) that must pass before another spike is detected",
		DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Average ISI (ms)", "Average ISI (ms)", DefaultGUIModel::STATE, },
	{ "CV", "Coefficient of Variation", DefaultGUIModel::STATE, },
	{ "# Spikes", "# Spikes", DefaultGUIModel::STATE, },
	{ "Time (s)", "Time (s)", DefaultGUIModel::STATE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

MeaHistogram::MeaHistogram(void) : DefaultGUIModel("MEA Histogram", ::vars, ::num_vars), thresh(-0.02), min_int(5e-3), spktime(0), prevspktime(0), state(0) {
	setWhatsThis("<p><b>MEA Histogram:</b><br>QWhatsThis description.</p>"); // TODO
	DefaultGUIModel::createGUI(vars, num_vars); // this is required to create the GUI
	customizeGUI();
	update( INIT ); // this is optional, you may place initialization code directly into the constructor
	refresh(); // this is required to update the GUI with parameter and state values
}

MeaHistogram::~MeaHistogram(void) { }

void MeaHistogram::execute(void) {
	systime = RT::OS::getTime();
	
	switch (state) {
		case 0:
			// for input(0)
			// pretty sure we will have input(1) to input(60), so detecting separate spikes should be easy
			if (input(0) > thresh) state = 1;// spike happened
			break;
	
		case 1:
			state = 2;
			break;

		case 2:
			if (input(0) > thresh && (systime - spktime) > 100) state = 4;
			else if (input(0) < thresh) state = 3;
			break;

		case 3:
			state = -1;
			break;
		
		case 4:
			if (input(0) < thresh) state = -1;
			break;

		case -1:
			if (systime - spktime > min_int) state = 0;
			break;
	
		default:
			break;
	}
	
	if (state == 1) countspikes();

	if (spikecount > 2) { // ignore first period
		runningPeriod.push(ISI);
		ISImean = runningPeriod.mean();
		ISIstd = runningPeriod.std();
		CV = runningPeriod.std() / runningPeriod.mean();
		printf("%f %f\n", ISI, ISImean);
	}
	
	output(0) = ISI; // send state information as output
	return;
}

void MeaHistogram::countspikes() {
	prevspktime = spktime;
	spktime = systime;
	ISI = (spktime - prevspktime) * 1e-6;
	spikecount++;
}

void MeaHistogram::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("Threshold (mV)", QString::number(thresh * 1000.0)); // in V, display in mV
			setParameter("Min Interval (ms)", QString::number(min_int * 1000)); // in s, display in ms
			setState("Average ISI (ms)", ISImean);
			setState("CV", CV);
			setState("# Spikes", spikecount);
			break;

		case MODIFY:
			thresh = getParameter("Threshold (mV)").toDouble() / 1000.; // displayed in mV, convert to V
			min_int = getParameter("Min Interval (ms)").toDouble() / 1000; // displayed in ms, convert to s
			break;

		case PAUSE:
			break;
		
		case UNPAUSE:
			reset();
			break;

		case PERIOD:
			dt = RT::System::getInstance()->getPeriod() * 1e-9; // s
		
		default:
			break;
	}
}

void MeaHistogram::reset() {
	runningPeriod.clear();
	CV = 0;
	systime = 0;
	state = 0;
	ISI = 0;
	ISImean = 0;
	ISIstd = 0;
	CV = 0;
	spikecount = 0;
	spktime = 0;
	prevspktime = 0;
}

void MeaHistogram::initParameters() {
	dt = RT::System::getInstance()->getPeriod() * 1e-9; // s
	reset();
}

void MeaHistogram::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	
	QGroupBox *bttnGroup = new QGroupBox;
	QHBoxLayout *bttnGroupLayout = new QHBoxLayout;
	bttnGroup->setLayout(bttnGroupLayout);
	QPushButton *resetButton = new QPushButton("Reset Statistics");
	bttnGroupLayout->addWidget(resetButton);
	QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	
	customlayout->addWidget(bttnGroup, 0, 0);
	setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
// void MeaHistogram::aBttn_event(void) { }

// void MeaHistogram::bBttn_event(void) { }

