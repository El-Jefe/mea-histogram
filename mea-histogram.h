/*
* Header file for the MEA histogram module
*/

#include <QMdiArea>
#include <default_gui_model.h>

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
	
		double some_parameter;
		double some_state;
		double period;
	
	private slots:
	// these are custom functions that can also be connected
	// to events through the Qt API. they must be implemented
	// in plugin_template.cpp
	
		void aBttn_event(void);
		void bBttn_event(void);
	
};
