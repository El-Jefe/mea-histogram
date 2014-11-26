/*
* MEA histogram module for RTXI 2.x
*/

#include <mea-histogram.h>
#include <main_window.h>
#include <iostream>
#include <QtGui>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new MeaHistogram();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "GUI label", "Tooltip description", DefaultGUIModel::PARAMETER
		| DefaultGUIModel::DOUBLE, },
	{ "A State", "Tooltip description", DefaultGUIModel::STATE, }, 
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

MeaHistogram::MeaHistogram(void) : DefaultGUIModel("MEA Histogram", ::vars, ::num_vars) {
	setWhatsThis("<p><b>MEA Histogram:</b><br>QWhatsThis description.</p>");
	DefaultGUIModel::createGUI(vars, num_vars); // this is required to create the GUI
	customizeGUI();
	update( INIT ); // this is optional, you may place initialization code directly into the constructor
	refresh(); // this is required to update the GUI with parameter and state values
}

MeaHistogram::~MeaHistogram(void) { }

void MeaHistogram::execute(void) {
	return;
}

void MeaHistogram::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			setParameter("GUI label", some_parameter);
			setState("A State", some_state);
			break;
	
		case MODIFY:
			some_parameter = getParameter("GUI label").toDouble();
			break;

		case UNPAUSE:
			break;

		case PAUSE:
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			break;
	
		default:
			break;
	}
}

void MeaHistogram::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	
	QGroupBox *button_group = new QGroupBox;
	
	QPushButton *abutton = new QPushButton("Button A");
	QPushButton *bbutton = new QPushButton("Button B");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(abutton);
	button_layout->addWidget(bbutton);
	QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));
	QObject::connect(bbutton, SIGNAL(clicked()), this, SLOT(bBttn_event()));
	
	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
void MeaHistogram::aBttn_event(void) { }

void MeaHistogram::bBttn_event(void) { }

