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
#include <complex>

using namespace std;

int main()
{
  // Number of antenna elements
  int elems = 9;
  // Real impedance coupled to each element
  float zl = 50.0;

  ifstream inbatf("../../models/mutual.bat", ios::in);

  std::vector<std::string> modelv;

  // Read the bat file and get files out from it.
  if (inbatf.is_open())
  {
    while (!inbatf.eof())
    {
      std::string str, nec, outstr;
      std::getline(inbatf, str);

      // Remove the exe file and .nec extension
      std::istringstream ins(str);
      ins >> nec >> outstr;
      size_t lastdot = outstr.find_last_of(".");
      modelv.push_back(outstr.substr(0, lastdot));
    }

    inbatf.close();
  }

  vector<vector<complex<float> > > voltages(elems + 1, vector<complex<float> >(elems + 1));
  vector<vector<complex<float> > > currents(elems + 1, vector<complex<float> >(elems + 1));
  vector<vector<complex<float> > > z(elems + 1, vector<complex<float> >(elems + 1));

  // Open and parse the .out file
  for (int k = 0; k < modelv.size(); k++)
  {
    int i = 0; int j = 0;
    std::string str, tmp;
    float curr_real1, curr_real2, curr_imag1, curr_imag2;

    //cout << modelv[k] << std::endl;
    // Find the element numbers used in this out file
    sscanf(modelv[k].c_str(), "m/m_%i_%i.nec", &i, &j);
    if (i == 0 || j == 0)
      continue;

    // Open the output file and parse it
    ifstream inmodelf(("../../out/" + modelv[k] + ".out").c_str(), ios::in);

    if (inmodelf.is_open())
    {
      while (!inmodelf.eof())
      {
        std::getline(inmodelf, str);

        if (!str.find("  NO.   NO.     X        Y        Z"))
        {
          // Get current for first segment
          for (int seg = 1; seg <= 5; seg++) 
            std::getline(inmodelf, str);

          std::istringstream ins(str);
          ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp;
          ins >> curr_real1; ins >> curr_imag1;

          //cout << curr_real1 << "+" << curr_imag1 << ",";

          // Get current for second segment
          for (int seg = 1; seg <= 9; seg++) 
            std::getline(inmodelf, str);

          ins.str(str);
          ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp;
          ins >> curr_real2; ins >> curr_imag2;

          //cout << curr_real2 << "+" << curr_imag2 << std::endl;
          //cout << "i: " << i << "," << "j: " << j << std::endl;

          if (i != j)
          {
            voltages[i][j] = complex<float>(zl * curr_real1, zl * curr_imag1);
            currents[i][j] = complex<float>(curr_real2, curr_imag2);
          }
          else
          {
            voltages[i][i] = complex<float>(zl * curr_real1, zl * curr_imag1);
            currents[i][i] = complex<float>(0.0, 0.0);
          }

          //cout << voltages[i][j] << "," << currents[i][j] << std::endl;
        }
      }
    }
    else
      std::cout << "error: input file not open" << std::endl;

    inmodelf.close();
  }

  // Generate the Z matrix and write it to the file
  ofstream outf("../out/mutual.out", ios::out);

  if (outf.is_open())
  {
    for (int i = 1; i <= elems; i++)
    {
      for (int j = 1; j <= elems; j++)
      {
        if (i == j)
          z[i][j] = 1;
        else
          z[i][j] = -(voltages[i][j] - voltages[i][i]) / currents[i][j] / complex<float>(zl, 0.0);

        outf << z[i][j].real() << "+" << z[i][j].imag() << "i" << ",";
      }
      outf << std::endl;
    }
  }
  else
    std::cout << "error: out file is not open" << std::endl;

  outf.close();

  return (0);
}
