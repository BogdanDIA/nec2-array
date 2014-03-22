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
  ifstream inbatf("../../models/model.bat", ios::in);
  std::string str, nec, outstr;

  std::vector<std::string> modelv;
  std::string symbol;

  // Read the bat file and get files out from it.
  if (inbatf.is_open())
  {
    while (!inbatf.eof())
    {
      std::getline(inbatf, str);

      std::istringstream ins(str);
      ins >> nec; ins >> outstr;

      // Remove the .nec extension
      size_t lastdot = outstr.find_last_of(".");

      modelv.push_back(outstr.substr(0, lastdot));
    }

    inbatf.close();
  }

  ofstream outf("../out/signal.out", ios::out);
  std::istringstream ins;
  std::string tmp;
  std::string curr_real;
  std::string curr_imag;

  for (int i = 0; i < modelv.size(); i++)
  {
    ifstream inmodelf(("../../out/" + modelv[i] + ".out").c_str(), ios::in);

    if (inmodelf.is_open())
    {
      while (!inmodelf.eof())
      {
        std::getline(inmodelf, str);

        if (!str.find("  NO.   NO.     X        Y        Z"))
        {
          for (int seg = 1; seg <= 5; seg++) 
            std::getline(inmodelf, str);

          ins.str(str);
          ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp; ins >> tmp;
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
    }

    inmodelf.close();
  }

  outf.close();

  return (0);
}
