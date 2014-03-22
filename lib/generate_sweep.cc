/*
Tools for Antenna array simulation in Nec2
Copyright (C) 2014 Bogdan Diaconescu

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

static const std::string cm("CM");
static const std::string ce("CM");
static const std::string gw1("GW	1	9	0	-0.8	-0.241800	0	-0.8	0.241800	0.000100");
static const std::string gw2("GW	2	9	0	-0.6	-0.241800	0	-0.6	0.241800	0.000100");
static const std::string gw3("GW	3	9	0	-0.4	-0.241800	0	-0.4	0.241800	0.000100");
static const std::string gw4("GW	4	9	0	-0.2	-0.241800	0	-0.2	0.241800	0.000100");
static const std::string gw5("GW	5	9	0	0	-0.241800	0	0	0.241800	0.000100");
static const std::string gw6("GW	6	9	0	0.2	-0.241800	0	0.2	0.241800	0.000100");
static const std::string gw7("GW	7	9	0	0.4	-0.241800	0	0.4	0.241800	0.000100");
static const std::string gw8("GW	8	9	0	0.6	-0.241800	0	0.6	0.241800	0.000100");
static const std::string gw9("GW	9	9	0	0.8	-0.241800	0	0.8	0.241800	0.000100");
static const std::string gw100("GW	100	9	10000	0	-0.241800	10000	0	0.241800	0.0150");
static const std::string ge("GE 0");
static const std::string ld1("LD	4	1	5	5	50	0");
static const std::string ld2("LD	4	2	5	5	50	0");
static const std::string ld3("LD	4	3	5	5	50	0");
static const std::string ld4("LD	4	4	5	5	50	0");
static const std::string ld5("LD	4	5	5	5	50	0");
static const std::string ld6("LD	4	6	5	5	50	0");
static const std::string ld7("LD	4	7	5	5	50	0");
static const std::string ld8("LD	4	8	5	5	50	0");
static const std::string ld9("LD	4	9	5	5	50	0");
static const std::string gn("GN -1");
static const std::string ex("EX  0 100 5 0 1 0 0");
static const std::string fr("FR  0 0 0 0 430.0000  0");
static const std::string en("EN");

static const int az_min = -85;
static const int az_max = 85;
static const int az_step = 5;

int main()
{
  int fs = 8000; // Sampling rate
  float time = 1; // Time we represent the signal
  int f1 = 300; // Frequency of a signal
  int f2 = 400; // Frequency of a signal
  float delta = (float) 1.0 / fs; // Delta
  int samples = (int) time * fs; // Number of samples we process

  std::vector<std::string> modelv;
  std::string symbol;
  std::string str;

  // Read Nec2 model from file.
  // We assume the file is in Unix format.
  ifstream modelf("../nec/m_sweep.nec", ios::in);

  if (modelf.is_open())
  {
    while (!modelf.eof())
    {
      std::getline(modelf, str);

      modelv.push_back(str);
    }

    modelf.close();
  }
  else
    std::cout << "error: input file not open" << std::endl;

  ofstream outbatf("../../models/m_sweep.bat", ios::out);
  if (!outbatf.is_open()) 
    std::cout << "error: out .bat file not open" << std::endl;

  for (int az = az_min; az < az_max; az += az_step)
  {
    std::ostringstream symbol_x;
    std::ostringstream symbol_y;

    symbol_x << "SY ex_x=" << 10000;
    symbol_y << "SY ex_y=" << 10000*tan((double)az*2*M_PI/360);

    for (int i = 0; i < samples; i++)
    {
      std::ostringstream outs;
      std::ostringstream outsb;
      outs << "a/" << az << "_" << i << ".nec";
      outsb << "a/" << az << "_" << i << ".nec";
      ofstream outf(("../../models/" + outs.str()).c_str(), ios::out);

      std::ostringstream symbol1;
      symbol1 << "SY vr1=" << cos(2*M_PI*f1*i*delta);

      std::ostringstream symbol2;
      symbol2 << "SY vr2=" << cos(2*M_PI*f2*i*delta);

      if (outf.is_open())
      {
        // Write the SY - symbol on the first line.
        outf << symbol1.str() << "\r\n";
        outf << symbol2.str() << "\r\n";
        outf << symbol_x.str() << "\r\n";
        outf << symbol_y.str() << "\r\n";

        // Write the model to the new file.
        // Create a DOS file.
        for (int j = 0; j < modelv.size(); j++)
          outf << modelv[j] << "\r\n";

        outf.close();
      }

      if (outbatf.is_open())
        outbatf << "C:\\4nec2\\exe\\4nec2.exe " << outsb.str() << " -I" << "\r\n";
    }
  }

  if (outbatf.is_open())
    outbatf.close();
  else
    std::cout << "error: out .bat file not open" << std::endl;

  return (0);
}
