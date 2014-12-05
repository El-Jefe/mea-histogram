/*
* Header file for the MEA histogram module
*/

#include <QMdiArea>
#include <default_gui_model.h>
#include <runningstat.h>

class MeaHistogram : public DefaultGUIModel {
	
	Q_OBJECT
	
	public:
	
		MeaHistogram(void);
		virtual ~MeaHistogram(void);
	
		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);
		
	
	protected:
	
		virtual void update(DefaultGUIModel::update_flags_t);
	
	private:
		// parameters
		double thresh;
		double min_int;
		// time of spikes
		double spktime;
		double prevspktime;
		RunningStat runningPeriod;
		double ISI;
		double ISImean;
		double ISIstd;
		double CV;
		double spikecount;
		// the internal state variable, sent as output
		int state;
		double systime;
		double dt; // real-time
		// function prototypes
		void initParameters();
		void countspikes();
	
	private slots:
	// these are custom functions that can also be connected
	// to events through the Qt API. they must be implemented
	// in plugin_template.cpp
	
		//void aBttn_event(void);
		//void bBttn_event(void);
		void reset();
	
};
