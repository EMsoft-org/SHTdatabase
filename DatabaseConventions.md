# SHTdatabase Conventions

Pull requests for new structure .sht files will only be accepted if the following conventions are followed:

* data must be stored as a *.sht formatted file
  * the DOI field should contain the paper to be cited when the file is used by other researchers (or the [DOI](https://doi.org/10.5281/zenodo.3515408) for this repository);
  * the notes string should contain structure source information if possible (if different from the DOI number).
* file naming convention: the file name should adhere to the following general structure:

```fortran
'formula (material or phase name) [structure symbol] {comments}.sht'
   e.g. 'Mg2SiO4 (forsterite) [S1_2] {20kV}.sht'
```
* In this file name, the following conventions are to be used (note the use of delimiters and spaces):
  * the **formula** field is required;
  * the **name** field is optional but preferred; if not used, then the parenthesis should be omitted;
  * the **structure symbol** field is optional with StrukturBericht designations preferred (but e.g., Pearson Symbols are also acceptable).  If not used, then the square brackets should be omitted;
  * short comments for EBSD files are placed between curly braces and should always include the accelerating voltage (in kV); the sample tilt angle should be included if not equal to 70°.
  * the following are valid file names:

```fortran
  Ni [A2] {30kV}.sht
  Ni [cF4] {20kV 65degrees}.sht
  Mg2SiO4 (forsterite) [S1_2] {10kV}.sht
  Ag {15kV}.sht
  ...
```   
* To allow for the use of the database for other scattering modalities, the preferred folder structure is as follows:

```fortran 
 'Modality/Crystal System/File Name (without comments)/File Name.sht
    * EBSD/Cubic/Co (alpha)/Co (alpha) {20kV}.sht
    * EBSD/Cubic/Co (alpha)/Co (alpha) {25kV 65deg}.sht
    * ...
```