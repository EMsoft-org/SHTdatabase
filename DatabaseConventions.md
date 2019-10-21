# SHTdatabase Conventions

Pull requests will not be accepted unless the following conventions are followed

* data stored as a *SHT file
  * paper to be cited when file is used in DOI (or the [DOI](https://doi.org/10.5281/zenodo.3515408) for this repository)
  * notes string should contain structure source information if possible (if different from DOI)
* file naming convention: 'formula (material or phase name) [structure symbol] {comments}.sht' e.g. 'Mg2SiO4 (forsterite) [S1_2] {20kV}.sht'
  * formula is required
  * name is optional but preferred
  * structure symbol is optional with Strukturbericht designations preferred (but e.g. Pearson Symbols are also acceptable)
  * short comments for EBSD files should always include accelerating voltage, tilt should be included if not 70 degrees
* folder structure 'Modality/Crystal System/File Name (without comments)/File Name.sht
  * EBSD/Cubic/Co (alpha)/Co (alpha) {20kV}.sht
  * EBSD/Cubic/Co (alpha)/Co (alpha) {25kV 65deg}.sht
  * ...
