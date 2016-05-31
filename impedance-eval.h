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

#include <default_gui_model.h>
#include <basicplot.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

class ImpedanceEval : public DefaultGUIModel {

	Q_OBJECT

	public:
		ImpedanceEval(void);
		virtual ~ImpedanceEval(void);

		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);

	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		int n;
		double impedance;
		double period;
		double duration;
		std::vector<double> source; // buffer for data before event
		std::vector<double> measured; // buffer for data before event
		void compute();
		double computeMag(std::vector<double>& v);
		void square(std::vector<double>& v);
		double mean(const std::vector<double>& v);
		double sum(const std::vector<double>& v);

		private slots:
			void reset(void);
};
