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

static const std::string ld1("LD	4	1	5	5	50	0");
static const std::string ld2("LD	4	2	5	5	50	0");
static const std::string ld3("LD	4	3	5	5	50	0");
static const std::string ld4("LD	4	4	5	5	50	0");
static const std::string ld5("LD	4	5	5	5	50	0");
static const std::string ld6("LD	4	6	5	5	50	0");
static const std::string ld7("LD	4	7	5	5	50	0");
static const std::string ld8("LD	4	8	5	5	50	0");
static const std::string ld9("LD	4	9	5	5	50	0");

static const std::string ex("EX  0 100 5 0 1 0 0");
static const std::string fr("FR  0 0 0 0 430.0000  0");

int main()
{
  ofstream outbatf("../../models/mutual.bat", ios::out);
  std::vector<std::string> elemv;
  std::vector<std::string> ldv;

  std::string symbol;
  std::string str;

  elemv.push_back("dummy");
  elemv.push_back(gw1);
  elemv.push_back(gw2);
  elemv.push_back(gw3);
  elemv.push_back(gw4);
  elemv.push_back(gw5);
  elemv.push_back(gw6);
  elemv.push_back(gw7);
  elemv.push_back(gw8);
  elemv.push_back(gw9);

  ldv.push_back("dummy");
  ldv.push_back(ld1);
  ldv.push_back(ld2);
  ldv.push_back(ld3);
  ldv.push_back(ld4);
  ldv.push_back(ld5);
  ldv.push_back(ld6);
  ldv.push_back(ld7);
  ldv.push_back(ld8);
  ldv.push_back(ld9);

  for (int i = 1; i <= elemv.size() - 1; i++)
  {
    std::ostringstream modelx;

    modelx << "CM" << "\r\n";
    modelx << "CE" << "\r\n";
    modelx << elemv[i] << "\r\n";
    modelx << gw100 << "\r\n";
    modelx << "GE" << "\r\n";
    modelx << ldv[i] << "\r\n";
    modelx << "GN -1" << "\r\n";
    modelx << "EK" << "\r\n";
    modelx << ex << "\r\n";
    modelx << fr << "\r\n";
    modelx << "EN" << "\r\n";

    std::ostringstream outsx;
    outsx << "m/m" << "_" << i << "_" << i << ".nec";
    ofstream outfx(("../../models/" + outsx.str()).c_str(), ios::out);

    if (outfx.is_open())
    {
      outfx << modelx.str();

      outfx.close();
    }

    if (outbatf.is_open())
      outbatf << "C:\\4nec2\\exe\\4nec2.exe " << outsx.str() << " -I" << "\r\n";
    else
      std::cout << "error: out file not open" << std::endl;

    for (int j = 1; j <= elemv.size() - 1; j++)
    {
      if (i != j)
      {
        std::ostringstream modelxy;

        modelxy << "CM" << "\r\n";
        modelxy << "CE" << "\r\n";
        modelxy << elemv[i] << "\r\n";
        modelxy << elemv[j] << "\r\n";
        modelxy << gw100 << "\r\n";
        modelxy << "GE" << "\r\n";
        modelxy << ldv[i] << "\r\n";
        modelxy << ldv[j] << "\r\n";
        modelxy << ex << "\r\n";
        modelxy << fr << "\r\n";
        modelxy << "EN" << "\r\n";

        std::ostringstream outsxy;
        outsxy << "m/m" << "_" << i << "_" << j << ".nec";
        ofstream outfxy(("../../models/" + outsxy.str()).c_str(), ios::out);

        if (outfxy.is_open())
        {
          outfxy << modelxy.str();

          outfxy.close();
        }
        else
          std::cout << "error: out file not open" << std::endl;

        if (outbatf.is_open())
          outbatf << "C:\\4nec2\\exe\\4nec2.exe " << outsxy.str() << " -I" << "\r\n";
        else
          std::cout << "error: out .bat file not open" << std::endl;
      }
    }
  }

  if (outbatf.is_open())
    outbatf.close();

  return (0);
}
