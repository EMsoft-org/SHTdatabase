/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                 *
 * Copyright (c) 2019, De Graef Group, Carnegie Mellon University                  *
 * All rights reserved.                                                            *
 *                                                                                 *
 * Author: William C. Lenthe                                                       *
 *                                                                                 *
 * Redistribution and use in source and binary forms, with or without              *
 * modification, are permitted provided that the following conditions are met:     *
 *                                                                                 *
 * 1. Redistributions of source code must retain the above copyright notice, this  *
 *    list of conditions and the following disclaimer.                             *
 *                                                                                 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,    *
 *    this list of conditions and the following disclaimer in the documentation    *
 *    and/or other materials provided with the distribution.                       *
 *                                                                                 *
 * 3. Neither the name of the copyright holder nor the names of its                *
 *    contributors may be used to endorse or promote products derived from         *
 *    this software without specific prior written permission.                     *
 *                                                                                 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    *
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   *
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <fstream>
#include <sstream>

#include "sht_file.hpp"

//@brief: rename sht file following database guidelines
int main(int argc, char *argv[]) {
	try {
		//sanity check argument count
		if(3 != argc) {
			std::cout << "usage: " << argv[0] << " inputFile dbPath\n";
			std::cout << "\tinputFile - spherical hamrnoics file to rename (*.sht)\n";
			std::cout << "\tdbPath    - path to database\n";
			return EXIT_FAILURE;
		}

		//open file and read
		sht::File file;
		std::ifstream is(argv[1], std::ios::in | std::ios::binary);
		file.read(is);

		//get modality (root folder)
		std::string dir(argv[2]);
		switch(file.header.modality()) {
			case sht::Modality::EBSD   : dir += "/EBSD/"; break;
			case sht::Modality::ECP    : dir += "/ECP/" ; break;
			case sht::Modality::TKD    : dir += "/TKD/" ; break;
			case sht::Modality::PED    : dir += "/PED/" ; break;
			case sht::Modality::Laue   : dir += "/Laue/"; break;
			case sht::Modality::Unknown:
			default                       : throw std::runtime_error("unknown or invalid modality");
		}

		//get crystal system
		if     (file.mpData.sgEff() <=   0) throw std::runtime_error("invalid space group");
		else if(file.mpData.sgEff() <=   2) dir += "Triclinic/";
		else if(file.mpData.sgEff() <=  15) dir += "Monoclinic/";
		else if(file.mpData.sgEff() <=  74) dir += "Orthorhombic/";
		else if(file.mpData.sgEff() <= 142) dir += "Tetragonal/";
		else if(file.mpData.sgEff() <= 167) dir += "Trigonal/";
		else if(file.mpData.sgEff() <= 194) dir += "Hexagonal/";
		else if(file.mpData.sgEff() <= 230) dir += "Cubic/";
		else throw std::runtime_error("invalid space group");

		//build pieces of file name
		std::string frm, mat, syb;
		for(const sht::CrystalData& xtal : file.mpData.xtals) {//loop over crystals
			//get elements of name for this crystal
			std::string xFrm = xtal.form.substr(0, xtal.formulaLen  ());
			std::string xMat = xtal.name.substr(0, xtal.matNameLen  ());
			std::string xSyb = xtal.symb.substr(0, xtal.structSymLen());

			//accumulate
			if(!xFrm.empty()) {
				if(!frm.empty()) frm += '+';//seperate overlaped formulas with plus sign
				frm += xFrm;
			}
			if(!xMat.empty()) {
				if(!mat.empty()) mat += '+';
				mat += xMat;
			}
			if(!xSyb.empty()) {
				if(!syb.empty()) syb += '+';
				syb += xSyb;
			}
		}

		//assemble file prefix
		std::string prefix = frm;
		if(!mat.empty()) prefix += " (" + mat + ")";
		if(!syb.empty()) prefix += " [" + syb + "]";
		if(prefix.empty()) throw std::runtime_error("empty crystal name");
		dir += prefix;

		//handle modality specific naming (currently there are only rules for EBSD)
		std::string suffix = ".sht";
		if(sht::Modality::EBSD == file.header.modality()) {
			std::ostringstream ss;
			ss << " {" << file.header.beamEnergy() << "kV";
			if(70.0f != file.header.primaryAngle()) ss << ' ' << file.header.primaryAngle() << "deg";
			ss << "}";
			suffix = ss.str() + suffix;
		}

		//assembly complete file name
		std::string fn = dir + '/' + prefix + suffix;
		std::cout << dir << '\n';
		std::cout << fn << '\n';

	} catch(std::exception& e) {
		std::cout << e.what() << '\n';
	}
	return EXIT_SUCCESS;
}
