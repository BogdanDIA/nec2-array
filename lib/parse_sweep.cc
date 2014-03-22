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

using namespace std;

int main()
{
  ifstream inbatf("../../models/m_sweep.bat", ios::in);
  std::string str, nec, outstr;

  std::vector<std::string> modelv;
  std::string symbol;

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
  else
    std::cout << "error: input .bat file not open" << std::endl;
 
  // Get the phi angle for the first entry
  int prev_phi = 0; int sample = 0;
  sscanf(modelv[0].c_str(), "a/%i_%i.out", &prev_phi, &sample);
  std::ostringstream outs;
  outs << "../out/signal_" << prev_phi << ".out";
  ofstream outf(outs.str().c_str(), ios::out);

  cout << outs.str() << std::endl;

  for (int i = 0; i < modelv.size(); i++)
  {
    ifstream inmodelf(("../../out/" + modelv[i] + ".out").c_str(), ios::in);

    int phi = 0;
    sscanf(modelv[i].c_str(), "a/%i_%i.out", &phi, &sample);

    if (prev_phi != phi)
    {
      outf.close();

      std::ostringstream outs;
      outs << "out/signal_" << phi << ".out";
      outf.open(outs.str().c_str(), ios::out);

      cout << outs.str() << std::endl;

      prev_phi = phi;
    }

    if (inmodelf.is_open() && outf.is_open())
    {
      while (!inmodelf.eof())
      {
        std::istringstream ins;
        std::string tmp;
        std::string curr_real;
        std::string curr_imag;

        std::getline(inmodelf, str);

        if (!str.find("  NO.   NO.     X        Y        Z"))
        {
          for (int seg = 1; seg <= 5; seg++) 
            std::getline(inmodelf, str);

          ins.str(str);
          ins >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;
          ins >> curr_real; ins >> curr_imag;

          outf << curr_real << "+" << curr_imag << "i" << ",";

          for (int elements = 1; elements <= 8; elements++)
          {
            for (int seg = 1; seg <= 9; seg++) 
              std::getline(inmodelf, str);

            ins.str(str);
            ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp;
            ins >> curr_real; ins >> curr_imag;
  
            outf << curr_real << "+" << curr_imag << "i" << ",";
          }

          outf << std::endl;
        }
      }

      if (inmodelf.is_open())
        inmodelf.close();
    }
  }

  if (outf.is_open())
    outf.close();

  return (0);
}
