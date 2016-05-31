/*
			Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

			This program is free software: you can redistribute it and/or modify
			it under the terms of the GNU General Public License as published by
			the Free Software Foundation, either version 3 of the License, or
			(at your option) any later version.

			This program is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			GNU General Public License for more details.

			You should have received a copy of the GNU General Public License
			along with this program.  If not, see <http://www.gnu.org/licenses/>.
			*/

#include "impedance-eval.h"
#include <main_window.h>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new ImpedanceEval();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Source Signal", "Signal delivered to the electrode in volts", DefaultGUIModel::INPUT, },
	{ "Measured Signal", "Signal measured after the electrode in volts", DefaultGUIModel::INPUT, },
	{ "Duration", "Duration for measurement in seconds", DefaultGUIModel::PARAMETER, }, 
	{ "Impedance", "Computed impedance in ohms", DefaultGUIModel::STATE, }, 
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

ImpedanceEval::ImpedanceEval(void) : DefaultGUIModel("Impedance Evaluation", ::vars, ::num_vars) {
	setWhatsThis("<p><b>Impedance Evaluation:</b><br>Measureme electrode impedance in RTXI.</p>");
	DefaultGUIModel::createGUI(vars, num_vars); // this is required to create the GUI
	customizeGUI();
	update(INIT); // this is optional, you may place initialization code directly into the constructor
	refresh(); // this is required to update the GUI with parameter and state values
	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

ImpedanceEval::~ImpedanceEval(void) { }

void ImpedanceEval::execute(void) {
	source.push_back(input(0)); // V
	measured.push_back(input(1)); // mA with 1kO r_sense
	return;
}

void ImpedanceEval::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			period = RT::System::getInstance()->getPeriod() * 1e-9; // ns
			duration = 5; // s
			n = duration / period;
			impedance = 0.0;
			setState("Impedance", impedance);
			setParameter("Duration", duration);
			source.resize(n);
			measured.resize(n);
			reset();
			break;

		case MODIFY:
			duration = getParameter("Duration").toDouble();
			break;

		case UNPAUSE:
			reset();
			break;

		case PAUSE:
			compute();
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-9; // ns
			break;

		default:
			break;
	}
}

void ImpedanceEval::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	QGroupBox *button_group = new QGroupBox;
	QPushButton *abutton = new QPushButton("Reset");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(abutton);
	QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(reset()));
	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}

void ImpedanceEval::reset(void)
{
	source.clear();
	measured.clear();
}

void ImpedanceEval::compute()
{
	double source_rms = computeMag(source);
	double measured_rms = computeMag(measured);
	impedance = source_rms / measured_rms;
}

double ImpedanceEval::computeMag(std::vector<double>& v)
{
	square(v);
	double mean_val = mean(v);
	double rms = std::sqrt(mean_val);
	return rms;
}

void ImpedanceEval::square(std::vector<double>& v)
{
	std::transform(v.begin(), v.end(), v.begin(), [](double d) { return d * d; });
}

double ImpedanceEval::sum(const std::vector<double>& v)
{
	return std::accumulate(v.begin(), v.end(), 0.0, std::plus<double>());
}

double ImpedanceEval::mean(const std::vector<double>& v)
{
	return sum(v) / v.size();
}
