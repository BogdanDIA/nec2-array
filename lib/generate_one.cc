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

int main()
{
  int fs = 8000; // Sampling rate
  float time = 1; // Time we represent the signal
  int f1 = 300; // Frequency of a signal
  int f2 = 400; // Frequency of a signal
  float delta = (float) 1.0 / fs; // Delta
  int samples = (int) time * fs; // Number of samples we process

  ifstream modelf("../nec/model.nec", ios::in);
  std::vector<std::string> modelv;
  std::string symbol;
  std::string str;

  // Read Nec2 model from file.
  // We assume the file is in Unix format.
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
    std::cout << "error: the nec file is not open" << std::endl;

  ofstream outbatf("../../models/model.bat", ios::out);

  for (int i = 0; i < samples; i++)
  {
    std::ostringstream outs;
    std::ostringstream outsb;
    outs << "a/m"; outs << i; outs << ".nec";
    outsb << "a/m"; outsb << i; outsb << ".nec";
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

      // Write the model to the new file.
      // Create a DOS file.
      for (int j = 0; j < modelv.size(); j++)
        outf << modelv[j] << "\r\n";

      outf.close();
    }
    else
      std::cout << "error: out model file is not open" << std::endl;

    if (outbatf.is_open())
      outbatf << "C:\\4nec2\\exe\\4nec2.exe " << outsb.str() << " -I" << "\r\n";
    else
      std::cout << "error: out bat file is not open" << std::endl;
  }

  if (outbatf.is_open())
    outbatf.close();

  return (0);
}
